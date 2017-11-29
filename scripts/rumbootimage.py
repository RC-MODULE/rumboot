import sys
import binascii
import argparse
import os

class RumbootImage():
        def __init__ (self, inFile):
            self.fd = open(inFile, 'wb')
            self.cur_pos = 0
            self.fd.seek(0, os.SEEK_END)
            self.file_size = self.fd.tell()
            self.header_size = 66
            self.fd.seek(0, os.SEEK_SET)

        def deinit():
            self.fd.close()

        def read_field(self, offset, name, len):
            self.fd.seek(offset, os.SEEK_SET)    # move the file pointer forward 0 bytes (i.e. to the 'w')
            self[name] = self.fd.read(len)
            self.cur_pos = self.fd.tell()
            return self[name]

        def write_field(self, offset, name, len, value):
            self.fd.seek(offset, os.SEEK_SET)    # move the file pointer forward 0 bytes (i.e. to the 'w')
            self[name] = value
            self.fd.write(self[name])
            self.cur_pos = self.fd.tell()
            return self[name]

        def load(self):
            self.fd.seek(0, os.SEEK_SET)
            self[header] = self.fd.read(self.header_size)
            self[image] = self.fd.read()
            self[data_crc32] = binascii.crc32(self[image])
            self[datalen] = self.file_size - self.header_size
            self[header_crc32] = binascii.crc32(self[header])

        def save(self, inFile):
            self.fd.seek(0, os.SEEK_SET)
            self.fd.write(self[header])
            self.fd.seek(header_size, os.SEEK_SET)
            self.write(self[image])
            self.write_field(self, 8, data_crc32, 4, self[data_crc32])
            self.write_field(self, 12, datalen, 4, self[datalen])
            self.write_field(self, 16 + 4*11, header_crc32, 4, self[header_crc32])

        def check(self):
            if self[magic] == 0xb01dface:
                return True
            else:
                return False

        def inject_fault(self, field):
            self[field] += 1

        def pad(self):
            if self.file_size % 4 != 0:
                self.fd.seek(0, os.SEEK_END)
                self.fd.write(0, 4 - self.file_size % 4)

def create_arg_parser ():
    parser = argparse.ArgumentParser()
    parser.add_argument ('-c', '--check', nargs='?')
    parser.add_argument('-i', '--input', nargs='?')
    parser.add_argument('-o', '--output', nargs='?')
    parser.add_argument('-f', '--fault', nargs='?')
    parser.add_argument('-p', '--pad', nargs='?')

    return parser

if __name__ == '__main__':
    parser = create_arg_parser()
    args = parser.parse_args()

    # print (namespace)
    if args.input and args.output:
        print "Input file name {}".format(args.input)
        input_file = args.input
        print "Output file name {}".format(args.output)
        output_file = args.output
    else:
        print("ERROR: input file name must be!")
        print("ERROR: output file name must be!")
        sys.exit(1)

    rumboot_img = RumbootImage(input_file)
    rumboot_img.load()
    rumboot_img.save(input_file)

    if args.check:
        print("Check file.")
        rumboot_img.check()

    if args.fault:
        print("Inject the fault in file.")
        rumboot_img.inject_fault(args.fault)

    if args.pad:
        print("PAD")
        rumboot_img.pad()

    rumboot_img.deinit()

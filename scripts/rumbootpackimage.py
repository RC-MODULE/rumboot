import sys
import binascii
import argparse
import os



class RumbootImage():

        fields = {'magic': 0,'datalen': 0, 'data_crc32': 0, 'header_crc32': 0}

        def __init__ (self, inFile):
            self.is_loaded = False
            self.fd = open(inFile, 'r+b')
            self.cur_pos = 0
            self.file_size = self.get_file_size()
            self.header_size = 66

            self.image = 0
            self.header = 0
            self.magic = 0

        def get_file_size(self):
            file_size = 0
            self.fd.seek(0, os.SEEK_END)
            file_size = self.fd.tell()
            print("File size: {}".format(file_size))
            self.fd.seek(0, os.SEEK_SET)
            return file_size

        def deinit(self):
            self.fd.close()

        def read_field(self, offset, name, len):
            self.fd.seek(offset, os.SEEK_SET)    # move the file pointer forward 0 bytes (i.e. to the 'w')
            self[name] = self.fd.read(len)
            self.cur_pos = self.fd.tell()
            return self[name]

        def write_field(self, offset, name, len, value):
            print("write field {} with offset: {}, len: {}, value: {}".format(name, offset, len, hex(value)))

            endian = self.get_endian()

            self.fd.seek(offset, os.SEEK_SET)    # move the file pointer forward 0 bytes (i.e. to the 'w')
            self.fields[name] = bytearray(value.to_bytes(len, endian))

            self.fd.write(self.fields[name])
            print("written bytes: {}".format( self.fields[name]) )

            self.cur_pos = self.fd.tell()

        def calc_fields(self):
            #Compatibility with all Python versions and platforms - & 0xffffffff
            self.fields['data_crc32'] = binascii.crc32(self.image) & 0xffffffff
            self.fields['datalen'] = self.file_size - self.header_size
            self.fields['header_crc32'] = binascii.crc32(self.header) & 0xffffffff

        def write_fields(self):
            self.write_field(8, "data_crc32", 4, self.fields['data_crc32'])
            self.write_field(12, "datalen", 4, self.fields['datalen'])
            self.write_field(16 + 4*11, "header_crc32", 4, self.fields['header_crc32'])

        def load(self):
            self.fd.seek(0, os.SEEK_SET)
            self.header = bytearray(self.fd.read(self.header_size))
            self.image = bytes(self.fd.read())
            self.fields['magic'] = bytearray(self.header[0:4])
            print("MAGIC: {}".format( binascii.hexlify(self.fields[ 'magic']) ))

            self.calc_fields()

            self.is_loaded = True

        def save(self, inFile):
            if self.is_loaded == False:
                self.load()

            self.fd.seek(0, os.SEEK_SET)
            self.fd.write(self.header)
            self.fd.seek(self.header_size, os.SEEK_SET)
            self.fd.write(self.image)

            self.write_fields()

        def check(self):
            if self.is_loaded == False:
                self.load()

            magic = binascii.hexlify(self.fields[ 'magic'])
            if int(magic, 16) == int(0xB01DFACE):
                print("Magic is OK!")
            else:
                print("ERROR: magic is wrong! magic: {}".format(magic))
                sys.exit(1)

        #FIX THIS!
        def inject_fault(self, field):
            if self.is_loaded == False:
                self.load()

            self.fields[field] += 1

        def pad(self):
            if self.is_loaded == False:
                self.load()

            if self.file_size % 4 != 0:
                self.fd.seek(0, os.SEEK_END)
                self.fd.write(0, 4 - self.file_size % 4)

        def get_endian(self):
            byteorder = 'unknow'
            magic = int(binascii.hexlify(self.fields[ 'magic']), 16)
            if magic == int(0xCEFA1DB0):
                byteorder = 'little'
            if magic == int(0xB01DFACE):
                byteorder = 'big'
            return byteorder

        def check_endian(self):
            if self.is_loaded == False:
                self.load()

            byteorder = self.get_endian(self)
            if sys.byteorder == byteorder:
                return True
            else:
                return False

        def pack(self):
            self.load(self)
            self.calc_fields()
            self.write_fields()

def create_arg_parser ():
    parser = argparse.ArgumentParser()
    parser.add_argument ('-c', '--check', nargs='?')
    parser.add_argument('-i', '--input', nargs='?')
    parser.add_argument('-f', '--fault', nargs='?')
    parser.add_argument('-a', '--pad', nargs='?')
    parser.add_argument('-e', '--endian', nargs='?')
    parser.add_argument('-p', '--pack', nargs='?')

    return parser

if __name__ == '__main__':
    parser = create_arg_parser()
    args = parser.parse_args()

    if args.input:
        print("Input file name {}".format(args.input))
        input_file = args.input
    else:
        print("ERROR: input file name must be!")
        sys.exit(1)

    rumboot_img = RumbootImage(input_file)
    rumboot_img.load()
    rumboot_img.check()
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

    if args.endian:
        print("Check if the endian the same with the host endian.")
        rumboot_img.check_endian()

    if args.pack:
        print("Pack image.")
        rumboot_img.pack()

    rumboot_img.deinit()

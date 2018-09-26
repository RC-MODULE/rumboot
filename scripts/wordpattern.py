#!/usr/bin/env python3
import argparse

parser = argparse.ArgumentParser(formatter_class=argparse.RawDescriptionHelpFormatter,
                                 description="wordpattern.py - Generate word pattern files\n")
parser.add_argument('-s', '--size',
                       nargs=1, metavar=('value'),
                       required=True,
                       help='''minimum size of the file in bytes
                       ''')

parser.add_argument('-f', '--file',
                       nargs=1, metavar=('value'),
                       required=True,
                       help='''minimum size of the file in bytes
                       ''')

parser.add_argument('-i', '--increment',
                       nargs=1, metavar=('value'),
                       help='''how much to increment
                       ''')

parser.add_argument('-e', '--endian',
                       nargs=1, metavar=('value'),
                       help='''endianness
                       ''')



opts = parser.parse_args()

endian = "little"
size = int(opts.size[0])
filename = opts.file[0]

if size % 4 > 0:
    size = size + 4
size = size / 4

fd = open(filename, 'w+b')

for x in range(0, int(size)):
    fd.write(bytearray(x.to_bytes(4, endian)))

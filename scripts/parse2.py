import sys, re

fout = open("../test.h", 'w')
fin  = open("../outputfile.txt", 'r')

sys.stdout = fout

for line in fin.readlines():
   result = re.split(r' ', line)
   func = result[1].strip()
   addr = "0x"+result[0]
   if func != ".hidden":
    print "#define", func, addr

fout.close()
fin.close()

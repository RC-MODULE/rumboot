from parse import parse 
import glob
import sys
import io
import os
import re

def count_brackets(line):
    br = 0
    for c in line:
        if c=='{':
            br+=1
        if c=='}':
            br-=1
    return br

def dump_structs(filename):
    print(f'// Processing {filename} / {os.path.basename(filename).replace(".c","")}')
    with open(filename, "r") as fl:
        data = fl.readlines()
        n_open = False
        n_brack = 0
        for line in data:
            if (line.find("regpoker_checker") != -1 and line.find("[]") != -1):
                n_open = True
                first = True
            if n_open:
                if n_brack > 0 or first:
                    sys.stdout.write(line)
                n_brack += count_brackets(line)
                if n_brack == 0 and first:
                    continue
                if n_brack == 0:
                    n_open = False
                first = False

for d in sys.argv[1:]:
    for f in glob.glob(d + "**/*.c", recursive=True):
        dump_structs(f)

import subprocess
from pathlib import Path
import re
from itertools import chain
import sys 

def stripcomments(text):
    return re.sub('//.*?\n|/\*.*?\*/', '', text, flags=re.S)

def comment_remover(text):
    def replacer(match):
        s = match.group(0)
        if s.startswith('/'):
            return " " # note: a space and not an empty string
        else:
            return s
    pattern = re.compile(
        r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
        re.DOTALL | re.MULTILINE
    )
    return re.sub(pattern, replacer, text)

def format_definition(line, symbol, addr):
    line = line.replace(symbol, "(*rom_" + symbol + ")")
    line = line.replace(";", "= (void *) 0x%x;" % addr)
    return line

def locate_definition(srcdir, symbol):
    bootapi = Path(srcdir + '/include/rumboot').glob('**/boot.h')
    pf = Path(srcdir + '/include/rumboot').glob('**/printf.h')
    xmodem =  Path(srcdir + '/include/rumboot').glob('**/xmodem.h')
    files = chain(bootapi, xmodem, pf)
    for filename in files:
        tmp = open(str(filename), "r")
        lines = tmp.read()
        lines = stripcomments(lines)
        definition = None
        for line in lines.split("\n"):
            if (definition != None):
                definition = definition + line
                if (line.find(";") >= 0):
                    return definition
            if line.find(" " + symbol) >=0:
                definition = line
                if (line.find(";") >= 0):
                    return definition

#node = parse_file("../include/rumboot/boot.h", 
#use_cpp=True,
#cpp_args=[
#    r'-E',
#    r'-I../include/platform/bbp3', 
#    r'-Ipycparser/utils/fake_libc_include/',
#    r'-D__attribute__(x)=',
#])
#
#TODO: ...

srcdir = sys.argv[1]
elf = sys.argv[2]

print("#ifndef __RUMBOOT_ROM_API")
print("#define __RUMBOOT_ROM_API")

cmdline = "readelf -s -W " + elf
cmd = subprocess.Popen(cmdline, shell=True, stdout=subprocess.PIPE)
for line in cmd.stdout:
    decoded = line.decode().split()
    if len(decoded) == 0:
        continue
    addr = decoded[1]
    try:
        addr = int(addr, 16)
    except Exception as e: 
        #print(e)
        continue
    ttype = decoded[3]
    scope = decoded[4]
    if (ttype == "FUNC") and (scope == "GLOBAL"):
        symbol = decoded[7]
        if (symbol == "rumboot_printf"):
            continue
        df = locate_definition(srcdir, symbol)
        if df == None:
            continue
        df = format_definition(df, symbol, addr)
        print(df)
print("#endif")
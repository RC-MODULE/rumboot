import serial
import sys
from xmodem import XMODEM
import os
from parse import parse
import ft232
import time
import io


ser = serial.Serial(sys.argv[1], 115200, timeout=1) # or whatever port you need
def getc(size, timeout=10):
    ret = ser.read(size)
    return ret or None
def putc(data, timeout=10):
    return ser.write(data)  # note that this ignores the timeout

print("Waiting for ROM to boot")

def poll_for_invite(welcome):
    while True:
        l = ser.readline()
        print(l.decode(), end='')
        if (l == welcome):
            print("Got invitation!")
            ser.write("X".encode())
            while True:
                tmp1 = ser.read(1)
                tmp2 = ser.read(1)
                if tmp1 == b"C" and tmp2 == b"C": 
                    break
            break
    
def xmodem_payload(fl, chunksize=0, welcome=b"boot: host: Hit 'X' for xmodem upload\n"):
    poll_for_invite(welcome)
    print("Sending payload")

    stream = open(fl, 'rb')
    modem = XMODEM(getc, putc)
    if chunksize==0:
        ret = modem.send(stream, retry=16)
    else:
        while True:
            data = stream.read(chunksize)
            if (not data):
                    break
            data = io.BytesIO(data)
            ret = modem.send(data, retry=16)
            poll_for_invite(welcome)            
    print("Delivered!")

xmodem_payload(sys.argv[2])

while True:
    line = ser.readline().decode()
    ret = parse("boot: host: Back in rom, code {}", line)
    print(line, end='')
    if ret:
       code = ret[0]
       exit(int(code))

exit(8)

import os
import sys
import argparse
from struct import *

def readWord():
    char = f.read(1)
    while char[0] != 0:
        sys.stdout.write("%s" %char.decode('ascii'))
        char = f.read(1)
    sys.stdout.write('\n')

parser = argparse.ArgumentParser(description='path to file')
parser.add_argument('path')
args = parser.parse_args()
print("Attempting to read file at %s..."% args.path)

f = open(args.path, 'rb')
format_type = (f.read(4)).decode('ascii')
if format_type == 'YM5!':
    print("YM5! format file detected based on first four bytes of \
file...")
else:
    print("Unexpected format detected")

f.seek(0x13)
interleaved = f.read(1)
if interleaved[0] == 1:
    print("File is interleaved...")
else:
    print("File is not interleaved...")
f.seek(0x22)

sys.stdout.write("Song title: ")
readWord()
sys.stdout.write("Author: ")
readWord()
sys.stdout.write("Comments: ")
readWord()

byte = f.read(1)
song_data = []
while len(byte) > 0:
    song_data.append(byte)
    byte = f.read(1)

song_data.pop()
song_data.pop()
song_data.pop()
song_data.pop()
print("Data length is %i bytes..." %len(song_data))
print("Song length is %d frames..." %(len(song_data)/16.0))



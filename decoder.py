import os
import sys
import argparse
import time
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
    song_data.append(byte[0])
    byte = f.read(1)

for i in range(0,4): 
    song_data.pop()

song_data_len = len(song_data)
n_frames = song_data_len/16.0

print("Data length is %i bytes..." %song_data_len)
print("Song length is %d frames..." %song_data_len)

if interleaved[0] ==1:
    print("Deinterleaving...")

registers = {}
for i in range(0,16):
    registers[i] = []
    
    j=0
    while j < n_frames:
        registers[i].append(song_data.pop(0))
        j+=1

current_time = time.time()
j=0
while j < n_frames:
    current_time = time.time()
    frame = {}
    j+=1
    for i in range(0,16):
        frame[i] = registers[i].pop(0)
        print("Register {reg_n}: {value} {bin_value}".format( \
            reg_n=i, value=hex(frame[i]), bin_value=bin(frame[i])))
    while time.time() < (current_time + 5): 
        time.sleep(0.01)         





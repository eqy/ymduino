import os
import sys
import argparse
import array
import time
import serial
import struct
import math

def readWord():
    char = f.read(1)
    while char[0] != 0:
        sys.stdout.write("%s" %char.decode('ascii'))
        char = f.read(1)
    sys.stdout.write('\n')

parser = argparse.ArgumentParser(description='path to file')
parser.add_argument('path')
parser.add_argument('force_interleaved')
args = parser.parse_args()
print("Attempting to read file at %s..."% args.path)

f = open(args.path, 'rb')
format_type = (f.read(4)).decode('ascii')
old_format = False
reg_count = 16
if format_type == 'YM5!':
    print("YM5! format file detected based on first four bytes of \
file...")
elif format_type == 'YM3!' or format_type == 'YM3b':
    old_format = True
else:
    print("Unexpected format detected")

if not old_format:
    f.seek(0xc)
    n_frames = struct.unpack('>i', f.read(4))[0]
    print(n_frames)

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
else:
    n_frames = math.floor((os.path.getsize(args.path)-4)/14)
    interleaved = [1]
    reg_count = 14

byte = f.read(1)
song_data = []
while len(byte) > 0:
    song_data.append(byte[0])
    byte = f.read(1)

for i in range(0,4): 
    song_data.pop()

song_data_len = len(song_data)
#n_frames = song_data_len/16.0

print("Data length is %i bytes..." %song_data_len)
print("Song length is %d frames..." %n_frames)

registers = {}
for i in range(0,reg_count):
    registers[i] = [] 
if interleaved[0] == 1 or args.force_interleaved == 'force_interleaved':
    print("Deinterleaving...")
    for i in range(0,reg_count):
        #j=0
        registers[i]=song_data[i*n_frames:(i+1)*n_frames]
        #while j < n_frames:
        #    try:
        #        registers[i].append(song_data.pop(0))
        #        j+=1
        #    except IndexError:
        #        print("Couldn't parse end of file, doing best effort playback")
        #        break
else:
    j=0
    while j < n_frames:
        for i in range(0,reg_count):
            try:
                registers[i].append(song_data.pop(0))
            except IndexError:
                print("Couldn't parse end of file, doing best effort playback")
                j = n_frames
                break
        j+=1 

#arduino = serial.Serial('/dev/ttyACM0', 9600)
current_time = time.time()
j=0
all_frames = array.array('B')
while j < n_frames:
    try:
        current_time = time.time()
        frame = array.array('B')
        for i in range(0,14):
            current_reg = registers[i].pop(0)
            frame.append(current_reg)
            all_frames.append(current_reg)
            print("Register {reg_n:>2d}: 0x{value:02x} {bin_value:08b}".format( \
                reg_n=i, value=current_reg, bin_value=current_reg))
        #arduino.write(frame)
        print(\
        "==Frame#{fr:05d}/{tot:05d}======".format(fr=int(j),tot=int(n_frames-1)))
        #time.sleep(0.02)         
        j+=1
    except IndexError:
        print("reached end of frames early...")
        break

with open('outputfile', 'wb') as f:
    all_frames.tofile(f)






import os
import sys
import argparse

parser = argparse.ArgumentParser(description='path to file')
parser.add_argument('path')
args = parser.parse_args()
print("Attempting to read file at %s..."% args.path)

f = open(args.path, 'rb')
format_type = (f.read(4)).decode('ascii')
if format_type == 'YM5!':
    print("YM5! format file detected based on first four bytes of \
file...") 

f.seek(0x14,0x00)
interleaved = f.read(1)
if interleaved:
    print("File is interleaved...")
else:
    print("File is not interleaved...")

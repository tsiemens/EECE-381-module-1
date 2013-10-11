#imToRaw16.py
# This file encodes most images into a custom rgb565 format
# The first 8 bytes are two integers for the width and height
# The remaining bytes (2*width*height) are 16 bit rgb565 encoded pixels
import Image
import sys
import struct
import ctypes
import math

argumentError = "Requires argumens: [source image file] [name of output raw16 (with extension)]"
imageError = "Input file must be a readable image"

if len(sys.argv) < 2:
    sys.exit(argumentError)

startIndex = 0
if sys.argv[0].endswith(".py"):
    startIndex = 1;
    if len(sys.argv) < 3:
        sys.exit(argumentError)

image = Image.open(sys.argv[startIndex])
if image == None:
    sys.exit(imageError)

output = None
try:
    output = open(sys.argv[startIndex + 1], "r")
    r = raw_input("File "+sys.argv[startIndex + 1]+" already exits. Overwite? [y|n]")
    r.strip()
    if r.lower().find('y', 0, 1) != 0:
        sys.exit("Aborting")
except IOError:
    pass

output = open(sys.argv[startIndex + 1], "wb")
if output == None:
    sys.exit("File io error")

# Tuple (width, height)
#print "width:", image.size[0]
#print "height:", image.size[1]

data32 = bytearray(4)
data16 = bytearray(2)

pos = 0
struct.pack_into(">i", data32, 0, image.size[0])
output.write(data32)

struct.pack_into(">i", data32, 0, image.size[1])
output.write(data32)

for y in range(0,image.size[1]):
    for x in range(0,image.size[0]):
        t = x, y
        c = image.getpixel(t)
        #print c
        temp = c[0]
        temp = int(round(temp * (31.0/255.0)))
        twoBytes = (temp << 11)
        temp = c[1]
        temp = int(round(temp * (63.0/255.0)))
        twoBytes = twoBytes ^ (temp << 5)
        temp = c[2]
        temp = int(round(temp * (31.0/255.0)))
        twoBytes = twoBytes ^ temp
        #print "%04x"%twoBytes
        struct.pack_into(">H", data16, 0, twoBytes)
        output.write(data16)

output.close()
print "Succeeded"


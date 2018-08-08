#!/usr/bin/env python3
import cv2
import os
import argparse
import sys

def remap(x, bits):
    return int(x * ((1<<bits)-1) / 255)

parser = argparse.ArgumentParser()
parser.add_argument('input', help='Path to input image')
parser.add_argument('output', nargs='?', help='Path to output folder', default=os.getcwd())
args = parser.parse_args()

img = cv2.imread(args.input)
if img is None:
    print("Could not open: {}".format(args.input), file=sys.stderr)
    exit(1)

height, width, _ = img.shape

pixels = []
for x in range(height):
    for y in range(width):
        px = img[x, y]
        r = remap(px[0], 5)
        g = remap(px[1], 6)
        b = remap(px[2], 5)

        # rgb565
        val = (r << 11) | (g << 5) | b
        # byte swap
        val = (val >> 8) | ((val & 0xFF) << 8)
        pixels.append(hex(val))


try:
    os.mkdir(args.output)
except FileExistsError as e:
    pass

TEMPLATE = """
#pragma once


const uint8_t image_{name}_width = {width};
const uint8_t image_{name}_height = {height};
const uint16_t image_{name}_data[] = {{
    {pixels}
}};
"""

name = os.path.splitext(os.path.basename(args.input))[0]
with open(os.path.join(args.output, "{}.h".format(name)), 'w') as out:
    out.write(TEMPLATE.format(
        name=name,
        width=height - 1,
        height=width - 1,
        pixels=",\n\t".join([", ".join(pixels[x:x+width]) for x in range(0, height*width, width)])
    ).lstrip())


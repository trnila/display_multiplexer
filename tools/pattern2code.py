#!/usr/bin/env python3
import sys
import os


def cumsum(items):
    sum = 0
    yield 0
    for x in items:
        sum += x
        yield sum


def process(path):
    commands = []
    y = 0
    with open(path) as file:
        for line in file.readlines():
            x = 0
            for c in line.strip():
                if c == 'O':
                    commands.append(x)
                    commands.append(y)
                x += 1

            y += 1

    return commands

if len(sys.argv) != 2:
    print("Usage: {} source_dir".format(sys.argv[0]))
    exit(1)

patterns = []
src = sys.argv[1]
for obj in os.listdir(sys.argv[1]):
    path = os.path.join(src, obj)
    if os.path.isfile(path):
        patterns.append(process(path))


TEMPLATE = """
int gameoflife_offsets[] = {{{sizes}}};
uint8_t gameoflife_patterns[] = {{{patterns}}};
"""

offsets = cumsum([len(pattern) for pattern in patterns])

sizes = ", ".join(map(str, offsets))
out = ", \n".join([", ".join([hex(y) for y in x]) for x in patterns])

print(TEMPLATE.format(sizes=sizes, patterns=out))
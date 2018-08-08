#!/bin/sh
rm -rf ./Inc/images
for file in images/*; do
  python3 tools/img2rgb565.py "$file" ./Inc/images
done

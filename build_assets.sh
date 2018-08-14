#!/bin/sh

# build images
rm -rf ./Inc/assets
mkdir -p ./Inc/assets/images

for file in assets/images/*; do
  python3 tools/img2rgb565.py "$file" ./Inc/assets/images
done

# build patterns
./tools/pattern2code.py assets/patterns/ > Inc/assets/patterns.h

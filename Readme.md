# Display multiplexer

## Build new bitmap for ILI9225
Add file to *images/* directory and run `./gen_images.sh`.
Header file with data will be created in *Inc/images/*.

## Build
```sh
$ ./gen_images.sh
$ mkdir build
$ cd build
$ cmake ..
$ make -j4

```

## Flash
```sh
$ openocd -f ../ocd-maple-stlink.cfg -c "init" -c "reset init" -c "flash write_image erase display_multiplexer.bin 0x08000000" -c "reset" -c "exit"
```

# Display multiplexer

## Build assets
Add assets to the *assets/* directory and run `./build_assets.sh`, that will generate headers files into `Inc/assets/`.

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

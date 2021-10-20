# Photo Mosaic

Receives a PPM image and returns a mosaic of smaller images (tiles) that fit best in each chunk of the bigger one.

## Compilation
Compile using `make`
```
$ make
```

## Usage
```
$ ./mosaico [-p directory] [-i file] [-o file]
```
Parameters: 
- `-p` : tile directory (default: `./tiles`)
- `-i` : input image file (default: `stdin`)
- `-o` : output image file (default: `stdout`)

The program will convert the input image to a mosaic of the tiles in the tile directory.

## Image format
The input image should be in the format PPM (P3 or P6). The output will be in the same format as the input.
The tiles should also be in one of these two formats.


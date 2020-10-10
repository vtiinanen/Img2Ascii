# Img2Ascii
https://github.com/vtiinanen/Img2Ascii

A program that reads image file and converts it to ASCII character representation. The ASCII output is printed to stdio.

Written in C.

Pro tip: link math lib when compiling, use flag -> -lm

### Example usage:

main.o -f example.jpg -s 0.5

### options:
**-f path/to/file** (filename)
-s 0.5 (scale to the original)
-b (flag to use more characters)

##### Uses following external libraries:
stb_image.h
stb_image_resize.h
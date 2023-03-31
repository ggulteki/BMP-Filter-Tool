# simple-filters-to-BMPs-tool

This is the implementation of a program that applies filters to BMP.

This program applies the following four simple visual effects to BMP files:

Grayscale -g
This filter turns the given image into black and white.

Reflection -r
This filter converts the given image into its reflection in the mirror.

Blur -b
This filter blurs the given image.

Edges -e
This filter brings out the edges of the objects in the given image using the Sobel operator.
For more detailed information about the Sobel Operator, please visit: https://en.wikipedia.org/wiki/Sobel_operator

How to use this

Compile by typing 'make' into the terminal screen.
$ make filter


Then, you can run the program like:
$ ./bmpfilter -g in.bmp out.bmp
$ ./bmpfilter -r in.bmp out.bmp
$ ./bmpfilter -b in.bmp out.bmp
$ ./bmpfilter -e in.bmp out.bmp

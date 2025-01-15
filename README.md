# BMP-Filter-Tool

**bmpfilter** is a command-line tool for applying various filters to BMP images such as **Grayscale**, **Reflection**, **Blur**, and **Edge Detection**.

## Available filters:

- **Grayscale** (-g): Converts the image to black and white
- **Reflection** (-r): Creates a mirror reflection of the image
- **Blur** (-b): Blurs the image using a [box-blur](https://en.wikipedia.org/wiki/Box_blur).
- **Edges** (-e): Enhances the edges in the image using the [Sobel operator](https://en.wikipedia.org/wiki/Sobel_operator) and watch [this](https://www.youtube.com/watch?v=VL8PuOPjVjY&t=173s) for better understanding.

## Usage:

**./bmpfilter \<flag\> \<input file\> \<output file\>**

- Compile the source code to build the bmpfilter program:
   ```sh
   make
   ```

- To remove object files and intermediate build artifacts:
   ```sh
   make clean
   ```

- To remove all build artifacts, including the binary:
   ```sh
   make fclean
   ```

## Example Usage for Edge Detection:

![Default](bmw-wheel.bmp)

```sh
./bmpfilter -e bmw-wheel.bmp out.bmp
```

### After:

![Edges](out.bmp)

## License

This project is open source and distributed under the GNU General Public License version 3.0 (GPL-3.0).

[![License: GPL-3.0](https://img.shields.io/badge/License-GPL%203.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

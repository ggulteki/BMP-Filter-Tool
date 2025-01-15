/*
 * filters.h
 * Image filtering functions and bitmap structure definitions
 */

#ifndef FILTERS_H
#define FILTERS_H

#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* Bitmap file constants */
#define BITMAP_HEADER_SIZE    54
#define BITMAP_TYPE          0x4d42
#define BITMAP_COMPRESSION   0

/* Type definitions for bitmap structures */
typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;

/*
 * Bitmap file header structure
 * Contains information about the type, size, and layout of a bitmap file
 */
typedef struct {
    WORD   bfType;           /* Bitmap type identifier */
    DWORD  bfSize;           /* Size of the bitmap file */
    WORD   bfReserved1;      /* Reserved field */
    WORD   bfReserved2;      /* Reserved field */
    DWORD  bfOffBits;        /* Offset to bitmap data */
} __attribute__((__packed__)) BITMAPFILEHEADER;

/*
 * Bitmap information header structure
 * Contains information about the dimensions and format of the bitmap image
 */
typedef struct {
    DWORD  biSize;           /* Size of this header */
    LONG   biWidth;          /* Image width in pixels */
    LONG   biHeight;         /* Image height in pixels */
    WORD   biPlanes;         /* Number of color planes */
    WORD   biBitCount;       /* Bits per pixel */
    DWORD  biCompression;    /* Compression type */
    DWORD  biSizeImage;      /* Size of image data */
    LONG   biXPelsPerMeter;  /* Horizontal resolution */
    LONG   biYPelsPerMeter;  /* Vertical resolution */
    DWORD  biClrUsed;        /* Number of colors used */
    DWORD  biClrImportant;   /* Number of important colors */
} __attribute__((__packed__)) BITMAPINFOHEADER;

/*
 * RGB triple structure
 * Represents a pixel's color values in BGR order
 */
typedef struct {
    BYTE  rgbtBlue;         /* Blue component */
    BYTE  rgbtGreen;        /* Green component */
    BYTE  rgbtRed;          /* Red component */
} __attribute__((__packed__)) RGBTRIPLE;

/* Function prototypes */
void grayscale(int height, int width, RGBTRIPLE image[height][width]);
void reflect(int height, int width, RGBTRIPLE image[height][width]);
void edges(int height, int width, RGBTRIPLE image[height][width]);
void blur(int height, int width, RGBTRIPLE image[height][width]);

#endif /* FILTERS_H */

/*
 * File: main.c
 * Author: ggulteki
 * Date: 2024-12-16 14:29:58
 * Description: Main program for BMP image filtering
 *
 * This program applies various filters (blur, edge detection, grayscale, reflection)
 * to BMP image files. It handles command-line arguments, file I/O, and image processing.
 */

#include "filters.h"

/* Command Line Argument Constants */
#define REQUIRED_ARGS 4        /* Number of required command line arguments */
#define FLAG_INDEX 1           /* Index of the filter flag argument */
#define INPUT_FILE_INDEX 2     /* Index of the input file argument */
#define OUTPUT_FILE_INDEX 3    /* Index of the output file argument */

/* Error Codes */
#define SUCCESS 0             /* Operation completed successfully */
#define ERR_ARGS 1           /* Invalid command line arguments */
#define ERR_OUTPUT_FILE 2    /* Failed to create output file */
#define ERR_HEADER_READ 3    /* Failed to read BMP headers */
#define ERR_FORMAT 4         /* Unsupported BMP format */
#define ERR_MEMORY 5         /* Memory allocation failure */
#define ERR_IMAGE_READ 6     /* Failed to read image data */
#define ERR_SEEK 7          /* Failed to seek within file */
#define ERR_WRITE_HEADER 8  /* Failed to write BMP header */
#define ERR_WRITE_INFO 9    /* Failed to write BMP info */
#define ERR_WRITE_DATA 10   /* Failed to write image data */
#define ERR_WRITE_PADDING 11 /* Failed to write padding bytes */

/*
 * Validates command line arguments
 *
 * Parameters:
 *   argc - Number of arguments
 *   argv - Array of argument strings
 *
 * Returns:
 *   Error code (0 if valid, ERR_ARGS if invalid)
 *
 * Description:
 *   Checks if the correct number of arguments is provided and if the filter flag is valid
 */
static int validate_args(int argc, char *argv[]) {
    if (argc != REQUIRED_ARGS) {
        printf("Usage: ./program <flag> <input file> <output file>\n");
        return ERR_ARGS;
    }

    const char flag = argv[FLAG_INDEX][1];
    if (flag != 'b' && flag != 'e' && flag != 'g' && flag != 'r') {
        printf("Invalid flag\n");
        return ERR_ARGS;
    }

    return SUCCESS;
}

/*
 * Opens and validates input/output files
 *
 * Parameters:
 *   infile  - Input filename
 *   outfile - Output filename
 *   inptr   - Pointer to input file handle
 *   outptr  - Pointer to output file handle
 *
 * Returns:
 *   Error code (SUCCESS if successful, ERR_ARGS or ERR_OUTPUT_FILE on failure)
 *
 * Description:
 *   Opens the input file in binary read mode and output file in binary write mode
 */
static int open_files(const char *infile, const char *outfile, FILE **inptr, FILE **outptr) {
    *inptr = fopen(infile, "rb");
    if (*inptr == NULL) {
        printf("Could not open %s.\n", infile);
        return ERR_ARGS;
    }

    *outptr = fopen(outfile, "wb");
    if (*outptr == NULL) {
        fclose(*inptr);
        printf("Could not create %s.\n", outfile);
        return ERR_OUTPUT_FILE;
    }

    return SUCCESS;
}

/*
 * Processes the image with the specified filter
 *
 * Parameters:
 *   flag   - Filter type flag ('b' for blur, 'e' for edges, 'g' for grayscale, 'r' for reflect)
 *   height - Image height in pixels
 *   width  - Image width in pixels
 *   image  - Image data array to be processed
 *
 * Description:
 *   Applies the selected filter transformation to the image data
 */
static void process_image(char flag, int height, int width, RGBTRIPLE image[height][width]) {
    switch (flag) {
        case 'b':
            blur(height, width, image);
            break;
        case 'e':
            edges(height, width, image);
            break;
        case 'g':
            grayscale(height, width, image);
            break;
        case 'r':
            reflect(height, width, image);
            break;
    }
}

/*
 * Writes the processed image to output file
 *
 * Parameters:
 *   outptr - Output file pointer
 *   bf     - Bitmap file header
 *   bi     - Bitmap info header
 *   height - Image height in pixels
 *   width  - Image width in pixels
 *   image  - Processed image data to write
 *
 * Returns:
 *   Error code (SUCCESS if successful, various ERR codes on failure)
 *
 * Description:
 *   Writes the BMP headers and image data with appropriate padding
 */
static int write_image(FILE *outptr, BITMAPFILEHEADER *bf, BITMAPINFOHEADER *bi,
                      int height, int width, RGBTRIPLE image[height][width]) {
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    if (fwrite(bf, sizeof(BITMAPFILEHEADER), 1, outptr) != 1) {
        return ERR_WRITE_HEADER;
    }

    if (fwrite(bi, sizeof(BITMAPINFOHEADER), 1, outptr) != 1) {
        return ERR_WRITE_INFO;
    }

    for (int i = 0; i < height; i++) {
        if (fwrite(image[i], sizeof(RGBTRIPLE), width, outptr) != width) {
            return ERR_WRITE_DATA;
        }

        for (int k = 0; k < padding; k++) {
            if (fputc(0x00, outptr) == EOF) {
                return ERR_WRITE_PADDING;
            }
        }
    }

    return SUCCESS;
}

/*
 * Main program entry point
 *
 * Parameters:
 *   argc - Number of command line arguments
 *   argv - Array of command line argument strings
 *
 * Returns:
 *   Program exit code (SUCCESS on successful execution, error code on failure)
 *
 * Description:
 *   Coordinates the entire image processing workflow:
 *   1. Validates command line arguments
 *   2. Opens input/output files
 *   3. Reads and validates BMP headers
 *   4. Allocates memory and reads image data
 *   5. Applies the selected filter
 *   6. Writes the processed image
 *   7. Cleans up resources
 */
int main(int argc, char *argv[]) {
    int result = validate_args(argc, argv);
    if (result != SUCCESS) {
        return result;
    }

    const char flag = argv[FLAG_INDEX][1];
    const char *infile = argv[INPUT_FILE_INDEX];
    const char *outfile = argv[OUTPUT_FILE_INDEX];

    FILE *inptr, *outptr;
    result = open_files(infile, outfile, &inptr, &outptr);
    if (result != SUCCESS) {
        return result;
    }

    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;
    if (fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr) != 1 ||
        fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr) != 1) {
        fclose(outptr);
        fclose(inptr);
        printf("Error reading BMP headers.\n");
        return ERR_HEADER_READ;
    }

    if (bf.bfType != BITMAP_TYPE || bf.bfOffBits != BITMAP_HEADER_SIZE ||
        bi.biSize != 40 || bi.biBitCount != 24 || bi.biCompression != BITMAP_COMPRESSION) {
        fclose(outptr);
        fclose(inptr);
        printf("Unsupported file format.\n");
        return ERR_FORMAT;
    }

    int height = abs(bi.biHeight);
    int width = bi.biWidth;
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    RGBTRIPLE(*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL) {
        fclose(outptr);
        fclose(inptr);
        printf("Not enough memory to store image.\n");
        return ERR_MEMORY;
    }

    for (int i = 0; i < height; i++) {
        if (fread(image[i], sizeof(RGBTRIPLE), width, inptr) != width) {
            free(image);
            fclose(outptr);
            fclose(inptr);
            printf("Error reading image data.\n");
            return ERR_IMAGE_READ;
        }

        if (fseek(inptr, padding, SEEK_CUR) == -1) {
            free(image);
            fclose(outptr);
            fclose(inptr);
            printf("Error seeking to next row.\n");
            return ERR_SEEK;
        }
    }

    process_image(flag, height, width, image);

    result = write_image(outptr, &bf, &bi, height, width, image);
    if (result != SUCCESS) {
        free(image);
        fclose(outptr);
        fclose(inptr);
        printf("Error writing output file.\n");
        return result;
    }

    free(image);
    image = NULL;
    fclose(inptr);
    fclose(outptr);

    return SUCCESS;
}

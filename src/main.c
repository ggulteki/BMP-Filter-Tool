/*
 * @Author: ggulteki 
 * @Date: 2024-12-16 14:29:58 
 * @Last Modified by: ggulteki
 * @Last Modified time: 2024-12-16 15:02:47
 */

#include "filters.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: ./program <flag> <input file> <output file>\n");
        return 1;
    }

    const char flag = argv[1][1];
    const char *infile = argv[2];
    const char *outfile = argv[3];
    
    if (flag != 'b' && flag != 'e' && flag != 'g' && flag != 'r') {
        printf("Invalid flag\n");
        return 1;
    }

    FILE *inptr = fopen(infile, "rb");
    if (inptr == NULL) {
        printf("Could not open %s.\n", infile);
        return 1;
    }

    FILE *outptr = fopen(outfile, "wb");
    if (outptr == NULL) {
        fclose(inptr);
        printf("Could not create %s.\n", outfile);
        return 2;
    }

    BITMAPFILEHEADER bf;
    if (fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr) != 1) {
        fclose(outptr);
        fclose(inptr);
        printf("Error reading BMP file header.\n");
        return 3;
    }
    
    BITMAPINFOHEADER bi;
    if (fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr) != 1) {
        fclose(outptr);
        fclose(inptr);
        printf("Error reading BMP info header.\n");
        return 3;
    }

    if (bf.bfType != BITMAP_TYPE || bf.bfOffBits != BITMAP_HEADER_SIZE ||
        bi.biSize != 40 || bi.biBitCount != 24 || bi.biCompression != BITMAP_COMPRESSION) {
        fclose(outptr);
        fclose(inptr);
        printf("Unsupported file format.\n");
        return 4;
    }

    int height = abs(bi.biHeight);
    int width = bi.biWidth;

    RGBTRIPLE(*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL) {
        printf("Not enough memory to store image.\n");
        fclose(outptr);
        fclose(inptr);
        return 5;
    }

    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    for (int i = 0; i < height; i++) {
        if (fread(image[i], sizeof(RGBTRIPLE), width, inptr) != width) {
            free(image);
            fclose(outptr);
            fclose(inptr);
            printf("Error reading image data.\n");
            return 6;
        }
        
        if (fseek(inptr, padding, SEEK_CUR) == -1) {
            free(image);
            fclose(outptr);
            fclose(inptr);
            printf("Error seeking to next row (fseek).\n");
            return 7;
        }
    }
    
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

    if (fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr) != 1) {
        free(image);
        fclose(outptr);
        fclose(inptr);
        printf("Error writing BMP file header.\n");
        return 8;
    }
    
    if (fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr) != 1) {
        free(image);
        fclose(outptr);
        fclose(inptr);
        printf("Error writing BMP info header.\n");
        return 9;
    }

    for (int i = 0; i < height; i++) {
        if (fwrite(image[i], sizeof(RGBTRIPLE), width, outptr) != width) {
            free(image);
            fclose(outptr);
            fclose(inptr);
            printf("Error writing image data (fwrite).\n");
            return 10;
        }

        for (int k = 0; k < padding; k++) {
            if (fputc(0x00, outptr) == EOF) {
                free(image);
                fclose(outptr);
                fclose(inptr);
                printf("Error writing padding byte.\n");
                return 11;
            }
        }
    }

    free(image);
    fclose(inptr);
    fclose(outptr);
    
    return 0;
}

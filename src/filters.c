/*
 * filters.c
 * Author: Gokberk Gultekin
 * Date: December 16, 2024
 * Implementation of the functions to apply filters to BMP files.
 */

#include "filters.h"

/* Constants for image processing */
#define AVG_DIVISOR 3.0f     /* Floating point divisor for grayscale average calculation */
#define MAX_RGB_VALUE 255    /* Maximum value for RGB components */

/*
 * Converts an image to grayscale by averaging RGB values.
 *
 * For each pixel, takes the average of the red, green, and blue values
 * to determine the grayscale value that is then applied to all three
 * color channels.
 *
 * Parameters:
 *   height - Number of rows in the image
 *   width - Number of columns in the image
 *   image - The 2D array of RGBTRIPLE pixels to be modified
 */
void grayscale(int height, int width, RGBTRIPLE image[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float avg = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / AVG_DIVISOR;
            uint8_t gray_value = round(avg);

            image[i][j].rgbtBlue = gray_value;
            image[i][j].rgbtGreen = gray_value;
            image[i][j].rgbtRed = gray_value;
        }
    }
}

/*
 * Reflects the image horizontally.
 *
 * Swaps pixels on horizontal axis, from left to right, effectively
 * creating a mirror image. Only processes up to the middle column
 * to avoid double-swapping.
 *
 * Parameters:
 *   height - Number of rows in the image
 *   width - Number of columns in the image
 *   image - The 2D array of RGBTRIPLE pixels to be modified
 */
void reflect(int height, int width, RGBTRIPLE image[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int left = 0, right = width - 1; left < right; left++, right--) {
            RGBTRIPLE temp = image[i][left];
            image[i][left] = image[i][right];
            image[i][right] = temp;
        }
    }
}

/*
 * Calculates the box blur value for a specific pixel.
 *
 * Computes the average RGB values of all neighboring pixels (including the pixel itself)
 * within a 3x3 grid. Handles edge cases where the grid extends beyond image boundaries.
 *
 * Parameters:
 *   i - Row index of the target pixel
 *   j - Column index of the target pixel
 *   height - Image height
 *   width - Image width
 *   image - Source image array
 *
 * Returns:
 *   RGBTRIPLE - Blurred pixel value
 */
static RGBTRIPLE box_blur(int i, int j, int height, int width, RGBTRIPLE image[height][width]) {
    unsigned int sum_red = 0, sum_green = 0, sum_blue = 0;
    unsigned int count = 0;

    // Iterate over 3x3 grid centered on pixel
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            int ni = i + di;  // Neighbor row
            int nj = j + dj;  // Neighbor column

            // Skip pixels outside image boundaries
            if (ni >= 0 && ni < height && nj >= 0 && nj < width) {
                sum_red += image[ni][nj].rgbtRed;
                sum_green += image[ni][nj].rgbtGreen;
                sum_blue += image[ni][nj].rgbtBlue;
                count++;
            }
        }
    }

    RGBTRIPLE blurred;
    blurred.rgbtRed = round((float)sum_red / count);
    blurred.rgbtGreen = round((float)sum_green / count);
    blurred.rgbtBlue = round((float)sum_blue / count);

    return blurred;
}

/*
 * Applies a box blur filter to the entire image.
 *
 * Creates a blurred version of the image by replacing each pixel with the average
 * color values of its neighboring pixels. Uses a temporary buffer to avoid
 * contaminating the blur calculations with already blurred pixels.
 *
 * Parameters:
 *   height - Number of rows in the image
 *   width - Number of columns in the image
 *   image - The 2D array of RGBTRIPLE pixels to be modified
 */
void blur(int height, int width, RGBTRIPLE image[height][width]) {
    // Allocate temporary buffer for blur calculations
    RGBTRIPLE(*temp)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed for blur buffer.\n");
        exit(1);
    }

    // Calculate blur for each pixel
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            temp[i][j] = box_blur(i, j, height, width, image);
    }

    // Copy blurred image back to original buffer
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            image[i][j] = temp[i][j];
    }

    // Clean up
    free(temp);
    temp = NULL;
}

/*
 * Calculates the Sobel operator value for a specific pixel.
 *
 * Computes the gradient magnitude using Sobel operators in both x and y directions.
 *
 * Parameters:
 *   i - Row index of the target pixel
 *   j - Column index of the target pixel
 *   height - Image height
 *   width - Image width
 *   image - Source image array
 *
 * Returns:
 *   RGBTRIPLE - Edge-detected pixel value
 */
static RGBTRIPLE sobel_calc(int i, int j, int height, int width, RGBTRIPLE image[height][width]) {
    // Sobel kernels for x and y gradients
    const int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    const int Gy[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    int gx = 0;
    int gy = 0;

    // Calculate Gx and Gy
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            int ni = i + di;
            int nj = j + dj;

            // Use 0 for pixels outside the image boundary
            int pixel_value = 0;
            if (ni >= 0 && ni < height && nj >= 0 && nj < width) {
                // Since image is already transformed to luminance, we can use any channel
                pixel_value = image[ni][nj].rgbtRed;
            }

            // Add weighted pixel values
            gx += pixel_value * Gx[di + 1][dj + 1];
            gy += pixel_value * Gy[di + 1][dj + 1];
        }
    }

    // Calculate the magnitude of the gradient
    int magnitude = round(sqrt((float)(gx * gx + gy * gy)));

    // Cap the value at 255
    //magnitude = magnitude > 255 ? 255 : magnitude;

    // Create the result pixel (same value for all channels since we're in grayscale)
    RGBTRIPLE result;
    result.rgbtRed = result.rgbtGreen = result.rgbtBlue = magnitude;

    return result;
}

/*
 * Converts an RGB image to luminance values.
 *
 * Transforms RGB values to luminance using standard coefficients:
 * L = 0.299R + 0.587G + 0.114B
 *
 * Parameters:
 *   height - Number of rows in the image
 *   width - Number of columns in the image
 *   image - The 2D array of RGBTRIPLE pixels to be modified
 */
void luminance(int height, int width, RGBTRIPLE image[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float luminance = 0.299f * image[i][j].rgbtRed +
                            0.587f * image[i][j].rgbtGreen +
                            0.114f * image[i][j].rgbtBlue;
            uint8_t lum_value = round(luminance);

            image[i][j].rgbtRed = lum_value;
            image[i][j].rgbtGreen = lum_value;
            image[i][j].rgbtBlue = lum_value;
        }
    }
}

/*
 * Applies edge detection to the image using the Sobel operator.
 *
 * First converts the image to grayscale and luminance, then applies
 * the Sobel operator to detect edges. Finally applies a blur to
 * smooth the result.
 *
 * Parameters:
 *   height - Number of rows in the image
 *   width - Number of columns in the image
 *   image - The 2D array of RGBTRIPLE pixels to be modified
 */
void edges(int height, int width, RGBTRIPLE image[height][width]) {
    grayscale(height, width, image);
    luminance(height, width, image);
    // Allocate temporary buffer for sobel calculation
    RGBTRIPLE(*temp)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed for temp buffer. \n");
        exit(1);
    }

    // Calculate sobel operator for each pixel
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            temp[i][j] = sobel_calc(i, j, height, width, image);
    }

    // Copy modified image back to original buffer
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (temp[i][j].rgbtRed > image[i][j].rgbtRed)
                image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = 255;
            else
                image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = 0;
        }
    }
    blur(height, width, image);

    // Clean up
    free(temp);
    temp = NULL;
}

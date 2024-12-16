/*
 * Filename: filters.c
 * Author: Gokberk Gultekin
 * Date: December 16, 2024
 * Description: The function's implementation for applying filters to BMP.
 */

#include "filters.h"

#define AVG_DIVISOR 3.0  // Constant for grayscale average calculation

/*
 * Common Parameters for Image Processing Functions:
 * - height: Number of rows in the image.
 * - width: Number of columns in the image.
 * - image: A 2D array of RGBTRIPLE structures representing the image pixels.
 *
 * Each function processes the image based on these parameters, modifying
 * the image in-place unless otherwise specified.
 */

/*
 * Converts an image to grayscale by averaging the RGB values of each pixel.
 * Modifies the image in-place.
 */
void grayscale(int height, int width, RGBTRIPLE image[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int a = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / AVG_DIVISOR);
            image[i][j].rgbtBlue = a;
            image[i][j].rgbtGreen = a;
            image[i][j].rgbtRed = a;
        }
    }
}

/*
 * Reflects the image horizontally by swapping pixels along the middle column.
 * Modifies the image in-place.
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
 * There are several ways to create the effect of blurring or softening an image.
 * For this problem, we'll use the "box blur," which works by averaging the color values
 * of neighboring pixels and assigning the new average value to each pixel for each color.
 */

// Helper function to calculate the blurred value for a specific pixel
RGBTRIPLE calculate_blur(int i, int j, int height, int width, RGBTRIPLE image[height][width]) {
    int sum_red = 0, sum_green = 0, sum_blue = 0, count = 0;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            int nx = i + x; // Neighbor row index
            int ny = j + y; // Neighbor column index
            // Check bounds to avoid accessing invalid pixels
            if (nx >= 0 && nx < height && ny >= 0 && ny < width) {
                sum_red += image[nx][ny].rgbtRed;
                sum_green += image[nx][ny].rgbtGreen;
                sum_blue += image[nx][ny].rgbtBlue;
                count++;
            }
        }
    }
    RGBTRIPLE blurred_pixel;
    blurred_pixel.rgbtRed = round((float)sum_red / (float)count);
    blurred_pixel.rgbtGreen = round((float)sum_green / (float)count);
    blurred_pixel.rgbtBlue = round((float)sum_blue / (float)count);

    return blurred_pixel;
}

// Main blur function
void blur(int height, int width, RGBTRIPLE image[height][width]) {
    RGBTRIPLE(*temp)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed for temp.\n");
        exit(1);
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            temp[i][j] = calculate_blur(i, j, height, width, image);
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j] = temp[i][j];
        }
    }

    free(temp);
    temp = NULL;
}

/*
 * In artificial intelligence algorithms for image processing, detecting edges in an image is often useful. These edges are
 * lines that create boundaries between objects. One way to achieve this effect is by applying the Sobel operator to the image.
 * Like image blurring, edge detection works by analyzing each pixel and modifying it based on the 3x3 grid of surrounding pixels.
 * However, instead of simply averaging the nine surrounding pixels, the Sobel operator computes the new value of each pixel
 * by taking a weighted sum of the surrounding pixels' values.
 */
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*temp)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (temp == NULL)
    {
        exit(1);
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0)
            {
                if (j == 0)
                {
                    int a = (image[i][j].rgbtRed * 0) + (image[i][j + 1].rgbtRed * 2) + (image[i + 1][j].rgbtRed * 0) +
                            (image[i + 1][j + 1].rgbtRed * 1);
                    int b = (image[i][j].rgbtGreen * 0) + (image[i][j + 1].rgbtGreen * 2) + (image[i + 1][j].rgbtGreen * 0) +
                            (image[i + 1][j + 1].rgbtGreen * 1);
                    int c = (image[i][j].rgbtBlue * 0) + (image[i][j + 1].rgbtBlue * 2) + (image[i + 1][j].rgbtBlue * 0) +
                            (image[i + 1][j + 1].rgbtBlue * 1);
                    int d = (image[i][j].rgbtRed * 0) + (image[i][j + 1].rgbtRed * 0) + (image[i + 1][j].rgbtRed * 2) +
                            (image[i + 1][j + 1].rgbtRed * 1);
                    int e = (image[i][j].rgbtGreen * 0) + (image[i][j + 1].rgbtGreen * 0) + (image[i + 1][j].rgbtGreen * 2) +
                            (image[i + 1][j + 1].rgbtGreen * 1);
                    int f = (image[i][j].rgbtBlue * 0) + (image[i][j + 1].rgbtBlue * 0) + (image[i + 1][j].rgbtBlue * 2) +
                            (image[i + 1][j + 1].rgbtBlue * 1);
                    int Gxred = a * a;
                    int Gxgreen = b * b;
                    int Gxblue = c * c;

                    int Gyred = d * d;
                    int Gygreen = e * e;
                    int Gyblue = f * f;

                    temp[i][j].rgbtBlue = (round(sqrt(Gxblue + Gyblue)) > 255) ? 255 : round(sqrt(Gxblue + Gyblue));
                    temp[i][j].rgbtGreen = (round(sqrt(Gxgreen + Gygreen)) > 255) ? 255 : round(sqrt(Gxgreen + Gygreen));
                    temp[i][j].rgbtRed = (round(sqrt(Gxred + Gyred)) > 255) ? 255 : round(sqrt(Gxred + Gyred));
                }
                if ((0 < j) && (j < (width - 1)))
                {
                    int a = (image[i][j - 1].rgbtRed * -2) + (image[i][j].rgbtRed * 0) + (image[i][j + 1].rgbtRed * 2) +
                            (image[i + 1][j - 1].rgbtRed * -1) + (image[i + 1][j].rgbtRed * 0) + (image[i + 1][j + 1].rgbtRed * 1);
                    int b = (image[i][j - 1].rgbtGreen * -2) + (image[i][j].rgbtGreen * 0) + (image[i][j + 1].rgbtGreen * 2) +
                            (image[i + 1][j - 1].rgbtGreen * -1) + (image[i + 1][j].rgbtGreen * 0) +
                            (image[i + 1][j + 1].rgbtGreen * 1);
                    int c = (image[i][j - 1].rgbtBlue * -2) + (image[i][j].rgbtBlue * 0) + (image[i][j + 1].rgbtBlue * 2) +
                            (image[i + 1][j - 1].rgbtBlue * -1) + (image[i + 1][j].rgbtBlue * 0) +
                            (image[i + 1][j + 1].rgbtBlue * 1);
                    
                    int d = (image[i][j - 1].rgbtRed * 0) + (image[i][j].rgbtRed * 0) + (image[i][j + 1].rgbtRed * 0) +
                            (image[i + 1][j - 1].rgbtRed * 1) + (image[i + 1][j].rgbtRed * 2) + (image[i + 1][j + 1].rgbtRed * 1);
                    int e = (image[i][j - 1].rgbtGreen * 0) + (image[i][j].rgbtGreen * 0) + (image[i][j + 1].rgbtGreen * 0) +
                            (image[i + 1][j - 1].rgbtGreen * 1) + (image[i + 1][j].rgbtGreen * 2) +
                            (image[i + 1][j + 1].rgbtGreen * 1);
                    int f = (image[i][j - 1].rgbtBlue * 0) + (image[i][j].rgbtBlue * 0) + (image[i][j + 1].rgbtBlue * 0) +
                            (image[i + 1][j - 1].rgbtBlue * 1) + (image[i + 1][j].rgbtBlue * 2) +
                            (image[i + 1][j + 1].rgbtBlue * 1);
                    
                    int Gxred = a * a;
                    int Gxgreen = b * b;
                    int Gxblue = c * c;
                    
                    int Gyred = d * d;
                    int Gygreen = e * e;
                    int Gyblue = f * f;

                    temp[i][j].rgbtBlue = (round(sqrt(Gxblue + Gyblue)) > 255) ? 255 : round(sqrt(Gxblue + Gyblue));
                    temp[i][j].rgbtGreen = (round(sqrt(Gxgreen + Gygreen)) > 255) ? 255 : round(sqrt(Gxgreen + Gygreen));
                    temp[i][j].rgbtRed = (round(sqrt(Gxred + Gyred)) > 255) ? 255 : round(sqrt(Gxred + Gyred));
                }
                if (j == (width - 1))
                {
                    int a = (image[i][j - 1].rgbtRed * -2) + (image[i][j].rgbtRed * 0) + (image[i + 1][j - 1].rgbtRed * -1) +
                            (image[i + 1][j].rgbtRed * 0);
                    int b = (image[i][j - 1].rgbtGreen * -2) + (image[i][j].rgbtGreen * 0) + (image[i + 1][j - 1].rgbtGreen * -1) +
                            (image[i + 1][j].rgbtGreen * 0);
                    int c = (image[i][j - 1].rgbtBlue * -2) + (image[i][j].rgbtBlue * 0) + (image[i + 1][j - 1].rgbtBlue * -1) +
                            (image[i + 1][j].rgbtBlue * 0);
                   
                    int d = (image[i][j - 1].rgbtRed * 0) + (image[i][j].rgbtRed * 0) + (image[i + 1][j - 1].rgbtRed * 1) +
                            (image[i + 1][j].rgbtRed * 2);
                    int e = (image[i][j - 1].rgbtGreen * 0) + (image[i][j].rgbtGreen * 0) + (image[i + 1][j - 1].rgbtGreen * 1) +
                            (image[i + 1][j].rgbtGreen * 2);
                    int f = (image[i][j - 1].rgbtBlue * 0) + (image[i][j].rgbtBlue * 0) + (image[i + 1][j - 1].rgbtBlue * 1) +
                            (image[i + 1][j].rgbtBlue * 2);
                    
                    int Gxred = a * a;
                    int Gxgreen = b * b;
                    int Gxblue = c * c;
                    
                    int Gyred = d * d;
                    int Gygreen = e * e;
                    int Gyblue = f * f;

                    temp[i][j].rgbtBlue = (round(sqrt(Gxblue + Gyblue)) > 255) ? 255 : round(sqrt(Gxblue + Gyblue));
                    temp[i][j].rgbtGreen = (round(sqrt(Gxgreen + Gygreen)) > 255) ? 255 : round(sqrt(Gxgreen + Gygreen));
                    temp[i][j].rgbtRed = (round(sqrt(Gxred + Gyred)) > 255) ? 255 : round(sqrt(Gxred + Gyred));
                }
            }
            if ((0 < i) && (i < (height - 1)))
            {
                if (j == 0)
                {
                    
                    int a = (image[i - 1][j].rgbtRed * 0) + (image[i - 1][j + 1].rgbtRed * 1) + (image[i][j].rgbtRed * 0) +
                            (image[i][j + 1].rgbtRed * 2) + (image[i + 1][j].rgbtRed * 0) + (image[i + 1][j + 1].rgbtRed * 1);
                    int b = (image[i - 1][j].rgbtGreen * 0) + (image[i - 1][j + 1].rgbtGreen * 1) + (image[i][j].rgbtGreen * 0) +
                            (image[i][j + 1].rgbtGreen * 2) + (image[i + 1][j].rgbtGreen * 0) + (image[i + 1][j + 1].rgbtGreen * 1);
                    int c = (image[i - 1][j].rgbtBlue * 0) + (image[i - 1][j + 1].rgbtBlue * 1) + (image[i][j].rgbtBlue * 0) +
                            (image[i][j + 1].rgbtBlue * 2) + (image[i + 1][j].rgbtBlue * 0) + (image[i + 1][j + 1].rgbtBlue * 1);
                    
                    int d = (image[i - 1][j].rgbtRed * -2) + (image[i - 1][j + 1].rgbtRed * -1) + (image[i][j].rgbtRed * 0) +
                            (image[i][j + 1].rgbtRed * 0) + (image[i + 1][j].rgbtRed * 2) + (image[i + 1][j + 1].rgbtRed * 1);
                    int e = (image[i - 1][j].rgbtGreen * -2) + (image[i - 1][j + 1].rgbtGreen * -1) + (image[i][j].rgbtGreen * 0) +
                            (image[i][j + 1].rgbtGreen * 0) + (image[i + 1][j].rgbtGreen * 2) + (image[i + 1][j + 1].rgbtGreen * 1);
                    int f = (image[i - 1][j].rgbtBlue * -2) + (image[i - 1][j + 1].rgbtBlue * -1) + (image[i][j].rgbtBlue * 0) +
                            (image[i][j + 1].rgbtBlue * 0) + (image[i + 1][j].rgbtBlue * 2) + (image[i + 1][j + 1].rgbtBlue * 1);
                    
                    int Gxred = a * a;
                    int Gxgreen = b * b;
                    int Gxblue = c * c;
                    
                    int Gyred = d * d;
                    int Gygreen = e * e;
                    int Gyblue = f * f;

                    temp[i][j].rgbtBlue = (round(sqrt(Gxblue + Gyblue)) > 255) ? 255 : round(sqrt(Gxblue + Gyblue));
                    temp[i][j].rgbtGreen = (round(sqrt(Gxgreen + Gygreen)) > 255) ? 255 : round(sqrt(Gxgreen + Gygreen));
                    temp[i][j].rgbtRed = (round(sqrt(Gxred + Gyred)) > 255) ? 255 : round(sqrt(Gxred + Gyred));
                }
                if ((0 < j) && (j < (width - 1)))
                {
                    int a = (image[i - 1][j - 1].rgbtRed * -1) + (image[i - 1][j].rgbtRed * 0) + (image[i - 1][j + 1].rgbtRed * 1) +
                            (image[i][j - 1].rgbtRed * -2) + (image[i][j].rgbtRed * 0) + (image[i][j + 1].rgbtRed * 2) +
                            (image[i + 1][j - 1].rgbtRed * -1) + (image[i + 1][j].rgbtRed * 0) + (image[i + 1][j + 1].rgbtRed * 1);
                    int b = (image[i - 1][j - 1].rgbtGreen * -1) + (image[i - 1][j].rgbtGreen * 0) +
                            (image[i - 1][j + 1].rgbtGreen * 1) + (image[i][j - 1].rgbtGreen * -2) + (image[i][j].rgbtGreen * 0) +
                            (image[i][j + 1].rgbtGreen * 2) + (image[i + 1][j - 1].rgbtGreen * -1) +
                            (image[i + 1][j].rgbtGreen * 0) + (image[i + 1][j + 1].rgbtGreen * 1);
                    int c = (image[i - 1][j - 1].rgbtBlue * -1) + (image[i - 1][j].rgbtBlue * 0) +
                            (image[i - 1][j + 1].rgbtBlue * 1) + (image[i][j - 1].rgbtBlue * -2) + (image[i][j].rgbtBlue * 0) +
                            (image[i][j + 1].rgbtBlue * 2) + (image[i + 1][j - 1].rgbtBlue * -1) + (image[i + 1][j].rgbtBlue * 0) +
                            (image[i + 1][j + 1].rgbtBlue * 1);
                    
                    int d = (image[i - 1][j - 1].rgbtRed * -1) + (image[i - 1][j].rgbtRed * -2) +
                            (image[i - 1][j + 1].rgbtRed * -1) + (image[i][j - 1].rgbtRed * 0) + (image[i][j].rgbtRed * 0) +
                            (image[i][j + 1].rgbtRed * 0) + (image[i + 1][j - 1].rgbtRed * 1) + (image[i + 1][j].rgbtRed * 2) +
                            (image[i + 1][j + 1].rgbtRed * 1);
                    int e = (image[i - 1][j - 1].rgbtGreen * -1) + (image[i - 1][j].rgbtGreen * -2) +
                            (image[i - 1][j + 1].rgbtGreen * -1) + (image[i][j - 1].rgbtGreen * 0) + (image[i][j].rgbtGreen * 0) +
                            (image[i][j + 1].rgbtGreen * 0) + (image[i + 1][j - 1].rgbtGreen * 1) +
                            (image[i + 1][j].rgbtGreen * 2) + (image[i + 1][j + 1].rgbtGreen * 1);
                    int f = (image[i - 1][j - 1].rgbtBlue * -1) + (image[i - 1][j].rgbtBlue * -2) +
                            (image[i - 1][j + 1].rgbtBlue * -1) + (image[i][j - 1].rgbtBlue * 0) + (image[i][j].rgbtBlue * 0) +
                            (image[i][j + 1].rgbtBlue * 0) + (image[i + 1][j - 1].rgbtBlue * 1) + (image[i + 1][j].rgbtBlue * 2) +
                            (image[i + 1][j + 1].rgbtBlue * 1);
                    
                    int Gxred = a * a;
                    int Gxgreen = b * b;
                    int Gxblue = c * c;
                    
                    int Gyred = d * d;
                    int Gygreen = e * e;
                    int Gyblue = f * f;

                    temp[i][j].rgbtBlue = (round(sqrt(Gxblue + Gyblue)) > 255) ? 255 : round(sqrt(Gxblue + Gyblue));
                    temp[i][j].rgbtGreen = (round(sqrt(Gxgreen + Gygreen)) > 255) ? 255 : round(sqrt(Gxgreen + Gygreen));
                    temp[i][j].rgbtRed = (round(sqrt(Gxred + Gyred)) > 255) ? 255 : round(sqrt(Gxred + Gyred));
                }
                if (j == (width - 1))
                {
                    
                    int a = (image[i - 1][j - 1].rgbtRed * -1) + (image[i - 1][j].rgbtRed * 0) + (image[i][j - 1].rgbtRed * -2) +
                            (image[i][j].rgbtRed * 0) + (image[i + 1][j - 1].rgbtRed * -1) + (image[i + 1][j].rgbtRed * 0);
                    int b = (image[i - 1][j - 1].rgbtGreen * -1) + (image[i - 1][j].rgbtGreen * 0) +
                            (image[i][j - 1].rgbtGreen * -2) + (image[i][j].rgbtGreen * 0) + (image[i + 1][j - 1].rgbtGreen * -1) +
                            (image[i + 1][j].rgbtGreen * 0);
                    int c = (image[i - 1][j - 1].rgbtBlue * -1) + (image[i - 1][j].rgbtBlue * 0) + (image[i][j - 1].rgbtBlue * -2) +
                            (image[i][j].rgbtBlue * 0) + (image[i + 1][j - 1].rgbtBlue * -1) + (image[i + 1][j].rgbtBlue * 0);
                    
                    int d = (image[i - 1][j - 1].rgbtRed * -1) + (image[i - 1][j].rgbtRed * -2) + (image[i][j - 1].rgbtRed * 0) +
                            (image[i][j].rgbtRed * 0) + (image[i + 1][j - 1].rgbtRed * 1) + (image[i + 1][j].rgbtRed * 2);
                    int e = (image[i - 1][j - 1].rgbtGreen * -1) + (image[i - 1][j].rgbtGreen * -2) +
                            (image[i][j - 1].rgbtGreen * 0) + (image[i][j].rgbtGreen * 0) + (image[i + 1][j - 1].rgbtGreen * 1) +
                            (image[i + 1][j].rgbtGreen * 2);
                    int f = (image[i - 1][j - 1].rgbtBlue * -1) + (image[i - 1][j].rgbtBlue * -2) + (image[i][j - 1].rgbtBlue * 0) +
                            (image[i][j].rgbtBlue * 0) + (image[i + 1][j - 1].rgbtBlue * 1) + (image[i + 1][j].rgbtBlue * 2);
                    
                    int Gxred = a * a;
                    int Gxgreen = b * b;
                    int Gxblue = c * c;
                    
                    int Gyred = d * d;
                    int Gygreen = e * e;
                    int Gyblue = f * f;

                    temp[i][j].rgbtBlue = (round(sqrt(Gxblue + Gyblue)) > 255) ? 255 : round(sqrt(Gxblue + Gyblue));
                    temp[i][j].rgbtGreen = (round(sqrt(Gxgreen + Gygreen)) > 255) ? 255 : round(sqrt(Gxgreen + Gygreen));
                    temp[i][j].rgbtRed = (round(sqrt(Gxred + Gyred)) > 255) ? 255 : round(sqrt(Gxred + Gyred));
                }
            }
            if (i == (height - 1))
            {
                if (j == 0)
                {
                    
                    int a = (image[i - 1][j].rgbtRed * 0) + (image[i - 1][j + 1].rgbtRed * 1) + (image[i][j].rgbtRed * 0) +
                            (image[i][j + 1].rgbtRed * 2);
                    int b = (image[i - 1][j].rgbtGreen * 0) + (image[i - 1][j + 1].rgbtGreen * 1) + (image[i][j].rgbtGreen * 0) +
                            (image[i][j + 1].rgbtGreen * 2);
                    int c = (image[i - 1][j].rgbtBlue * 0) + (image[i - 1][j + 1].rgbtBlue * 1) + (image[i][j].rgbtBlue * 0) +
                            (image[i][j + 1].rgbtBlue * 2);
                    
                    int d = (image[i - 1][j].rgbtRed * -2) + (image[i - 1][j + 1].rgbtRed * -1) + (image[i][j].rgbtRed * 0) +
                            (image[i][j + 1].rgbtRed * 0);
                    int e = (image[i - 1][j].rgbtGreen * -2) + (image[i - 1][j + 1].rgbtGreen * -1) + (image[i][j].rgbtGreen * 0) +
                            (image[i][j + 1].rgbtGreen * 0);
                    int f = (image[i - 1][j].rgbtBlue * -2) + (image[i - 1][j + 1].rgbtBlue * -1) + (image[i][j].rgbtBlue * 0) +
                            (image[i][j + 1].rgbtBlue * 0);
                   
                    int Gxred = a * a;
                    int Gxgreen = b * b;
                    int Gxblue = c * c;
                    
                    int Gyred = d * d;
                    int Gygreen = e * e;
                    int Gyblue = f * f;

                    temp[i][j].rgbtBlue = (round(sqrt(Gxblue + Gyblue)) > 255) ? 255 : round(sqrt(Gxblue + Gyblue));
                    temp[i][j].rgbtGreen = (round(sqrt(Gxgreen + Gygreen)) > 255) ? 255 : round(sqrt(Gxgreen + Gygreen));
                    temp[i][j].rgbtRed = (round(sqrt(Gxred + Gyred)) > 255) ? 255 : round(sqrt(Gxred + Gyred));
                }
                if ((0 < j) && (j < (width - 1)))
                {   
                    int a = (image[i - 1][j - 1].rgbtRed * -1) + (image[i - 1][j].rgbtRed * 0) + (image[i - 1][j + 1].rgbtRed * 1) +
                            (image[i][j - 1].rgbtRed * -2) + (image[i][j].rgbtRed * 0) + (image[i][j + 1].rgbtRed * 2);
                    int b = (image[i - 1][j - 1].rgbtGreen * -1) + (image[i - 1][j].rgbtGreen * 0) +
                            (image[i - 1][j + 1].rgbtGreen * 1) + (image[i][j - 1].rgbtGreen * -2) + (image[i][j].rgbtGreen * 0) +
                            (image[i][j + 1].rgbtGreen * 2);
                    int c = (image[i - 1][j - 1].rgbtBlue * -1) + (image[i - 1][j].rgbtBlue * 0) +
                            (image[i - 1][j + 1].rgbtBlue * 1) + (image[i][j - 1].rgbtBlue * -2) + (image[i][j].rgbtBlue * 0) +
                            (image[i][j + 1].rgbtBlue * 2);
                    
                    int d = (image[i - 1][j - 1].rgbtRed * -1) + (image[i - 1][j].rgbtRed * -2) +
                            (image[i - 1][j + 1].rgbtRed * -1) + (image[i][j - 1].rgbtRed * 0) + (image[i][j].rgbtRed * 0) +
                            (image[i][j + 1].rgbtRed * 0);
                    int e = (image[i - 1][j - 1].rgbtGreen * -1) + (image[i - 1][j].rgbtGreen * -2) +
                            (image[i - 1][j + 1].rgbtGreen * -1) + (image[i][j - 1].rgbtGreen * 0) + (image[i][j].rgbtGreen * 0) +
                            (image[i][j + 1].rgbtGreen * 0);
                    int f = (image[i - 1][j - 1].rgbtBlue * -1) + (image[i - 1][j].rgbtBlue * -2) +
                            (image[i - 1][j + 1].rgbtBlue * -1) + (image[i][j - 1].rgbtBlue * 0) + (image[i][j].rgbtBlue * 0) +
                            (image[i][j + 1].rgbtBlue * 0);
                    
                    int Gxred = a * a;
                    int Gxgreen = b * b;
                    int Gxblue = c * c;
                    
                    int Gyred = d * d;
                    int Gygreen = e * e;
                    int Gyblue = f * f;

                    temp[i][j].rgbtBlue = (round(sqrt(Gxblue + Gyblue)) > 255) ? 255 : round(sqrt(Gxblue + Gyblue));
                    temp[i][j].rgbtGreen = (round(sqrt(Gxgreen + Gygreen)) > 255) ? 255 : round(sqrt(Gxgreen + Gygreen));
                    temp[i][j].rgbtRed = (round(sqrt(Gxred + Gyred)) > 255) ? 255 : round(sqrt(Gxred + Gyred));
                }
                if (j == (width - 1))
                {
                    
                    int a = (image[i - 1][j - 1].rgbtRed * -1) + (image[i - 1][j].rgbtRed * 0) + (image[i][j - 1].rgbtRed * -2) +
                            (image[i][j].rgbtRed * 0);
                    int b = (image[i - 1][j - 1].rgbtGreen * -1) + (image[i - 1][j].rgbtGreen * 0) +
                            (image[i][j - 1].rgbtGreen * -2) + (image[i][j].rgbtGreen * 0);
                    int c = (image[i - 1][j - 1].rgbtBlue * -1) + (image[i - 1][j].rgbtBlue * 0) + (image[i][j - 1].rgbtBlue * -2) +
                            (image[i][j].rgbtBlue * 0);
                    
                    int d = (image[i - 1][j - 1].rgbtRed * -1) + (image[i - 1][j].rgbtRed * -2) + (image[i][j - 1].rgbtRed * 0) +
                            (image[i][j].rgbtRed * 0);
                    int e = (image[i - 1][j - 1].rgbtGreen * -1) + (image[i - 1][j].rgbtGreen * -2) +
                            (image[i][j - 1].rgbtGreen * 0) + (image[i][j].rgbtGreen * 0);
                    int f = (image[i - 1][j - 1].rgbtBlue * -1) + (image[i - 1][j].rgbtBlue * -2) + (image[i][j - 1].rgbtBlue * 0) +
                            (image[i][j].rgbtBlue * 0);
                    
                    int Gxred = a * a;
                    int Gxgreen = b * b;
                    int Gxblue = c * c;
                    
                    int Gyred = d * d;
                    int Gygreen = e * e;
                    int Gyblue = f * f;

                    temp[i][j].rgbtBlue = (round(sqrt(Gxblue + Gyblue)) > 255) ? 255 : round(sqrt(Gxblue + Gyblue));
                    temp[i][j].rgbtGreen = (round(sqrt(Gxgreen + Gygreen)) > 255) ? 255 : round(sqrt(Gxgreen + Gygreen));
                    temp[i][j].rgbtRed = (round(sqrt(Gxred + Gyred)) > 255) ? 255 : round(sqrt(Gxred + Gyred));
                }
            }
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
    free(temp);
}
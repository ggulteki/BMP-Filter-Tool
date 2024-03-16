/*
 * Filename: filters.c
 * Author: Gokberk Gultekin
 * Date: March 16, 2024
 * Description: This is the implementation of a program that applies filters to BMP.
 */

#include "./include/filters.h"

/*
 * To convert a pixel to grayscale, we ensure that the red, green, and blue values 
 * are all set to the same value.
 */
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int a = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            image[i][j].rgbtBlue = a;
            image[i][j].rgbtGreen = a;
            image[i][j].rgbtRed = a;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*temp)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (temp == NULL)
    {
        exit(1);
    }

    for (int i = 0; i < height; i++)
    {
        if (width % 2 == 0)
        {
            for (int j = 0; j < width / 2; j++)
            {
                temp[i][j] = image[i][j];
                image[i][j] = image[i][width - 1 - j];
                image[i][width - 1 - j] = temp[i][j];
            }
        }
        else
        {
            for (int j = 0; j < width / 2; j++)
            {
                temp[i][j] = image[i][j];
                image[i][j] = image[i][width - 1 - j];
                image[i][width - 1 - j] = temp[i][j];
            }
        }
    }

    free(temp);
}

/*
 * There are a number of ways to create the effect of blurring or softening an image.
 * For this problem, we’ll use the “box blur,” which works by taking each pixel and, for each color value, giving it a
 * new value by averaging the color values of neighboring pixels.
 */
void blur(int height, int width, RGBTRIPLE image[height][width])
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
                    int a = round(
                        (image[i][j].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed) /
                        4.0);
                    int b = round((image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j].rgbtGreen +
                                   image[i + 1][j + 1].rgbtGreen) /
                                  4.0);
                    int c = round((image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j].rgbtBlue +
                                   image[i + 1][j + 1].rgbtBlue) /
                                  4.0);

                    temp[i][j].rgbtBlue = c;
                    temp[i][j].rgbtGreen = b;
                    temp[i][j].rgbtRed = a;
                }
                if ((0 < j) && (j < (width - 1)))
                {
                    int a = round((image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed +
                                   image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed) /
                                  6.0);
                    int b = round((image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen +
                                   image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen) /
                                  6.0);
                    int c = round((image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue +
                                   image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue) /
                                  6.0);

                    temp[i][j].rgbtBlue = c;
                    temp[i][j].rgbtGreen = b;
                    temp[i][j].rgbtRed = a;
                }
                if (j == (width - 1))
                {
                    int a = round(
                        (image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed) /
                        4.0);
                    int b = round((image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i + 1][j - 1].rgbtGreen +
                                   image[i + 1][j].rgbtGreen) /
                                  4.0);
                    int c = round((image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i + 1][j - 1].rgbtBlue +
                                   image[i + 1][j].rgbtBlue) /
                                  4.0);

                    temp[i][j].rgbtBlue = c;
                    temp[i][j].rgbtGreen = b;
                    temp[i][j].rgbtRed = a;
                }
            }

            if ((0 < i) && (i < (height - 1)))
            {
                if (j == 0)
                {
                    int a = round((image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j].rgbtRed +
                                   image[i][j + 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed) /
                                  6.0);
                    int b = round((image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j].rgbtGreen +
                                   image[i][j + 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen) /
                                  6.0);
                    int c = round((image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j].rgbtBlue +
                                   image[i][j + 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue) /
                                  6.0);

                    temp[i][j].rgbtBlue = c;
                    temp[i][j].rgbtGreen = b;
                    temp[i][j].rgbtRed = a;
                }
                if ((0 < j) && (j < (width - 1)))
                {
                    int a = round((image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed +
                                   image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed +
                                   image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed) /
                                  9.0);
                    int b = round((image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen +
                                   image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen +
                                   image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen) /
                                  9.0);
                    int c = round((image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue +
                                   image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue +
                                   image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue) /
                                  9.0);

                    temp[i][j].rgbtBlue = c;
                    temp[i][j].rgbtGreen = b;
                    temp[i][j].rgbtRed = a;
                }
                if (j == (width - 1))
                {
                    int a = round((image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i][j - 1].rgbtRed +
                                   image[i][j].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed) /
                                  6.0);
                    int b = round((image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i][j - 1].rgbtGreen +
                                   image[i][j].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen) /
                                  6.0);
                    int c = round((image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i][j - 1].rgbtBlue +
                                   image[i][j].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue) /
                                  6.0);

                    temp[i][j].rgbtBlue = c;
                    temp[i][j].rgbtGreen = b;
                    temp[i][j].rgbtRed = a;
                }
            }

            if (i == (height - 1))
            {
                if (j == 0)
                {
                    int a = round(
                        (image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed) /
                        4.0);
                    int b = round((image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j].rgbtGreen +
                                   image[i][j + 1].rgbtGreen) /
                                  4.0);
                    int c = round((image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j].rgbtBlue +
                                   image[i][j + 1].rgbtBlue) /
                                  4.0);

                    temp[i][j].rgbtBlue = c;
                    temp[i][j].rgbtGreen = b;
                    temp[i][j].rgbtRed = a;
                }
                if ((0 < j) && (j < (width - 1)))
                {
                    int a = round((image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed +
                                   image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed) /
                                  6.0);
                    int b = round((image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen +
                                   image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen) /
                                  6.0);
                    int c = round((image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue +
                                   image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue) /
                                  6.0);

                    temp[i][j].rgbtBlue = c;
                    temp[i][j].rgbtGreen = b;
                    temp[i][j].rgbtRed = a;
                }
                if (j == (width - 1))
                {
                    int a = round(
                        (image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i][j - 1].rgbtRed + image[i][j].rgbtRed) /
                        4.0);
                    int b = round((image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i][j - 1].rgbtGreen +
                                   image[i][j].rgbtGreen) /
                                  4.0);
                    int c = round((image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i][j - 1].rgbtBlue +
                                   image[i][j].rgbtBlue) /
                                  4.0);

                    temp[i][j].rgbtBlue = c;
                    temp[i][j].rgbtGreen = b;
                    temp[i][j].rgbtRed = a;
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

/*
 * In artificial intelligence algorithms for image processing, it is often useful to detect edges in an image: lines in
 * the image that create a boundary between one object and another. One way to achieve this effect is by applying the
 * Sobel operator to the image. Like image blurring, edge detection also works by taking each pixel, and modifying it
 * based on the 3x3 grid of pixels that surrounds that pixel. But instead of just taking the average of the nine pixels,
 * the Sobel operator computes the new value of each pixel by taking a weighted sum of the values for the surrounding
 * pixels.
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
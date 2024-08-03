#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int mean =
                round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = mean;
            image[i][j].rgbtGreen = mean;
            image[i][j].rgbtRed = mean;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sepiaRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen +
                                 .189 * image[i][j].rgbtBlue);
            int sepiaGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen +
                                   .168 * image[i][j].rgbtBlue);
            int sepiaBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen +
                                  .131 * image[i][j].rgbtBlue);

            image[i][j].rgbtRed = (sepiaRed <= 255) ? sepiaRed : 255;
            image[i][j].rgbtGreen = (sepiaGreen <= 255) ? sepiaGreen : 255;
            image[i][j].rgbtBlue = (sepiaBlue <= 255) ? sepiaBlue : 255;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE a = image[i][j];
            RGBTRIPLE b = image[i][width - 1 - j];
            image[i][j] = b;
            image[i][width - 1 - j] = a;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE im[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            im[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int meanBlue = 0x00;
            int meanGreen = 0x00;
            int meanRed = 0x00;
            float ctr = 0;
            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    if ((i - 1 + k) >= 0 && (j - 1 + l) >= 0 && (i - 1 + k) < height &&
                        (j - 1 + l) < width)
                    {
                        ctr = ctr + 1.0;
                        meanBlue = meanBlue + im[i - 1 + k][j - 1 + l].rgbtBlue;
                        meanGreen = meanGreen + im[i - 1 + k][j - 1 + l].rgbtGreen;
                        meanRed = meanRed + im[i - 1 + k][j - 1 + l].rgbtRed;
                    }
                }
            }

            image[i][j].rgbtBlue = round(meanBlue / ctr);
            image[i][j].rgbtGreen = round(meanGreen / ctr);
            image[i][j].rgbtRed = round(meanRed / ctr);
        }
    }
    return;
}

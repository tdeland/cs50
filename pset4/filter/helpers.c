#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // calculate average intensity
            int average = (int) round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            // assign same value to each color
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        // loop over half of the image width
        for (int j = 0; j < width / 2; j++)
        {
            // assign target to temp
            RGBTRIPLE temp = image[i][j];
            // replace target with mirrored pixel
            image[i][j] = image[i][width - j - 1];
            // replace mirrored pixel with original target
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // copy image into temp
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    // loop through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // initialize color conainers
            int red = 0;
            int green = 0;
            int blue = 0;
            float dividend = 0; // for float precision
            // loop through each pixel in a 3 x 3 block around i, j
            for (int k = i - 1; k <= i + 1; k ++)
            {
                for (int l = j - 1; l <= j + 1; l++)
                {
                    // check that target pixel is within bounds of the image array
                    if (k >= 0 && k < height && l >= 0 && l < width)
                    {
                        // running tally of colors and number of pixels
                        red += temp[k][l].rgbtRed;
                        green += temp[k][l].rgbtGreen;
                        blue += temp[k][l].rgbtBlue;
                        dividend++;
                    }
                }
            }
            // calculate average value for each color and assign to original image
            image[i][j].rgbtRed = (int) round(red / dividend);
            image[i][j].rgbtGreen = (int) round(green / dividend);
            image[i][j].rgbtBlue = (int) round(blue / dividend);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // copy image into temp
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    // define edge kernels
    int Gx[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int Gy[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    // loop through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // initialize color containters
            int red_x = 0;
            int grn_x = 0;
            int blu_x = 0;
            int red_y = 0;
            int grn_y = 0;
            int blu_y = 0;
            // loop through each pixel in a 3 x 3 block around i, j
            for (int k = i - 1; k <= i + 1; k ++)
            {
                for (int l = j - 1; l <= j + 1; l++)
                {
                    // check that target pixel is within bounds of the image array
                    if (k >= 0 && k < height && l >= 0 && l < width)
                    {
                        red_x += temp[k][l].rgbtRed * Gx[k - i + 1][l - j + 1];
                        grn_x += temp[k][l].rgbtGreen * Gx[k - i + 1][l - j + 1];
                        blu_x += temp[k][l].rgbtBlue * Gx[k - i + 1][l - j + 1];
                        red_y += temp[k][l].rgbtRed * Gy[k - i + 1][l - j + 1];
                        grn_y += temp[k][l].rgbtGreen * Gy[k - i + 1][l - j + 1];
                        blu_y += temp[k][l].rgbtBlue * Gy[k - i + 1][l - j + 1];
                    }
                }
            }

            // calculate sqrt sum squares for each color
            int red = (int) round(sqrt(pow(red_x, 2) + pow(red_y, 2)));
            int grn = (int) round(sqrt(pow(grn_x, 2) + pow(grn_y, 2)));
            int blu = (int) round(sqrt(pow(blu_x, 2) + pow(blu_y, 2)));

            // check value against max threshold and assign
            image[i][j].rgbtRed = (red > 255) ? 255 : red;
            image[i][j].rgbtGreen = (grn > 255) ? 255 : grn;
            image[i][j].rgbtBlue = (blu > 255) ? 255 : blu;
        }
    }
    return;
}

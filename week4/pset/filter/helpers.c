// BMP-related data types based on Microsoft's own

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 * Common Data Types
 *
 * The data types in this section are essentially aliases for C/C++
 * primitive data types.
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/cc230309.aspx.
 * See http://en.wikipedia.org/wiki/Stdint.h for more on stdint.h.
 */
typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;

/**
 * BITMAPFILEHEADER
 *
 * The BITMAPFILEHEADER structure contains information about the type, size,
 * and layout of a file that contains a DIB [device-independent bitmap].
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/dd183374(VS.85).aspx.
 */
typedef struct
{
    WORD   bfType;
    DWORD  bfSize;
    WORD   bfReserved1;
    WORD   bfReserved2;
    DWORD  bfOffBits;
} __attribute__((__packed__))
BITMAPFILEHEADER;

/**
 * BITMAPINFOHEADER
 *
 * The BITMAPINFOHEADER structure contains information about the
 * dimensions and color format of a DIB [device-independent bitmap].
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/dd183376(VS.85).aspx.
 */
typedef struct
{
    DWORD  biSize;
    LONG   biWidth;
    LONG   biHeight;
    WORD   biPlanes;
    WORD   biBitCount;
    DWORD  biCompression;
    DWORD  biSizeImage;
    LONG   biXPelsPerMeter;
    LONG   biYPelsPerMeter;
    DWORD  biClrUsed;
    DWORD  biClrImportant;
} __attribute__((__packed__))
BITMAPINFOHEADER;

/**
 * RGBTRIPLE
 *
 * This structure describes a color consisting of relative intensities of
 * red, green, and blue.
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/aa922590.aspx.
 */
typedef struct
{
    BYTE  rgbtBlue;
    BYTE  rgbtGreen;
    BYTE  rgbtRed;
} __attribute__((__packed__))
RGBTRIPLE;

// so that the values are not capped at 255 (just for intermediary use)
typedef struct
{
    float rgbtBlue;
    float rgbtGreen;
    float rgbtRed;
}
RGBFLOAT;

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]);

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]);

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width]);

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]);

// performs convolution
RGBFLOAT convolute3x3(int height, int width, RGBTRIPLE image[height][width], int i, int j, float kernel[3][3]);

int trunc255(double x);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // calculates average color
            int avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);

            // initisalises a new pixel (RGB-Triple)
            RGBTRIPLE new_pixel = {.rgbtRed = avg, .rgbtGreen = avg, .rgbtBlue = avg};

            // sets current pixel to new pixel
            image[i][j] = new_pixel;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Idea: we scan the image row by row, and each time reverse the row, by filling a new row that we malloc

    // scan each row
    for (int i = 0; i < height; i++)
    {
        // create space in memory for the new row
        RGBTRIPLE *new_row = malloc(sizeof(RGBTRIPLE) * width);

        // loop through the current row backwards and fill each pixel into the new row
        for (int j = 0; j < width; j++)
        {
            new_row[j] = image[i][width - 1 - j];
        }

        // set the current row of the original image equal to new_row
        for (int j = 0; j < width; j++)
        {
            image[i][j] = new_row[j];
        }

        // free memory
        free(new_row);
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // first save original image in new array
    RGBTRIPLE ogImage[height][width];

    // deepcopy image to osImage
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            ogImage[i][j] = image[i][j];
        }
    }

    // loop through all pixels of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumRed = 0;
            int sumBlue = 0;
            int sumGreen = 0;
            int count = 0;
            // loop through all neighbouring pixels
            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    // check if pixel at x,y around origin i,j is within image
                    int iShift = i - 1 + x;
                    int jShift = j - 1 + y;
                    if (iShift >= 0 && iShift < height && jShift >= 0 && jShift < width)
                    {
                        sumRed += ogImage[iShift][jShift].rgbtRed;
                        sumBlue += ogImage[iShift][jShift].rgbtBlue;
                        sumGreen += ogImage[iShift][jShift].rgbtGreen;

                        count++;
                    }
                }
            }
            RGBTRIPLE newPixel = {.rgbtRed = round(sumRed / (float)count),
                                  .rgbtBlue = round(sumBlue / (float)count),
                                  .rgbtGreen = round(sumGreen / (float)count)
                                 };
            image[i][j] = newPixel;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // first save original image in new array
    RGBTRIPLE ogImage[height][width];

    // deepcopy image to osImage
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            ogImage[i][j] = image[i][j];
        }
    }

    // define  kernels that will be applied to each pixel of the image
    float Gx[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    float Gy[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    // apply kernel to every pixel of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBFLOAT pixelGx = convolute3x3(height, width, ogImage, i, j, Gx);
            RGBFLOAT pixelGy = convolute3x3(height, width, ogImage, i, j, Gy);

            image[i][j].rgbtRed = trunc255(sqrt(pow(pixelGx.rgbtRed, 2) + pow(pixelGy.rgbtRed, 2)));
            image[i][j].rgbtBlue = trunc255(sqrt(pow(pixelGx.rgbtBlue, 2) + pow(pixelGy.rgbtBlue, 2)));
            image[i][j].rgbtGreen = trunc255(sqrt(pow(pixelGx.rgbtGreen, 2) + pow(pixelGy.rgbtGreen, 2)));
        }
    }
    return;
}

// performs kernel convolution (only for 3x3 kernels for now)
RGBFLOAT convolute3x3(int height, int width, RGBTRIPLE image[height][width], int i, int j, float kernel[3][3])
{
    // get a 3x3 array of RGBTRIPLES centered around i,j and pad things outside of boundaries with Null or something
    RGBTRIPLE image_snippet[3][3];

    // define black pixel
    RGBTRIPLE black_pixel = {.rgbtRed = 0, .rgbtBlue = 0, .rgbtGreen = 0};

    // loop over 3x3 grid of pixels
    for (int y = 0; y < 3; y++)
    {
        int iShift = i - 1 + y;

        for (int x = 0; x < 3; x++)
        {
            int jShift = j - 1 + x;


            // if shifted pixel is within original image boundaries
            if (iShift >= 0 && iShift < height && jShift >= 0 && jShift < width)
            {
                image_snippet[y][x] = image[iShift][jShift];
            }
            // if not within boundaries set pixel to
            else
            {
                image_snippet[y][x] = black_pixel;
            }
        }
    }

    // do the convolution
    float red = 0;
    float green = 0;
    float blue = 0;


    for (int y = 0; y < 3; y++)
    {
        for (int x = 0; x < 3; x++)
        {
            float k = kernel[y][x];

            red += image_snippet[y][x].rgbtRed * k;
            blue += image_snippet[y][x].rgbtBlue * k;
            green += image_snippet[y][x].rgbtGreen * k;
        }
    }
    RGBFLOAT new_pixel = {.rgbtRed = red, .rgbtBlue = blue, .rgbtGreen = green};
    return new_pixel;
}

// retuns 255 if argument is above 255,else returns rounded argument
int trunc255(double x)
{
    if (x > 255)
    {
        return 255;
    }
    else
    {
        return round(x);
    }
}
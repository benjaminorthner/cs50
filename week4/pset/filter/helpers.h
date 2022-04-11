#include "bmp.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]);

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]);

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width]);

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]);

// performs convolution
RGBTRIPLE convolute3x3(int height, int width, RGBTRIPLE image[height][width], int i, int j, float kernel[3][3]);

int trunc255(double x);
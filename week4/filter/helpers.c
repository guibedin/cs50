#include "helpers.h"
#include <stdio.h>
#include <math.h>

#define TOP_LEFT 0
#define TOP_EDGE 1
#define TOP_RIGHT 2
#define LEFT_EDGE 3
#define RIGHT_EDGE 4
#define BOTTOM_LEFT 5
#define BOTTOM_EDGE 6
#define BOTTOM_RIGHT 7
#define MIDDLE 8


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //printf("RGB: (%i, %i, %i)\n", image[0][0].rgbtRed, image[0][0].rgbtGreen, image[0][0].rgbtBlue);
    for(int h = 0; h < height; h++) {
        for(int w = 0; w < width; w++) {
            double mean = (double) (image[h][w].rgbtRed + image[h][w].rgbtGreen + image[h][w].rgbtBlue) / 3;

            image[h][w].rgbtRed = (int) round(mean);
            image[h][w].rgbtGreen = (int) round(mean);
            image[h][w].rgbtBlue = (int) round(mean);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for(int h = 0; h < height; h++) {
        for(int w = 0; w < width / 2; w++) {
            // swap first with last, second with second to last, etc...
            int temp[3] = {image[h][width - w - 1].rgbtRed, image[h][width - w - 1].rgbtGreen, image[h][width - w - 1].rgbtBlue};

            image[h][width - w - 1].rgbtRed = image[h][w].rgbtRed;
            image[h][width - w - 1].rgbtGreen = image[h][w].rgbtGreen;
            image[h][width - w - 1].rgbtBlue = image[h][w].rgbtBlue;

            image[h][w].rgbtRed = temp[0];
            image[h][w].rgbtGreen = temp[1];
            image[h][w].rgbtBlue = temp[2];
        }
    }
    return;
}

void calc_blur(int h, int w, int i, int i2, int j, int j2, int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE blur_image[height][width]) {

    // Totals for each color to calculate mean
    int totalR = 0;
    int totalG = 0;
    int totalB = 0;

    // number of pixels to calculate mean
    int total_pixels = 0;

    for(int a = i; a < i2; a++) {
        for(int b = j; b < j2; b++) {
            totalR += image[h + a][w + b].rgbtRed;
            totalG += image[h + a][w + b].rgbtGreen;
            totalB += image[h + a][w + b].rgbtBlue;
            total_pixels++;
        }
    }

    // Calc mean for blur
    float mR = (float) totalR / total_pixels;
    float mG = (float) totalG / total_pixels;
    float mB = (float) totalB / total_pixels;

    blur_image[h][w].rgbtRed = round(mR);
    blur_image[h][w].rgbtGreen = round(mG);
    blur_image[h][w].rgbtBlue = round(mB);
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blur_image[height][width];

    for(int h = 0; h < height; h++) {
        for(int w = 0; w < width; w++) {
            //printf("image[h][w].red: %i - ", image[h][w].rgbtRed);
            // Top edge
            if(h == 0) {
                if(w == 0) { // top left corner
                    calc_blur(h, w, 0, 2, 0, 2, height, width, image, blur_image);
                }

                if(w > 0 && w < width - 1) { // top edge
                    calc_blur(h, w, 0, 2, -1, 2, height, width, image, blur_image);
                }

                if(w == width - 1) { // top right corner
                    calc_blur(h, w, 0, 2, -1, 1, height, width, image, blur_image);
                }
            }

            // Middle
            if(h > 0 && h < height - 1) {
                if(w == 0) { // left edge
                    calc_blur(h, w, -1, 2, 0, 2, height, width, image, blur_image);
                }

                if(w > 0 && w < width - 1) { // middle of the image
                    calc_blur(h, w, -1, 2, -1, 2, height, width, image, blur_image);
                }

                if(w == width - 1) { // right edge
                    calc_blur(h, w, -1, 2, -1, 1, height, width, image, blur_image);
                }
            }

            // Bottom edge
            if(h == height - 1) {
                if(w == 0) { // bottom left corner
                    calc_blur(h, w, -1, 1, 0, 2, height, width, image, blur_image);
                }

                if(w > 0 && w < width - 1) { // bottom edge
                    calc_blur(h, w, -1, 1, -1, 2, height, width, image, blur_image);
                }

                if(w == width - 1) { // bottom right corner
                    calc_blur(h, w, -1, 1, -1, 1, height, width, image, blur_image);
                }
            }
        }
    }

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            image[i][j] = blur_image[i][j];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{

    // creates image with borders - easier to calc Gx and Gy
    RGBTRIPLE edge_image[height + 2][width + 2];

    int gx[3][3] = {{-1, 0, 1},
                    {-2, 0, 2},
                    {-1, 0, 1}};

    int gy[3][3] = {{-1 , -2, -1},
                    {0, 0, 0},
                    {1, 2, 1}};

    // add 1 row at top and bottom
    for(int i = 0; i < width + 2; i++) {
        edge_image[0][i].rgbtRed = 0;
        edge_image[0][i].rgbtGreen = 0;
        edge_image[0][i].rgbtBlue = 0;

        edge_image[height + 1][i].rgbtRed = 0;
        edge_image[height + 1][i].rgbtGreen = 0;
        edge_image[height + 1][i].rgbtBlue = 0;
    }

    // add 1 column to left and right
    for(int i = 0; i < height + 2; i++) {
        edge_image[i][0].rgbtRed = 0;
        edge_image[i][0].rgbtGreen = 0;
        edge_image[i][0].rgbtBlue = 0;

        edge_image[i][width + 1].rgbtRed = 0;
        edge_image[i][width + 1].rgbtGreen = 0;
        edge_image[i][width + 1].rgbtBlue = 0;
    }

    // copy image to edge image
    for(int h = 0; h < height; h++) {
        for(int w = 0; w < width; w++) {
            edge_image[h + 1][w + 1] = image[h][w];
        }
    }

    for(int h = 0; h < height; h++) {
        // Sums of each color * by gx
        int sumGxR, sumGxG, sumGxB;

        // Sums of each color * by gy
        int sumGyR, sumGyG, sumGyB;

        for(int w = 0; w < width; w++) {
            sumGxR = 0;
            sumGxG = 0;
            sumGxB = 0;
            sumGyR = 0;
            sumGyG = 0;
            sumGyB = 0;

            for(int i = -1; i < 2; i++) {
                for(int j = -1; j < 2; j++) {
                    //printf("h: %i - h+i+1: %i - i+1: %i\n", h, h+i+1, i+1);
                    sumGxR += (edge_image[h + i + 1][w + j + 1].rgbtRed * gx[i + 1][j + 1]);
                    sumGxG += (edge_image[h + i + 1][w + j + 1].rgbtGreen * gx[i + 1][j + 1]);
                    sumGxB += (edge_image[h + i + 1][w + j + 1].rgbtBlue * gx[i + 1][j + 1]);

                    sumGyR += (edge_image[h + i + 1][w + j + 1].rgbtRed * gy[i + 1][j + 1]);
                    sumGyG += (edge_image[h + i + 1][w + j + 1].rgbtGreen * gy[i + 1][j + 1]);
                    sumGyB += (edge_image[h + i + 1][w + j + 1].rgbtBlue * gy[i + 1][j + 1]);
                }
            }

            // Calc rgb using gx and gy
            int r = (int) round(sqrt(pow(sumGxR, 2) + pow(sumGyR, 2)));
            int g = (int) round(sqrt(pow(sumGxG, 2) + pow(sumGyG, 2)));
            int b = (int) round(sqrt(pow(sumGxB, 2) + pow(sumGyB, 2)));

            image[h][w].rgbtRed = r;
            image[h][w].rgbtGreen = g;
            image[h][w].rgbtBlue = b;

            // cap to 255
            if(r > 255) {
                image[h][w].rgbtRed = 255;
            }
            if(g > 255) {
                image[h][w].rgbtGreen = 255;
            }
            if(b > 255) {
                image[h][w].rgbtBlue = 255;
            }
        }
    }

    return;
}

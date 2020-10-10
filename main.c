/*
--------------------------------------------------------
Img2Ascii
https://github.com/vtiinanen/Img2Ascii
--------------------------------------------------------
MIT License
Copyright (c) 2020 vtiinanen
--------------------------------------------------------
RELEASE: 1.0.0
--------------------------------------------------------
*/

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_resize.h"

// Constants
// " .:-=+*#%@"
const int SET1[] = {32, 46, 58, 45, 61, 43, 42, 35, 37, 64};
// " .'`^",:;Il!i><~+_-?][}{1)(|\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$"
const int SET2[] = {32, 46, 39, 96, 94, 34, 44, 58, 59, 73, 108, 33, 105, 62, 60, 126, 43, 95, 45, 63, 93, 91, 125, 123, 49, 41, 40, 124, 92, 47, 116, 102, 106, 114, 120, 110, 117, 118, 99, 122, 88, 89, 85, 74, 67, 76, 81, 48, 79, 90, 109, 119, 113, 112, 100, 98, 107, 104, 97, 111, 42, 35, 77, 87, 38, 56, 37, 66, 64, 36};

// Global variables
int selectedSet = 1;
float scale = 1.0;
char filename[240];

// Func dec's
void generateAscii2Stdio(unsigned char *pixelData, size_t imgSize, int width);

int main(int argc, char *argv[])
{
    // cmd-line arguments handling
    int opt;
    int exitFlag = 0;
    // put ':' in the starting of the
    // string so that program can
    // distinguish between '?' and ':'
    while ((opt = getopt(argc, argv, ":f:s:b")) != -1)
    {
        switch (opt)
        {
        case 'b':
            selectedSet = 2;
            break;
        case 'f':
            strncpy(filename, optarg, sizeof(filename) - 1);
            filename[sizeof(filename) - 1] = '\0';
            break;
        case 's':
            scale = atof(optarg);
            break;
        case ':':
            fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            exitFlag++;
            break;
        case '?':
            fprintf(stderr, "Unknown option -%c option.\n", optopt);
            break;
        }
    }

    if (exitFlag > 0)
    {
        exit(1);
    }

    // optind is for the extra arguments
    // which are not parsed
    for (; optind < argc; optind++)
    {
        fprintf(stderr, "Extra arguments  %s\n", argv[optind]);
    }

    // Load the image
    int width, height, channels_in_file;
    unsigned char *pixelData = stbi_load(filename, &width, &height, &channels_in_file, 3);
    if (pixelData == NULL)
    {
        fprintf(stderr, "Error in loading the image\n");
        exit(1);
    }

    size_t img_size = width * height * 3;

    int new_width = (float)width * scale;
    int new_height = (float)height * scale;
    size_t new_img_size = new_width * new_height * 3;

    unsigned char *new_pixelData = NULL;
    new_pixelData = (unsigned char *)malloc(new_img_size);

    stbir_resize_uint8(pixelData, width, height, 0, new_pixelData, new_width, new_height, 0, 3);
    generateAscii2Stdio(new_pixelData, new_img_size, new_width);
    stbi_image_free(pixelData);
    free(new_pixelData);

    return 0;
}

void generateAscii2Stdio(unsigned char *pixelData, size_t imgSize, int width)
{
    unsigned char *ptr_limit = pixelData + imgSize;
    int i = 1;
    float rgb[3];
    float grayScaleValue = 0.0;
    for (; pixelData < ptr_limit; pixelData++, i++)
    {
        rgb[0] = *pixelData;
        pixelData++;
        rgb[1] = *pixelData;
        pixelData++;
        rgb[2] = *pixelData;

        // Ponderate colors with different weight
        grayScaleValue = (0.21 * rgb[0]) + (0.72 * rgb[1]) + (0.07 * rgb[2]);

        // Print to stdout
        if (selectedSet == 1)
        {
            fprintf(stdout, "%c", SET1[(int)(grayScaleValue / 28.333)]); // 255 / 9 = 28.333
            fprintf(stdout, "%c", SET1[(int)(grayScaleValue / 28.333)]); // 255 / 9 = 28.333
        }
        if (selectedSet == 2)
        {
            fprintf(stdout, "%c", SET2[(int)(grayScaleValue / 3.642)]); // 255 / 69 = 3.695
            fprintf(stdout, "%c", SET2[(int)(grayScaleValue / 3.642)]); // 255 / 69 = 3.695
        }
        if (i == width)
        {
            fprintf(stdout, "\n");
            i = 0;
        }
    }
}

/*
    revision history:
        1.0.0  (2020-10-10) first released version
*/
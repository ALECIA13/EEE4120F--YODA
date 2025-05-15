// sobel.c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Sobel kernels
static const int gx[3][3] = {
    { -1, 0, +1 },
    { -2, 0, +2 },
    { -1, 0, +1 }
};
static const int gy[3][3] = {
    { +1, +2, +1 },
    {  0,  0,  0 },
    { -1, -2, -1 }
};

int main(void) {
    const char *infile  = "img_filtered_greyscale.jpg";
    const char *outfile = "img_sobel_edges_uct.jpg";

    int width, height, channels;
    // Load as single‑channel (greyscale)
    unsigned char *grey = stbi_load(infile, &width, &height, &channels, 1);
    if (!grey) {
        fprintf(stderr, "Error: could not load image '%s'\n", infile);
        return 1;
    }
    printf("Loaded image: %dx%d, %d channel(s)\n", width, height, 1);

    // Allocate output buffer
    unsigned char *edges = malloc(width * height);
    if (!edges) {
        fprintf(stderr, "Error: could not allocate memory for edges\n");
        stbi_image_free(grey);
        return 1;
    }

    // Zero borders
    for (int x = 0; x < width; x++) {
        edges[x] = 0;
        edges[(height-1)*width + x] = 0;
    }
    for (int y = 0; y < height; y++) {
        edges[y*width + 0] = 0;
        edges[y*width + (width-1)] = 0;
    }

    // Apply Sobel filter
    for (int y = 1; y < height-1; y++) {
        for (int x = 1; x < width-1; x++) {
            int sumX = 0, sumY = 0;
            for (int j = -1; j <= 1; j++) {
                for (int i = -1; i <= 1; i++) {
                    int pixel = grey[(y + j) * width + (x + i)];
                    sumX += gx[j+1][i+1] * pixel;
                    sumY += gy[j+1][i+1] * pixel;
                }
            }
            int mag = (int)(sqrt((double)(sumX*sumX + sumY*sumY)));
            edges[y * width + x] = (unsigned char)(mag > 255 ? 255 : mag);
        }
    }

    // Write out the edge image
    if (!stbi_write_jpg(outfile, width, height, 1, edges, 90)) {
        fprintf(stderr, "Error: failed to write '%s'\n", outfile);
        stbi_image_free(grey);
        free(edges);
        return 1;
    }

    printf("Sobel edges saved to '%s'\n", outfile);

    stbi_image_free(grey);
    free(edges);
    return 0;
}

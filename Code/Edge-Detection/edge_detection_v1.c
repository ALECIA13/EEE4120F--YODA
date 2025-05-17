// ==============================================================================================
// Standard libraries
// ==============================================================================================
#include <stdint.h> // Defines integer types
#include <stdio.h>  // For I/O operations
#include <stdlib.h> // For memory allocation
#include <math.h>   // For mathematical operations

// ==============================================================================================
// Single header libraries
// stb_image.h - https://github.com/nothings/stb/blob/master/stb_image.h
// stb_image_write.h - https://github.com/nothings/stb/blob/master/stb_image_write.h
// ==============================================================================================
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // Handles image loading
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h" // Handles image saving

// ==============================================================================================
// Constants - Sobel Kernels
// ==============================================================================================
// Horizontal edge detection kernel (Gx)
static const int gx[3][3] = {
    { -1, 0, +1 },
    { -2, 0, +2 },
    { -1, 0, +1 }
};

// Vertical edge detection kernel (Gy)
static const int gy[3][3] = {
    { +1, +2, +1 },
    {  0,  0,  0 },
    { -1, -2, -1 }
};

/**
 * @brief Main function to perform Sobel edge detection on a greyscale image.
 *
 * Loads a greyscale image, applies the Sobel operator to detect edges,
 * and writes the resulting edge-detected image to a file.
 *
 * @return int Returns 0 on success, or 1 on failure (file loading, allocation, or writing error).
 */
int main(void) {
    // Input and output files
    const char *infile  = "img_filtered_greyscale.jpg";
    const char *outfile = "img_edge_detection.jpg";

    int width, height, channels;

    // Load the greyscale image
    unsigned char *grey = stbi_load(infile, &width, &height, &channels, 1);
    if (!grey) {
        fprintf(stderr, "Error: could not load image '%s'\n", infile);
        return 1; // Exit if loading fails
    }
    printf("Loaded image: %dx%d, %d channel(s)\n", width, height, 1);

    // Allocate memory for the output edge image
    unsigned char *edges = malloc(width * height);
    if (!edges) {
        fprintf(stderr, "Error: could not allocate memory for edges\n");
        stbi_image_free(grey);
        return 1;
    }

    // Set all border pixels to 0
    for (int x = 0; x < width; x++) {
        edges[x] = 0; // Top row
        edges[(height-1)*width + x] = 0; // Bottom row
    }
    for (int y = 0; y < height; y++) {
        edges[y*width + 0] = 0; // Left column
        edges[y*width + (width-1)] = 0; // Right column
    }

    // Process each non-border pixel in the image
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            // Sums for horizontal and vertical gradients
            int sumX = 0, sumY = 0;  

            // Apply both Sobel kernels to the 3x3 neighborhood around current pixel
            for (int j = -1; j <= 1; j++) {
                for (int i = -1; i <= 1; i++) {
                    // Get the greyscale value of the current neighborhood pixel
                    int pixel = grey[(y + j) * width + (x + i)];
                    
                    // Apply kernel weights and update gradient components
                    sumX += gx[j + 1][i + 1] * pixel;  // Horizontal gradient component
                    sumY += gy[j + 1][i + 1] * pixel;  // Vertical gradient component
                }
            }

            // Compute gradient magnitude
            int magnitude = (int)(sqrt((double)(sumX * sumX + sumY * sumY)));
            
            // Limit the value to the valid range [0, 255] and store in output
            if (magnitude > 255) {magnitude = 255;}
            edges[y * width + x] = (unsigned char)(magnitude);
        }
    }
    
    // Save the edge-detected image to a file
    if (!stbi_write_jpg(outfile, width, height, 1, edges, 90)) {
        fprintf(stderr, "Error: failed to write '%s'\n", outfile);
        stbi_image_free(grey);
        free(edges);
        return 1;
    }

    printf("Edge-detected image saved to '%s'\n", outfile);

    // Free allocated resources
    stbi_image_free(grey);
    free(edges);
    return 0;
}

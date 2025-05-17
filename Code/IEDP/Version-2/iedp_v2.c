/**
 * @file iedp.c
 * @brief Image processing pipeline: Median Filter → Greyscale → Sobel Edge Detection
 */

// ==============================================================================================
// Standard libraries
// ==============================================================================================
#include <stdint.h> // Defines integer types
#include <stdio.h> // For I/O operations
#include <stdlib.h> // For memory allocation
#include <math.h> // For mathematical operations
#include <string.h> // For string operations

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
// Constants and Structures
// ==============================================================================================
// Defines the window size for the median filter
#define WINDOW_SIZE 3 

/**
 * @brief Structure representing an RGB pixel
 */
typedef struct {
    uint8_t r; // Red channel
    uint8_t g; // Green channel
    uint8_t b; // Blue channel
} RGB;
// Macro to compute brightness by summing RGB components
#define BRIGHTNESS(p) ((p).r + (p).g + (p).b) 

// ==============================================================================================
// A: Median Filter - Applies median filter to an RGB image
// ==============================================================================================
/**
 * @brief Applies a 3x3 median filter to an RGB image to reduce noise while preserving edges
 * 
 * @param input  Pointer to the input image data
 * @param output Pointer to the output image data
 * @param height Image height
 * @param width  Image width
 */
void MedianFilter(unsigned char *input, unsigned char *output, int height, int width) {
    // Process each pixel in the image
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Calculate the index of the current pixel
            int current_idx = (y * width + x) * 3;  

            // Skip edge pixels (cannot apply a full 3x3 filter at edges)
            if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
                // Copy original pixel values to output
                output[current_idx]     = input[current_idx];     // Red channel
                output[current_idx + 1] = input[current_idx + 1]; // Green channel
                output[current_idx + 2] = input[current_idx + 2]; // Blue channel
                continue;  // Move to next pixel
            }

            // Array to store the 3x3 window of pixels around the current position
            RGB window[WINDOW_SIZE * WINDOW_SIZE];
            int idx = 0;  // Index counter for the window array

            // Collect the 3x3 neighborhood of pixels around the current pixel
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    // Calculate the index of the neighbor pixel - FIXED HERE
                    int neighbor_idx = ((y + dy) * width + (x + dx)) * 3;
                    
                    // Store RGB values in the window array
                    window[idx].r = input[neighbor_idx];     // Red channel
                    window[idx].g = input[neighbor_idx + 1]; // Green channel
                    window[idx].b = input[neighbor_idx + 2]; // Blue channel
                    idx++;  // Move to next position in window array
                }
            }

            // Sort the pixels in ascending order of brightness using bubble sort
            for (int k = 0; k < WINDOW_SIZE * WINDOW_SIZE - 1; k++) {
                for (int l = 0; l < WINDOW_SIZE * WINDOW_SIZE - 1 - k; l++) {
                    // Compare brightness of adjacent pixels
                    if (BRIGHTNESS(window[l]) > BRIGHTNESS(window[l + 1])) {
                        // Swap pixels if they're in the wrong order
                        RGB temp = window[l];
                        window[l] = window[l + 1];
                        window[l + 1] = temp;
                    }
                }
            }

            // Assign the median pixel (middle of sorted array) to the output
            // For a 3x3 window (9 pixels), the median is at index 4
            output[current_idx]     = window[4].r;  // Red channel of median
            output[current_idx + 1] = window[4].g;  // Green channel of median
            output[current_idx + 2] = window[4].b;  // Blue channel of median
        }
    }
}

// ==============================================================================================
// B: Greyscale Conversion - Converts RGB image to greyscale
// ==============================================================================================
/**
 * @brief Converts an RGB image to greyscale.
 *
 * @param input  Pointer to input RGB image data
 * @param output Pointer to output greyscale image data
 * @param height Image height
 * @param width  Image width
 */
void ConvertToGreyscale(unsigned char *input, unsigned char *output, int height, int width) {
    // Process each pixel in the image row by row, column by column
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Calculate the index of the current pixel in the input RGB image
            int idx = (y * width + x) * 3;
            
            // Extract individual RGB channel values
            uint8_t r = input[idx];        // Red channel
            uint8_t g = input[idx + 1];    // Green channel
            uint8_t b = input[idx + 2];    // Blue channel
            
            // Convert RGB to greyscale using standard luminance formula
            output[y * width + x] = (uint8_t)(0.299 * r + 0.587 * g + 0.114 * b);
        }
    }
}

// ==============================================================================================
// C: Sobel Edge Detection - Detects edges in greyscale image
// ==============================================================================================
/**
 * @brief Applies Sobel edge detection to a greyscale image.
 *
 * @param grey  Pointer to input greyscale image data
 * @param edges Pointer to output edge image data
 * @param width Image width
 * @param height Image height
 */
void SobelEdgeDetection(unsigned char *grey, unsigned char *edges, int width, int height) {
    // Define Sobel operator kernels for detecting edges
    // gx detects horizontal edges (vertical gradients)
    static const int gx[3][3] = {
        { -1, 0, +1 },  // Top row: detect horizontal change
        { -2, 0, +2 },  // Middle row: stronger weight for center pixels
        { -1, 0, +1 }   // Bottom row: detect horizontal change
    };
    
    // gy detects vertical edges (horizontal gradients)
    static const int gy[3][3] = {
        { +1, +2, +1 },  // Top row: positive weights
        {  0,  0,  0 },  // Middle row: zeros (no horizontal gradient detection)
        { -1, -2, -1 }   // Bottom row: negative weights to detect vertical change
    };

    // Set all border pixels to 0 since we can't apply the 3x3 kernel there
    // Set top and bottom row borders to zero
    for (int x = 0; x < width; x++) {
        edges[x] = 0;                        // Top row
        edges[(height - 1) * width + x] = 0; // Bottom row
    }
    
    // Set left and right column borders to zero
    for (int y = 0; y < height; y++) {
        edges[y * width] = 0;            // Left column
        edges[y * width + (width - 1)] = 0;  // Right column
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
            if (magnitude > 255) {
                magnitude = 255;
            }
            edges[y * width + x] = (unsigned char)(magnitude);
        }
    }
}

// ==============================================================================================
// Main Function
// ==============================================================================================
/**
 * @brief Entry point of the image processing program.
 * 1. Loads an RGB image.
 * 2. Applies a median filter.
 * 3. Converts to greyscale,
 * 4. Performs Sobel edge detection. 
 * 5. Outputs all results to files.
 *
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @return 0 on success, 1 on failure
 */
int main(int argc, char *argv[]) {
    // Define input and output file paths
    const char *infile;
    char filtered_outfile[256];
    char grey_outfile[256];
    char edge_outfile[256];
    
    // Process command line arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_image>\n", argv[0]);
        fprintf(stderr, "Example: %s input.jpg\n", argv[0]);
        return 1;
    }
    
    // First argument is the input image file
    infile = argv[1];
    
    // Generate output filenames based on input filename
    // Find the last dot in the filename to extract the base name
    const char *last_dot = strrchr(infile, '.');
    const char *last_slash = strrchr(infile, '/');
    
    // Figure out the base filename (without directory or extension)
    const char *base_name;
    if (last_slash && last_dot > last_slash) {
        base_name = last_slash + 1;
    } else if (!last_slash && last_dot) {
        base_name = infile;
    } else {
        base_name = infile;
    }
    
    // Length for the base part of the filename (without extension)
    size_t base_len = last_dot ? (size_t)(last_dot - base_name) : strlen(base_name);
    
    // Create output filenames
    snprintf(filtered_outfile, sizeof(filtered_outfile), "%.*s_filtered.jpg", (int)base_len, base_name);
    snprintf(grey_outfile, sizeof(grey_outfile), "%.*s_greyscale.jpg", (int)base_len, base_name);
    snprintf(edge_outfile, sizeof(edge_outfile), "%.*s_edges.jpg", (int)base_len, base_name);
    printf("Processing image: %s\n", infile);
    
    // Image dimensions
    int width, height;
    // color channel count
    int channels;
    
    // 1. Load the input image using stb_image library
    unsigned char *img_data = stbi_load(infile, &width, &height, &channels, 3);
    
    // Check if image loading was successful
    if (!img_data) {
        fprintf(stderr, "Failed to load image '%s'\n", infile);
        return 1;  // Exit with error code
    }
    printf("Loaded image: %dx%d, %d channels\n", width, height, 3);

    // Allocate memory for each stage of image processing
    // Output of median filter
    unsigned char *filtered_rgb = malloc(width * height * 3);
    // Output of greyscale conversion    
    unsigned char *grey_image   = malloc(width * height);
    // Output of Sobel edge detection       
    unsigned char *edge_image   = malloc(width * height);       

    // Check if all memory allocations succeeded
    if (!filtered_rgb || !grey_image || !edge_image) {
        fprintf(stderr, "Failed to allocate memory\n");
        // Free the original image data
        stbi_image_free(img_data);  
        // Free processing buffers
        free(filtered_rgb); 
        free(grey_image); 
        free(edge_image);
        // Exit with error code
        return 1;  
    }

    // 2. Apply Median Filter to reduce noise in the RGB image
    MedianFilter(img_data, filtered_rgb, height, width);
    
    // Save the filtered RGB image
    if (!stbi_write_jpg(filtered_outfile, width, height, 3, filtered_rgb, 90)) {
        fprintf(stderr, "Failed to write filtered RGB image\n");
    } else {
        printf("Filtered RGB image saved to '%s'\n", filtered_outfile);
    }

    // 3. Convert the filtered RGB image to greyscale
    ConvertToGreyscale(filtered_rgb, grey_image, height, width);
    
    // 4. Apply Sobel Edge Detection to detect edges in the greyscale image
    SobelEdgeDetection(grey_image, edge_image, width, height);

    // 5. Save the greyscale image to a file using stb_image_write
    if (!stbi_write_jpg(grey_outfile, width, height, 1, grey_image, 90)) {
        fprintf(stderr, "Failed to write greyscale image\n");
    } else {
        printf("Greyscale image saved to '%s'\n", grey_outfile);
    }

    // Save the edge-detected image to a file
    if (!stbi_write_jpg(edge_outfile, width, height, 1, edge_image, 90)) {
        fprintf(stderr, "Failed to write edge image\n");
    } else {
        printf("Edge-detected image saved to '%s'\n", edge_outfile);
    }

    // Clean up: Free all allocated memory
    stbi_image_free(img_data);   // Free the original image
    free(filtered_rgb);          // Free the filtered RGB image
    free(grey_image);            // Free the greyscale image
    free(edge_image);            // Free the edge-detected image
    
    // Indicate successful program execution
    return 0;  
}

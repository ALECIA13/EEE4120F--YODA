// Standard libraries
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Macros
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // Include the stb_image library for image loading
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h" // Include the stb_image_write library for image writing

// Sets the filter window size for the median filter
#define WINDOW_SIZE 3

// Structure to represent an RGB pixel
typedef struct {
    uint8_t r; // Red component
    uint8_t g; // Green component
    uint8_t b; // Blue component
} RGB;

// Macro to calculate the brightness of an RGB pixel
#define BRIGHTNESS(pixel) ((pixel).r + (pixel).g + (pixel).b)

// Function to apply a median filter to an RGB image
void MedianFilter(
    unsigned char *input, // Pointer to the input image data
    unsigned char *output, // Pointer to the output image data
    int height, int width   // Height and width of the image
) {
    // Iterate over each pixel in the image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Calculate the index of the current pixel in the input image data
            int current_idx = (i * width + j) * 3;

            // Handle border pixels by copying them directly to the output
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                output[current_idx] = input[current_idx];     // Copy red component
                output[current_idx + 1] = input[current_idx + 1]; // Copy green component
                output[current_idx + 2] = input[current_idx + 2]; // Copy blue component
                continue; // Move to the next pixel
            }

            // Create a window to store the neighboring pixels
            RGB window[WINDOW_SIZE * WINDOW_SIZE];
            int idx = 0; // Index for the window array

            // Iterate over the neighbors of the current pixel
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    // Calculate the index of the neighbor pixel in the input image data
                    int neighbor_idx = ((i + di) * width + (j + dj)) * 3;
                    window[idx].r = input[neighbor_idx];     // Store red component of neighbor
                    window[idx].g = input[neighbor_idx + 1]; // Store green component of neighbor
                    window[idx].b = input[neighbor_idx + 2]; // Store blue component of neighbor
                    idx++; // Increment the window index
                }
            }

            // Sort the window based on the brightness of the pixels (Bubble Sort)
            for (int k = 0; k < WINDOW_SIZE * WINDOW_SIZE - 1; k++) {
                for (int l = 0; l < WINDOW_SIZE * WINDOW_SIZE - 1 - k; l++) {
                    if (BRIGHTNESS(window[l]) > BRIGHTNESS(window[l + 1])) {
                        RGB temp = window[l];       // Temporary variable for swapping
                        window[l] = window[l + 1];
                        window[l + 1] = temp;
                    }
                }
            }

            // The median pixel is at the center of the sorted window (index 4 for a 3x3 window)
            output[current_idx] = window[4].r;     // Set the red component of the output pixel to the median red
            output[current_idx + 1] = window[4].g; // Set the green component of the output pixel to the median green
            output[current_idx + 2] = window[4].b; // Set the blue component of the output pixel to the median blue
        }
    }
}

// Function to convert an RGB image to greyscale
void ConvertToGreyscale(
    unsigned char *input,  // Pointer to the input RGB image data
    unsigned char *output, // Pointer to the output greyscale image data
    int height, int width    // Height and width of the image
) {
    // Iterate over each pixel in the image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Calculate the index of the current pixel in the input image data
            int idx = (i * width + j) * 3;
            uint8_t r = input[idx];     // Red component
            uint8_t g = input[idx + 1]; // Green component
            uint8_t b = input[idx + 2]; // Blue component
            // Use standard luminance formula to calculate the greyscale value
            output[i * width + j] = (uint8_t)(0.299 * r + 0.587 * g + 0.114 * b);
        }
    }
}

int main() {
    int width, height, channels;
    // Load the image "uct.jpg"
    unsigned char *img_data = stbi_load("img_uct.jpg", &width, &height, &channels, 3);

    // Check if image loading was successful
    if (img_data == NULL) {
        fprintf(stderr, "Failed to load image\n");
        return 1; // Indicate an error
    }

    // Print the dimensions and number of channels of the loaded image
    printf("Loaded image: %d x %d, %d channels\n", width, height, 3);

    // Allocate memory for the filtered RGB image
    unsigned char *filtered_rgb = (unsigned char *)malloc(width * height * 3);
    // Check if memory allocation was successful
    if (filtered_rgb == NULL) {
        fprintf(stderr, "Failed to allocate memory for filtered RGB image\n");
        stbi_image_free(img_data); // Free the loaded image data
        return 1;                  // Indicate an error
    }

    // Apply the median filter to the loaded image
    MedianFilter(img_data, filtered_rgb, height, width);

    // Allocate memory for the greyscale output image
    unsigned char *grey_output = (unsigned char *)malloc(width * height);
    // Check if memory allocation was successful
    if (grey_output == NULL) {
        fprintf(stderr, "Failed to allocate memory for greyscale image\n");
        stbi_image_free(img_data); // Free the loaded image data
        free(filtered_rgb);       // Free the filtered RGB image data
        return 1;                  // Indicate an error
    }

    // Convert the filtered RGB image to greyscale
    ConvertToGreyscale(filtered_rgb, grey_output, height, width);

    // Write the greyscale image to a JPEG file
    if (!stbi_write_jpg("img_filtered_greyscale.jpg", width, height, 1, grey_output, 90)) {
        fprintf(stderr, "Failed to write greyscale image\n");
        stbi_image_free(img_data); // Free the loaded image data
        free(filtered_rgb);       // Free the filtered RGB image data
        free(grey_output);        // Free the greyscale image data
        return 1;                  // Indicate an error
    }

    // Print a success message
    printf("Median filtered greyscale image saved to 'filtered_greyscale.jpg'\n");

    // Free the allocated memory
    stbi_image_free(img_data);
    free(filtered_rgb);
    free(grey_output);

    return 0; // Indicate successful execution
}
// ==============================================================================================
// Standard libraries
// ==============================================================================================
#include <stdint.h> // Defines integer types
#include <stdio.h> // For I/O operations
#include <stdlib.h> // For memory allocation

// ==============================================================================================
// Single header libraries
// stb_image.h - https://github.com/nothings/stb/blob/master/stb_image.h
// stb_image_write.h - https://github.com/nothings/stb/blob/master/stb_image_write.h
// ==============================================================================================
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // For image loading
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h" // For image writing

// ==============================================================================================
// Constants and Structures
// ==============================================================================================
// Sets the filter window size for the median filter
#define WINDOW_SIZE 3

/**
 * @brief Structure to represent an RGB pixel.
 */
typedef struct {
    uint8_t r; // Red component
    uint8_t g; // Green component
    uint8_t b; // Blue component
} RGB;

// Macro to calculate the brightness of an RGB pixel
#define BRIGHTNESS(pixel) ((pixel).r + (pixel).g + (pixel).b)
// ==============================================================================================
// A: Median Filter - Applies median filter to an RGB image
// ==============================================================================================
/**
 * @brief Applies a median filter to an RGB image.
 *
 * This function smooths the image by replacing each pixel with the median
 * value of its neighbors in a 3x3 window, based on brightness.
 *
 * @param input Pointer to the input image data (RGB).
 * @param output Pointer to the output image data (RGB).
 * @param height Height of the image in pixels.
 * @param width Width of the image in pixels.
 */
void MedianFilter(
    unsigned char *input,  // Pointer to the input image data
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

            // Assign the median pixel (middle of sorted array) to the output
            // For a 3x3 window (9 pixels), the median is at index 4
            output[current_idx]     = window[4].r;  // Red component of median
            output[current_idx + 1] = window[4].g;  // Green component of median
            output[current_idx + 2] = window[4].b;  // Blue component of median
        }
    }
}
// ==============================================================================================
// B: Greyscale Conversion - Converts RGB image to greyscale
// ==============================================================================================
/**
 * @brief Converts an RGB image to greyscale.
 *
 * Uses the standard luminance formula: 0.299 * R + 0.587 * G + 0.114 * B
 *
 * @param input Pointer to the input RGB image data.
 * @param output Pointer to the output greyscale image data.
 * @param height Height of the image in pixels.
 * @param width Width of the image in pixels.
 */
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

/**
 * @brief Main function to load an image, apply median filtering, convert to greyscale,
 * and save both processed versions to disk.
 *
 * @return int 0 on success, 1 on failure.
 */
int main() {
    int width, height, channels;
    // Load the image "img_input.jpg"
    unsigned char *img_data = stbi_load("img_input.jpg", &width, &height, &channels, 3);

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

    // Write the RGB filtered image to a JPEG file
    if (!stbi_write_jpg("img_filtered_rgb.jpg", width, height, 3, filtered_rgb, 90)) {
        fprintf(stderr, "Failed to write RGB filtered image\n");
        stbi_image_free(img_data);
        free(filtered_rgb);
        return 1;
    }

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
        stbi_image_free(img_data);
        free(filtered_rgb);
        free(grey_output);
        return 1;
    }

    // Print a success message
    printf("Filtered RGB image saved to 'img_filtered_rgb.jpg'\n");
    printf("Greyscale image saved to 'img_filtered_greyscale.jpg'\n");

    // Free the allocated memory
    stbi_image_free(img_data);
    free(filtered_rgb);
    free(grey_output);

    return 0; // Indicate successful program execution
}

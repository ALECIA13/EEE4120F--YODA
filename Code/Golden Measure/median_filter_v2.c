// standard libraries
#include <stdint.h>
#include <stdio.h>

// Maximum image width (not yet used)
#define MAX_IMG_WIDTH  320
// Maximum image height (not yet used)
#define MAX_IMG_HEIGHT 240
/// Size of the filter window
#define WINDOW_SIZE 3

/**
 * @struct RGB
 * @brief Represents an RGB pixel with 8-bit channels.
 */
typedef struct {
    uint8_t r; // Red channel
    uint8_t g; // Green channel
    uint8_t b; // Blue channel
} RGB;

// Macro to calculate the sum of RGB channels in a pixel.
#define SUM(pixel) ((pixel).r + (pixel).g + (pixel).b)

/**
 * @brief Applies a 3x3 median filter based on RGB brightness to the input image.
 * 
 * @param input  The input image as a 2D array of RGB pixels.
 * @param output The output (filtered) image as a 2D array of RGB pixels.
 * @param height Height of the image in pixels.
 * @param width  Width of the image in pixels.
 */
 
 
 
 04:38:03
 
 
 
void MedianFilter(
    RGB input[MAX_IMG_HEIGHT][MAX_IMG_WIDTH],
    RGB output[MAX_IMG_HEIGHT][MAX_IMG_WIDTH],
    int height, int width
) {
    // Loop over every pixel in the image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {

            // For edge pixels, directly copy the original pixel
            if (i == 0 || i == height-1 || j == 0 || j == width-1) {
                output[i][j] = input[i][j];
                continue;
            }

            // Collect 3x3 neighborhood of pixels around input[i][j]
            RGB window[9];
            int idx = 0;
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    window[idx++] = input[i+di][j+dj];
                }
            }

            // Sort the 3x3 window based on RGB sum using bubble sort
            for (int k = 0; k < 8; k++) {
                for (int l = 0; l < 8 - k; l++) {
                    if (SUM(window[l]) > SUM(window[l+1])) {
                        RGB temp = window[l];
                        window[l] = window[l+1];
                        window[l+1] = temp;
                    }
                }
            }

            // Assign the median pixel (middle value after sorting) to the output
            output[i][j] = window[4];
        }
    }
}

/**
 * @brief Main function to test the median filter with a 3x3 RGB matrix.
 * 
 * @return int Exit status.
 */
int main() {
    RGB input[MAX_IMG_HEIGHT][MAX_IMG_WIDTH] = {0};  // Original image
    RGB output[MAX_IMG_HEIGHT][MAX_IMG_WIDTH] = {0}; // Filtered image

    // Initialize test image
    input[0][0] = (RGB){2, 4, 7};       // rgbSum: 13
    input[0][1] = (RGB){30, 46, 23};    // rgbSum: 99
    input[0][2] = (RGB){23, 65, 86};    // rgbSum: 174

    input[1][0] = (RGB){34, 57, 3};     // rgbSum: 94
    input[1][1] = (RGB){34, 87, 94};    // rgbSum: 215
    input[1][2] = (RGB){123, 143, 67};  // rgbSum: 333

    input[2][0] = (RGB){23, 43, 197};   // rgbSum: 263
    input[2][1] = (RGB){33, 76, 97};    // rgbSum: 206
    input[2][2] = (RGB){34, 78, 54};    // rgbSum: 166

    // Apply the 3x3 median filter
    MedianFilter(input, output, 3, 3);

    // Print the original RGB image
    printf("Original RGB Image:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            RGB px = input[i][j];
            printf("(%3d,%3d,%3d) ", px.r, px.g, px.b);
        }
        printf("\n");
    }

    // Print sum (R+G+B) of each original pixel
    printf("\nRGB Sum:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%4d ", SUM(input[i][j]));
        }
        printf("\n");
    }

    // Print the filtered (output) RGB image
    printf("\nProcessed Image:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            RGB px = output[i][j];
            printf("(%3d,%3d,%3d) ", px.r, px.g, px.b);
        }
        printf("\n");
    }

    return 0;
}

#include <stdint.h>
#include <stdio.h>

#define MAX_IMG_WIDTH  640
#define MAX_IMG_HEIGHT 480
#define WINDOW_SIZE 3

typedef struct {
    uint8_t r, g, b;
} RGB;

#define BRIGHTNESS(pixel) ((pixel).r + (pixel).g + (pixel).b)

void MedianFilter(
    RGB input[MAX_IMG_HEIGHT][MAX_IMG_WIDTH],
    RGB output[MAX_IMG_HEIGHT][MAX_IMG_WIDTH],
    int height, int width
) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // For edges/corners, just copy the original pixel
            if (i == 0 || i == height-1 || j == 0 || j == width-1) {
                output[i][j] = input[i][j];
                continue;
            }

            // For non-edge pixels, collect 3x3 neighborhood
            RGB window[9];
            int idx = 0;
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    window[idx++] = input[i+di][j+dj];
                }
            }

            // Bubble sort for filter window
            for (int k = 0; k < 8; k++) {
                for (int l = 0; l < 8 - k; l++) {
                    if (BRIGHTNESS(window[l]) > BRIGHTNESS(window[l+1])) {
                        RGB temp = window[l];
                        window[l] = window[l+1];
                        window[l+1] = temp;
                    }
                }
            }

            output[i][j] = window[4]; //Ge  Median from window ,median is at index 4
        }
    }
}

int main() {
    RGB input[MAX_IMG_HEIGHT][MAX_IMG_WIDTH] = {0};
    RGB output[MAX_IMG_HEIGHT][MAX_IMG_WIDTH] = {0};

    // Test image (3x3)
    input[0][0] = (RGB){7, 5,8};
    input[0][1] = (RGB){30, 46, 23};
    input[0][2] = (RGB){0, 0, 0};
    input[1][0] = (RGB){34, 57, 33};
    input[1][1] = (RGB){0,0,0};
    input[1][2] = (RGB){0, 0,0};
    input[2][0] = (RGB){34,78,54};
    input[2][1] = (RGB){0,0,0};
    input[2][2] = (RGB){2,4,7};

    MedianFilter(input, output, 3, 3);

    printf("Original image:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("(%3d, %3d, %3d) ", input[i][j].r, input[i][j].g, input[i][j].b);
        }
        printf("\n");
    }

    printf("\nProcessed image:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("(%3d, %3d, %3d) ", output[i][j].r, output[i][j].g, output[i][j].b);
        }
        printf("\n");
    }

    return 0;
}
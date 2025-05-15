// standard input-output library
#include <stdio.h>
// standard library
#include <stdlib.h>   
// for uint16_t and uint32_t types
#include <stdint.h>
#include <string.h> // for memset and memcpy

// image dimensions
#define WIDTH  320
#define HEIGHT 240

/**
 * @brief Sort an array of 9 integers in ascending order using bubble sort.
 * @param arr Array of 9 integers to sort.
 */
void bubblesort(int arr[9]) {
    int i, j, temp;
    // outer loop: iterates through elements
    for (i = 0; i < 8; i++) {
        // inner loop: compare elements
        for (j = i + 1; j < 9; j++) {
            // out of order? 
            // yes: SWAP elements
            if (arr[i] > arr[j]) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

// BMP file header structure (14 bytes)
#pragma pack(push, 1)
typedef struct {
    uint16_t signature;    // Magic identifier: 0x4d42
    uint32_t fileSize;     // File size in bytes
    uint16_t reserved1;    // Not used
    uint16_t reserved2;    // Not used
    uint32_t dataOffset;   // Offset to image data in bytes from beginning of file
} BMPFileHeader;

// BMP information header (40 bytes)
typedef struct {
    uint32_t headerSize;          // DIB Header size (40 bytes)
    int32_t  width;               // Width of the image
    int32_t  height;              // Height of the image
    uint16_t planes;              // Number of color planes
    uint16_t bitsPerPixel;        // Bits per pixel
    uint32_t compression;         // Compression type
    uint32_t imageSize;           // Image size in bytes
    int32_t  xPixelsPerMeter;     // Pixels per meter
    int32_t  yPixelsPerMeter;     // Pixels per meter
    uint32_t colorsUsed;          // Number of colors  
    uint32_t colorsImportant;     // Important colors 
} BMPInfoHeader;

// The complete BMP header
typedef struct {
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
} BMPHeader;
#pragma pack(pop)

/**
 * @brief Main function to load an image, apply median filtering, and save result.
 *
 * @return int 0 on success.
 */
int main() {
    FILE *fp_in, *fp_out;
    BMPHeader header;
    unsigned char *imageData = NULL;
    int input_image[HEIGHT][WIDTH][3];
    int sum_image[HEIGHT][WIDTH];
    int output_image[HEIGHT][WIDTH];
    int i, j;
    
    // Calculate row padding for both 24-bit and 32-bit BMPs
    int paddingBytes24 = (4 - (WIDTH * 3) % 4) % 4;
    int paddingBytes32 = 0; // No padding needed for 32-bit BMPs
    int rowSize24 = WIDTH * 3 + paddingBytes24;
    int rowSize32 = WIDTH * 4; // 4 bytes per pixel

    // Open input BMP image in read binary mode
    fp_in = fopen("img_input.bmp", "rb");
    if (!fp_in) {
        printf("Error: Cannot open input.bmp\n");
        return 1;
    }

    // Read BMP header
    if (fread(&header, sizeof(BMPHeader), 1, fp_in) != 1) {
        printf("Error: Could not read BMP header\n");
        fclose(fp_in);
        return 1;
    }
    
    // Verify file is a BMP file
    if (header.fileHeader.signature != 0x4D42) { // "BM" in little endian
        printf("Error: Not a valid BMP file (signature: 0x%X)\n", header.fileHeader.signature);
        fclose(fp_in);
        return 1;
    }

    // Get BMP details
    int width = header.infoHeader.width;
    int height = header.infoHeader.height;
    int bitsPerPixel = header.infoHeader.bitsPerPixel;
    
    // Check for compatibility
    if (width != WIDTH || height != HEIGHT) {
        printf("Error: Image dimensions do not match expected %dx%d (got %dx%d)\n", 
               WIDTH, HEIGHT, width, height);
        fclose(fp_in);
        return 1;
    }
    
    // Print BMP information for debugging
    printf("BMP Info: %dx%d, %d-bit\n", width, height, bitsPerPixel);
    printf("Data Offset: %u bytes\n", header.fileHeader.dataOffset);
    
    // Check bit depth and set appropriate row size
    int rowSize = 0;
    if (bitsPerPixel == 24) {
        printf("Processing 24-bit BMP\n");
        rowSize = rowSize24;
    } else if (bitsPerPixel == 32) {
        printf("Processing 32-bit BMP\n");
        rowSize = rowSize32;
    } else {
        printf("Error: Only 24-bit and 32-bit BMP files are supported (got %d-bit)\n", bitsPerPixel);
        fclose(fp_in);
        return 1;
    }

    // Allocate memory for image data
    int imageSize = rowSize * height;
    imageData = (unsigned char*)malloc(imageSize);
    if (!imageData) {
        printf("Error: Memory allocation failed\n");
        fclose(fp_in);
        return 1;
    }
    
    // Seek to the beginning of pixel data
    if (fseek(fp_in, header.fileHeader.dataOffset, SEEK_SET) != 0) {
        printf("Error: Could not seek to pixel data\n");
        free(imageData);
        fclose(fp_in);
        return 1;
    }
    
    // Read the entire image data
    if (fread(imageData, 1, imageSize, fp_in) != imageSize) {
        printf("Error: Could not read image data\n");
        free(imageData);
        fclose(fp_in);
        return 1;
    }
    
    fclose(fp_in);
    
    // Process the image data into our 3D array
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            int pixelPos;
            
            // Calculate position in the image data array
            if (bitsPerPixel == 24) {
                pixelPos = (height - 1 - i) * rowSize + j * 3;
                // Extract BGR values (24-bit)
                input_image[i][j][2] = imageData[pixelPos];     // B
                input_image[i][j][1] = imageData[pixelPos + 1]; // G
                input_image[i][j][0] = imageData[pixelPos + 2]; // R
            } else { // 32-bit
                pixelPos = (height - 1 - i) * rowSize + j * 4;
                // Extract BGRA values (32-bit)
                input_image[i][j][2] = imageData[pixelPos];     // B
                input_image[i][j][1] = imageData[pixelPos + 1]; // G
                input_image[i][j][0] = imageData[pixelPos + 2]; // R
                // Alpha channel is at pixelPos + 3, but we ignore it
            }
        }
    }
    
    // Free the original image data as we've stored it in our array
    free(imageData);
    
    // Sum the RGB channels
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            sum_image[i][j] = input_image[i][j][0] + input_image[i][j][1] + input_image[i][j][2];
        }
    }

    // Apply 3x3 median filter to non-border pixels
    for (i = 1; i < height - 1; i++) {
        for (j = 1; j < width - 1; j++) {
            int window[9];
            int idx = 0;
            
            // Gather neighbors
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    window[idx++] = sum_image[i + di][j + dj];
                }
            }
            
            // Sort and take median
            bubblesort(window);
            output_image[i][j] = window[4];
        }
    }

    // Create a new 24-bit BMP file for output
    // This is more compatible than 32-bit
    fp_out = fopen("img_output.bmp", "wb");
    if (!fp_out) {
        printf("Error: Cannot create output.bmp\n");
        return 1;
    }
    
    // Create a new header for the output file (24-bit grayscale)
    BMPHeader outHeader;
    memset(&outHeader, 0, sizeof(BMPHeader));
    
    // File header
    outHeader.fileHeader.signature = 0x4D42; // "BM"
    outHeader.fileHeader.dataOffset = sizeof(BMPHeader);
    outHeader.fileHeader.fileSize = sizeof(BMPHeader) + height * rowSize24;
    
    // Info header
    outHeader.infoHeader.headerSize = sizeof(BMPInfoHeader);
    outHeader.infoHeader.width = width;
    outHeader.infoHeader.height = height;
    outHeader.infoHeader.planes = 1;
    outHeader.infoHeader.bitsPerPixel = 24; // 24-bit for compatibility
    outHeader.infoHeader.compression = 0;   // No compression
    outHeader.infoHeader.imageSize = height * rowSize24;
    outHeader.infoHeader.xPixelsPerMeter = 2835; // 72 DPI
    outHeader.infoHeader.yPixelsPerMeter = 2835; // 72 DPI
    outHeader.infoHeader.colorsUsed = 0;
    outHeader.infoHeader.colorsImportant = 0;
    
    // Write the header to output file
    if (fwrite(&outHeader, sizeof(BMPHeader), 1, fp_out) != 1) {
        printf("Error: Could not write header to output file\n");
        fclose(fp_out);
        return 1;
    }
    
    // Allocate memory for output image data
    unsigned char *outputData = (unsigned char*)malloc(height * rowSize24);
    if (!outputData) {
        printf("Error: Memory allocation failed for output data\n");
        fclose(fp_out);
        return 1;
    }
    memset(outputData, 0, height * rowSize24); // Initialize to zero
    
    // Fill output data
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            int pixelPos = (height - 1 - i) * rowSize24 + j * 3;
            unsigned char gray;
            
            // Determine pixel value
            if (i == 0 || j == 0 || i == height-1 || j == width-1) {
                // Border pixels: use standard grayscale conversion
                gray = (unsigned char)(0.299 * input_image[i][j][0] + 
                                      0.587 * input_image[i][j][1] + 
                                      0.114 * input_image[i][j][2]);
            } else {
                // Non-border pixels: use median filtered value
                gray = (unsigned char)(output_image[i][j] / 3.0 + 0.5);
            }
            
            // Set BGR values to the same grayscale value
            outputData[pixelPos] = gray;     // B
            outputData[pixelPos + 1] = gray; // G
            outputData[pixelPos + 2] = gray; // R
        }
        
        // Add padding bytes at the end of the row
        for (j = 0; j < paddingBytes24; j++) {
            outputData[(height - 1 - i) * rowSize24 + width * 3 + j] = 0;
        }
    }
    
    // Write image data to file
    if (fwrite(outputData, 1, height * rowSize24, fp_out) != height * rowSize24) {
        printf("Error: Could not write image data to output file\n");
        free(outputData);
        fclose(fp_out);
        return 1;
    }
    
    // Clean up
    free(outputData);
    fclose(fp_out);
    
    printf("Median filtering complete. Output saved to output.bmp\n");
    return 0;
}
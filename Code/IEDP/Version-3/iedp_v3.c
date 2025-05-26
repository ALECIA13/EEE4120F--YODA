/** 
 * To compile: gcc -fdiagnostics-color=always -g "iedp_gui.c" -o "iedp_gui.exe" -lgdi32 -luser32 -lcomdlg32 -lcomctl32
 */

/**
 * @file iedp_gui.c
 * @brief Image processing pipeline with Windows GUI - Median Filter → Greyscale → Sobel Edge Detection
 */

// ======================================================================================================================
// Standard libraries
// ======================================================================================================================
#include <stdint.h> // Defines integer types
#include <stdio.h> // For I/O operations
#include <stdlib.h> // For memory allocation
#include <math.h> // For mathematical operations
#include <time.h> // For timing operations
#include <windows.h> // For core Windows API functions
#include <commdlg.h> // For file dialog functions
#include <commctrl.h> // For common controls

// ======================================================================================================================
// Single header libraries
// stb_image.h - https://github.com/nothings/stb/blob/master/stb_image.h
// stb_image_write.h - https://github.com/nothings/stb/blob/master/stb_image_write.h
// ======================================================================================================================
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// ======================================================================================================================
// Constants and Structures
// ======================================================================================================================
#define WINDOW_SIZE 3 // Window size for the median filter (3 x 3 kernel)
#define WND_CLASS_NAME "IEDPWindowClass" // Name of the window class for the GUI
#define WINDOW_WIDTH 1200 // Width of the window
#define WINDOW_HEIGHT 900 // Height of the window
#define ID_LOAD_BUTTON 101 // Unique ID for the load button
#define ID_PROCESS_BUTTON 102 // Unique ID for the process button
#define ID_SAVE_BUTTON 103 // Unique ID for the save button
#define ID_STATUS_BAR 104 // Unique ID for the status bar
#define ID_PROGRESS 105 // Unique ID for the progress bar

/**
 * @brief Structure representing an RGB pixel
 */
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB;

/**
 * @brief Structure to hold the application's state
 */
typedef struct {
    // HWND handles for GUI elements
    HWND window;
    HWND load_button;
    HWND process_button;
    HWND save_button;
    HWND status_bar;
    HWND progress_bar;
    HWND metrics_label;
    // Pointer to the original image data
    unsigned char *original_img;
    int width;
    int height;
    // Pointers to the processed image data 
    unsigned char *filtered_rgb;
    unsigned char *grey_image;
    unsigned char *edge_image;
    // For Windows bitmaps
    HBITMAP original_bmp;
    HBITMAP filtered_bmp;
    HBITMAP grey_bmp;
    HBITMAP edge_bmp;
    // Timing metrics
    double load_time;
    double median_time;
    double grey_time;
    double edge_time;
    double total_time;
} AppData;
// Pointer allocated in WinMain to store application state
AppData *g_app = NULL;

// ======================================================================================================================
// Macro
// ======================================================================================================================
// Compute the sum of RGB components
#define BRIGHTNESS(p) ((p).r + (p).g + (p).b)

// ======================================================================================================================
// A: Median Filter - Applies median filter to an RGB image
// ======================================================================================================================
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
            
            // Skip edge pixels
            if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
                output[current_idx] = input[current_idx];
                output[current_idx + 1] = input[current_idx + 1];
                output[current_idx + 2] = input[current_idx + 2];
                continue;
            }
            // Array to store the 3x3 window of pixels around the current position
            RGB window[WINDOW_SIZE * WINDOW_SIZE];
            // Index counter for the window array
            int idx = 0; 
            
            // Collect the 3x3 neighborhood of pixels around the current pixel
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    // Calculate the index of the neighbor pixel
                    int neighbor_idx = ((y + dy) * width + (x + dx)) * 3;
                    // Store RGB values in the window array 
                    window[idx].r = input[neighbor_idx];
                    window[idx].g = input[neighbor_idx + 1];
                    window[idx].b = input[neighbor_idx + 2];
                    idx++;
                }
            }

            // Sort the pixels in ascending order of brightness using bubble sort
            for (int k = 0; k < WINDOW_SIZE * WINDOW_SIZE - 1; k++) {
                for (int l = 0; l < WINDOW_SIZE * WINDOW_SIZE - 1 - k; l++) {
                    // Compare brightness of adjacent pixels
                    if (BRIGHTNESS(window[l]) > BRIGHTNESS(window[l + 1])) {
                        // Out of order? SWAP
                        RGB temp = window[l];
                        window[l] = window[l + 1];
                        window[l + 1] = temp;
                    }
                }
            }
            // Assign the median pixel (middle of sorted array) to the output
            output[current_idx] = window[4].r;
            output[current_idx + 1] = window[4].g;
            output[current_idx + 2] = window[4].b;
        }
    }
}

// ======================================================================================================================
// B: Greyscale Conversion - Converts RGB image to greyscale
// ======================================================================================================================
/**
 * @brief Converts an RGB image to greyscale.
 *
 * @param input  Pointer to input RGB image data
 * @param output Pointer to output greyscale image data
 * @param height Image height
 * @param width  Image width
 */
void ConvertToGreyscale(unsigned char *input, unsigned char *output, int height, int width) {
    // Process each pixel in the image
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Calculate the index of the current pixel in the input RGB image 
            int idx = (y * width + x) * 3;
            // Extract individual RGB component values
            uint8_t r = input[idx];
            uint8_t g = input[idx + 1];
            uint8_t b = input[idx + 2];
            // Convert RGB to greyscale using standard luminance formula 
            output[y * width + x] = (uint8_t)(0.299 * r + 0.587 * g + 0.114 * b);
        }
    }
}

// ======================================================================================================================
// C: Sobel Edge Detection - Detects edges in greyscale image
// ======================================================================================================================
/**
 * @brief Applies Sobel edge detection to a greyscale image.
 *
 * @param grey  Pointer to input greyscale image data
 * @param edges Pointer to output edge image data
 * @param width Image width
 * @param height Image height
 */
void SobelEdgeDetection(unsigned char *grey, unsigned char *edges, int width, int height) {
    // Sobel operator kernels
    // gx detects horizontal edges (vertical gradients)
    static const int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    // gy detects vertical edges (horizontal gradients)
    static const int gy[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
    
    // Set all border pixels to 0
    // Set top and bottom row borders to zero
    for (int x = 0; x < width; x++) {
        edges[x] = 0;                        
        edges[(height - 1) * width + x] = 0; 
    }
    
    // Set left and right column borders to zero
    for (int y = 0; y < height; y++) {
        edges[y * width] = 0;            
        edges[y * width + (width - 1)] = 0;  
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
            // Check if gradient magnitude is within the valid range [0, 255]
            if (magnitude > 255) magnitude = 255;
            // Assign the magnitude to the output edge image
            edges[y * width + x] = (unsigned char)(magnitude);
        }
    }
}

// ======================================================================================================================
// Save Processed Images
// ======================================================================================================================
/**
 * @brief Saves the processed images (median filtered, greyscale, edge) to PNG files.
 */

void save_images() {
    // Check if there are processed images to save
    if (!g_app->filtered_rgb || !g_app->grey_image || !g_app->edge_image) {
        MessageBox(g_app->window, "No processed images to save", "Error", MB_ICONERROR);
        return;
    }
        
    OPENFILENAME ofn; // Declares an OPENFILENAME struct used by Windows to configure and manage file dialogs
    char filename[MAX_PATH] = ""; // Buffer for filename
    ZeroMemory(&ofn, sizeof(ofn)); // Sets all bytes of the ofn structure to 0
    ofn.lStructSize = sizeof(ofn); // Tells the Windows API how large the structure is
    ofn.hwndOwner = g_app->window; // Sets the window handle (HWND) of the owner window
    ofn.lpstrFile = filename; // Tells the dialog where to store the filepath the user selects
    ofn.nMaxFile = sizeof(filename); // Tells the dialog hwo big the filename buffer is
    ofn.lpstrFilter = "PNG Files\0*.png\0All Files\0*.*\0"; // File type filter
    ofn.nFilterIndex = 1; // Default filter
    // Flags
    // OFN_PATHMUSTEXIST: Ensures the user can only select a file in an existing path.
    // OFN_OVERWRITEPROMPT: Prompts the user with a confirmation dialog if the selected file already exists.
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT; 
    ofn.lpstrDefExt = "png"; // Default file extension 
    
    // Opens the Save File dialog
    // If the iser selects a file and confirms it, it returns TRUE and proceeds
    // If the user cancels, it skips the block 
    if (GetSaveFileName(&ofn)) {
        // Sends a Windows message to update the status bar
        // SB_SRTTEXY: Sets the text of a status bar panel.
        // Displays "Saving images" while the saving process runs
        SendMessage(g_app->status_bar, SB_SETTEXT, 0, (LPARAM)"Saving images...");
        
        // Declares three buffers to hold the file paths for the output images
        char filtered_path[MAX_PATH], grey_path[MAX_PATH], edge_path[MAX_PATH];

        // Build the filenames by appending suffixes to the base filename
        snprintf(filtered_path, sizeof(filtered_path), "%s_filtered.png", filename);
        snprintf(grey_path, sizeof(grey_path), "%s_grey.png", filename);
        snprintf(edge_path, sizeof(edge_path), "%s_edge.png", filename);

        // Saves the filtered RGB image with 3 channels
        // g_app->filtered_rgb: pointer to image pixel data
        // g_app->width * 3: width * 3 bytes per pixel
        stbi_write_png(filtered_path, g_app->width, g_app->height, 3, g_app->filtered_rgb, g_app->width * 3);
        // Saves the greyscale image with 1 channel
        // g_app->grey_image: greyscale image pixel data 
        stbi_write_png(grey_path, g_app->width, g_app->height, 1, g_app->grey_image, g_app->width);
        // Saves the greyscale image with 1 channel
        // g_app->edge_image: edge detection image pixel data 
        stbi_write_png(edge_path, g_app->width, g_app->height, 1, g_app->edge_image, g_app->width);

        //  Updates the status bar to inform the user: "Images saved successfully" 
        SendMessage(g_app->status_bar, SB_SETTEXT, 0, (LPARAM)"Images saved successfully");
    }
}


// ======================================================================================================================
// Create Bitmap from Image Data
// ======================================================================================================================
/**
 * @brief Creates a Windows HBITMAP from raw image data.
 * 
 * @param data Raw image data
 * @param width Width of the image.
 * @param height Height of the image.
 * @param channels Number of color channels
 * @return HBITMAP handle to the created bitmap, or NULL on failure.
 */
HBITMAP create_bitmap_from_data(unsigned char *data, int width, int height, int channels) {
    HDC hdc = GetDC(NULL); // Get a device context (DC) for the entire screen
    HDC memDC = CreateCompatibleDC(hdc); // Create a memory-based DC compatible with the screen
    
    BITMAPINFO bmi; // Declares a BITMAPINFO struct used to describe the dimensions, format, and layout of the bitmap
    ZeroMemory(&bmi, sizeof(BITMAPINFO)); // Clears all field in bmi by setting all bytes to zero
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); // Sets the size of the BITMAPINFOHEADER
    bmi.bmiHeader.biWidth = width; // Sets the width of the bitmap
    bmi.bmiHeader.biHeight = -height; // Sets the height of the bitmap (-ve: Top-down bitmap)
    bmi.bmiHeader.biPlanes = 1; 
    bmi.bmiHeader.biBitCount = 24; // Sets the color depth 
    bmi.bmiHeader.biCompression = BI_RGB; // Specifies the compression method (BI_RGB = no compression)

    unsigned char *rgb_data = malloc(width * height * 3);
    // Checks if memory allocation failed
    // Yes: Report error: "Memory allocation failed for RGB buffer"
    if (!rgb_data) {
        MessageBox(NULL, "Memory allocation failed for RGB buffer", "Error", MB_ICONERROR);
        DeleteDC(memDC); // Deletes memory DC
        ReleaseDC(NULL, hdc); // Releases screen DC
        // Returns NULL
        return NULL;
    }

    // Greyscale to RGB
    if (channels == 1) {
        for (int i = 0; i < width * height; i++) {
            rgb_data[i * 3] = rgb_data[i * 3 + 1] = rgb_data[i * 3 + 2] = data[i];
        }
    } 
    // RGB conversion: Swaps red and blue
    else if (channels == 3) {
        for (int i = 0; i < width * height; i++) {
            rgb_data[i * 3] = data[i * 3 + 2];
            rgb_data[i * 3 + 1] = data[i * 3 + 1];
            rgb_data[i * 3 + 2] = data[i * 3];
        }
    } else {
        MessageBox(NULL, "Unsupported channel count", "Error", MB_ICONERROR);
        DeleteDC(memDC); // Deletes memory DC
        ReleaseDC(NULL, hdc); // Releases screen DC
        free(rgb_data); // Frees RGB buffer
        // Returns NULL (for unsupported formats)
        return NULL;
    }

    void *bits;
    // CreateDIBSection allocates a device-independent bitmap
    // bits points to the memory where pixel data should be copied
    HBITMAP bitmap = CreateDIBSection(memDC, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
    // Checks if bimap creation was successful
    // Yes: Copy pixel data into the memory used by the bitmap
    // No: Report error "Failed to create DIB section"
    if (bitmap && bits) { 
        memcpy(bits, rgb_data, width * height * 3);
    } else {
        MessageBox(NULL, "Failed to create DIB section", "Error", MB_ICONERROR);
    }

    DeleteDC(memDC); // Deletes memory DC
    ReleaseDC(NULL, hdc); // Releases screen DC
    free(rgb_data); // Frees RGB buffer

    // Returns the HBITMAP ready for drawing
    return bitmap;
}

// ======================================================================================================================
// Update Metrics Text
// ======================================================================================================================
/**
 * @brief Updates the metrics label in the GUI with timing information.
 *        Displays either image dimensions and load time, or processing times.
 */
void update_metrics_text() {
    char metrics[512]; // Declares a buffer to hold the formatted string that will be displayed in the UI
    
    // Checks if an original image is loaded
    // Yes: Displays confirmation output ("Image Loaded") and load time in milliseconds
    if (g_app->original_img && !g_app->filtered_rgb) {
        // Populate the metrics string with image dimensions and the load time
        snprintf(metrics, sizeof(metrics), 
                "Image loaded: %dx%d\nLoad time: %.3f ms", 
                g_app->width, g_app->height, g_app->load_time);
    } 
    // Checks if image filtering has been performed
    else if (g_app->filtered_rgb) {
        // Populate the metrics string with times for each processing stage
        snprintf(metrics, sizeof(metrics), 
                "Processing times:\n"
                "Median Filter: %.3f ms\n"
                "Greyscale: %.3f ms\n"
                "Edge Detection: %.3f ms\n"
                "Total: %.3f ms",
                g_app->median_time, g_app->grey_time, g_app->edge_time, g_app->total_time);
    } 
    // If no image has been loaded at all, displays a default message: "No image Loaded"
    else {
        strcpy(metrics, "No image loaded");
    }
    
    // Sets the text of the metrrics label in the GUI window to the metrics string
    SetWindowText(g_app->metrics_label, metrics);
}

// ======================================================================================================================
// Loads an Image
// ======================================================================================================================
/**
 * @brief Loads an image from a file selected by the user.
 *        Uses stb_image to load, checks dimensions, and updates GUI.
 */
void load_image() {
    OPENFILENAME ofn; // Declares an OPENFILENAME struct
    char filename[MAX_PATH] = ""; // Buffer to store the selected file's path
    
    ZeroMemory(&ofn, sizeof(ofn)); // Clears all fielsds of the ofn structure 
    ofn.lStructSize = sizeof(ofn); // Sets the size of the structure
    ofn.hwndOwner = g_app->window; // Sets the owner of the dialog box to app's main window
    ofn.lpstrFile = filename; // Points lpstrFile to the filename buffer
    ofn.nMaxFile = sizeof(filename); // Tells the dialog how big the buffer is
    ofn.lpstrFilter = "Image Files\0*.jpg;*.jpeg;*.png;*.bmp\0All Files\0*.*\0"; // Sets the filters for image types and all files
    ofn.nFilterIndex = 1; // Default filter
    // Flags
    // OFN_PATHMUSTEXIST: Ensures the user can only select a file in an existing path.
    // OFN_OVERWRITEPROMPT: Prompts the user with a confirmation dialog if the selected file already exists.
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    
    // Temporarily update the status bar to show that an image is being loaded
    SendMessage(g_app->status_bar, SB_SETTEXT, 0, (LPARAM)"Loading image...");
    
    if (GetOpenFileName(&ofn)) {
        clock_t start = clock();
        unsigned char *new_image = stbi_load(filename, &g_app->width, &g_app->height, NULL, 3);
        clock_t end = clock();
        g_app->load_time = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC; // Convert to milliseconds
        
        // Checks if image loading succeeded
        if (new_image) {
            // Check if image dimensions are 320x240
            if (g_app->width != 320 || g_app->height != 240) {
                stbi_image_free(new_image); // Frees the image buffer
                // Notifies the user via the status bar and dialog
                SendMessage(g_app->status_bar, SB_SETTEXT, 0, (LPARAM)"Invalid image size");
                MessageBox(g_app->window, "Please load an image with dimensions 320x240 pixels", "Invalid Image Size", MB_ICONERROR);
                // Exits the function
                return;
            }
            
            // Free previous image data and bitmaps if they exist
            if (g_app->original_img) stbi_image_free(g_app->original_img);
            if (g_app->filtered_rgb) free(g_app->filtered_rgb);
            if (g_app->grey_image) free(g_app->grey_image);
            if (g_app->edge_image) free(g_app->edge_image);
            // Deletes previous Windows bitmap objects to release GDI resources
            if (g_app->original_bmp) DeleteObject(g_app->original_bmp);
            if (g_app->filtered_bmp) DeleteObject(g_app->filtered_bmp);
            if (g_app->grey_bmp) DeleteObject(g_app->grey_bmp);
            if (g_app->edge_bmp) DeleteObject(g_app->edge_bmp);
            
            g_app->original_img = new_image; // Stores the newly loaded image in the global app state
            // Creates Windows bitmaps to display original image and filtered image
            g_app->original_bmp = create_bitmap_from_data(g_app->original_img, g_app->width, g_app->height, 3);
            g_app->filtered_bmp = create_bitmap_from_data(g_app->original_img, g_app->width, g_app->height, 3);
            
            // Updates the GUI label with load time and image info
            update_metrics_text();
            
            // Enables the "Process" button
            EnableWindow(g_app->process_button, TRUE);
            // Disables the "Save" button until processing is complete
            EnableWindow(g_app->save_button, FALSE);
            
            // Update the status bar
            SendMessage(g_app->status_bar, SB_SETTEXT, 0, (LPARAM)"Image loaded successfully");
            // Forces the window to redraw and display the loaded window
            InvalidateRect(g_app->window, NULL, TRUE);
        } 
        // if image loading fails
        // Inform the user via both status bar and messagebox
        else {
            SendMessage(g_app->status_bar, SB_SETTEXT, 0, (LPARAM)"Failed to load image");
            MessageBox(g_app->window, "Failed to load image", "Error", MB_ICONERROR);
        }
    }
}
// ======================================================================================================================
// Processes the Loaded Image
// ======================================================================================================================
/**
 * @brief Processes the loaded image through the pipeline: Median Filter -> Greyscale -> Sobel.
 *        Updates GUI elements including progress bar and metrics.
 */
void process_image() {
    // Checks if an image is loaded
    // No: Displays an error via staus bar and message box, then exits the function
    if (!g_app->original_img) {
        SendMessage(g_app->status_bar, SB_SETTEXT, 0, (LPARAM)"No image loaded");
        MessageBox(g_app->window, "No image loaded", "Error", MB_ICONERROR);
        return;
    }
    // Updates the status bar to indicate processing has started
    SendMessage(g_app->status_bar, SB_SETTEXT, 0, (LPARAM)"Processing image...");
    // Sets the prgress bar range from 0 to 100
    SendMessage(g_app->progress_bar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
    // Each step will increment it by 33
    SendMessage(g_app->progress_bar, PBM_SETSTEP, (WPARAM)33, 0);
    // Displays the progress bar
    ShowWindow(g_app->progress_bar, SW_SHOW);
    
    // Allocates memory for the processed images
    g_app->filtered_rgb = malloc(g_app->width * g_app->height * 3);
    g_app->grey_image = malloc(g_app->width * g_app->height);
    g_app->edge_image = malloc(g_app->width * g_app->height);
    
    // Checks if memory allocation failed
    // No: Displays an error via staus bar and message box, then exits the function
    if (!g_app->filtered_rgb || !g_app->grey_image || !g_app->edge_image) {
        SendMessage(g_app->status_bar, SB_SETTEXT, 0, (LPARAM)"Memory allocation failed");
        MessageBox(g_app->window, "Memory allocation failed", "Error", MB_ICONERROR);
        return;
    }
    
    // Start timer (Total Processing Time)
    clock_t start_total = clock();
    
    // Median Filter
    // Sets and advances the progress bar by 1 step
    SendMessage(g_app->progress_bar, PBM_SETSTEP, (WPARAM)33, 0);
    SendMessage(g_app->progress_bar, PBM_STEPIT, 0, 0);
    // Starts timer (Median Filter Processing Time)
    clock_t start = clock();
    // Calls the MedianFilter() function on the original image
    MedianFilter(g_app->original_img, g_app->filtered_rgb, g_app->height, g_app->width);
    // Measures the time taken
    clock_t end = clock();
    // Stores time taken in g_app->median_time 
    g_app->median_time = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC; // Convert to milliseconds
    
    // Greyscale Conversion
    // Advances the progress bar by 1 step
    SendMessage(g_app->progress_bar, PBM_STEPIT, 0, 0);
    // Starts timer (Greyscale Conversion Processing Time)
    start = clock();
    // Calls ConverToGreyscale() function on median filtered image 
    ConvertToGreyscale(g_app->filtered_rgb, g_app->grey_image, g_app->height, g_app->width);
    // Measures the time taken
    end = clock();
    // Stores time taken in g_app->grey_time 
    g_app->grey_time = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC; // Convert to milliseconds
    
    // Sobel Edge Detection
    // Final progress step
    SendMessage(g_app->progress_bar, PBM_STEPIT, 0, 0);
    // Starts timer (Sobel Edge Detection Processing Time)
    start = clock();
    // Calls SobelEdgeDetection() function on greyscale image
    SobelEdgeDetection(g_app->grey_image, g_app->edge_image, g_app->width, g_app->height);
    // Measures the time taken
    end = clock();
    // Stores time taken in g_app->edge_time 
    g_app->edge_time = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC; // Convert to milliseconds
    
    // Measures total processing time
    clock_t end_total = clock();
    // Stores time taken in g_app->total_time
    g_app->total_time = (double)(end_total - start_total) * 1000.0 / CLOCKS_PER_SEC; // Convert to milliseconds
    
    // Converts all processed image buffers into Windows-compatible HBITMAPs for display
    g_app->filtered_bmp = create_bitmap_from_data(g_app->filtered_rgb, g_app->width, g_app->height, 3);
    g_app->grey_bmp = create_bitmap_from_data(g_app->grey_image, g_app->width, g_app->height, 1);
    g_app->edge_bmp = create_bitmap_from_data(g_app->edge_image, g_app->width, g_app->height, 1);
    
    // Updates the text labe will all processing times
    update_metrics_text();
    // Enables the "Save" button 
    EnableWindow(g_app->save_button, TRUE);
    // Hides the progress bar
    ShowWindow(g_app->progress_bar, SW_HIDE);
    // Updates the status par
    SendMessage(g_app->status_bar, SB_SETTEXT, 0, (LPARAM)"Image processing completed");
    // Foces the window to redraw
    InvalidateRect(g_app->window, NULL, TRUE);
}

// ======================================================================================================================
// Draws an HBITMAP 
// ======================================================================================================================
/**
 * @brief Draws an HBITMAP into a specified RECT on a device context.
 *        The bitmap is scaled to fit the rectangle while maintaining aspect ratio.
 * 
 * @param hdc The device context to draw on.
 * @param bitmap The HBITMAP to draw.
 * @param rect Pointer to the RECT defining the drawing area.
 */
void draw_image(HDC hdc, HBITMAP bitmap, RECT *rect) {
    // If no bitmap is provided, exits the function
    if (!bitmap) return;
    
    // Creates a memory device context compatible withe the destination DC (hdc) to hold the bitmap
    HDC memDC = CreateCompatibleDC(hdc); 
    HBITMAP oldBitmap = SelectObject(memDC, bitmap); // Selects the bitmap into memDC
    BITMAP bm;
    GetObject(bitmap, sizeof(BITMAP), &bm); // Retrieves the properties of the bitmap
    
    // Rectangel dimensions
    double rect_width = rect->right - rect->left;
    double rect_height = rect->bottom - rect->top;
    // Calculates the scaling factor to fit the bitmap with the provided rectangle
    double scale = min(rect_width / bm.bmWidth, rect_height / bm.bmHeight);
    
    // Computes the final scaled width, height, and centered position within the rectangle.
    int dest_width = (int)(bm.bmWidth * scale);
    int dest_height = (int)(bm.bmHeight * scale);
    int dest_x = rect->left + (rect_width - dest_width) / 2;
    int dest_y = rect->top + (rect_height - dest_height) / 2;
    
    // Sets high-quality stretch mode for image scaling.
    SetStretchBltMode(hdc, HALFTONE);
    // Performs the drawing, scaling the bitmap to the calculated dimensions.
    StretchBlt(hdc, dest_x, dest_y, dest_width, dest_height, 
              memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
    // Selects the previous bitmap
    SelectObject(memDC, oldBitmap);
    // Deletes the temporary memory DC
    DeleteDC(memDC);
}

// ======================================================================================================================
// Main Message Handler
// ======================================================================================================================
/**
 * @brief Window procedure for handling messages sent to the main window.
 * 
 * @param hwnd Handle to the window.
 * @param uMsg Message identifier.
 * @param wParam Additional message information.
 * @param lParam Additional message information.
 * @return LRESULT Result of message processing.
 */
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            // Initialize common controls
            INITCOMMONCONTROLSEX icex;
            icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
            icex.dwICC = ICC_PROGRESS_CLASS;
            InitCommonControlsEx(&icex);
            
            // Create control group
            CreateWindow("STATIC", "Control Panel", 
                        WS_VISIBLE | WS_CHILD | SS_LEFT,
                        10, 10, 300, 20,
                        hwnd, NULL, GetModuleHandle(NULL), NULL);
            
            // Creates "Load Image" button (enabled by default).
            g_app->load_button = CreateWindow(
                "BUTTON", "Load Image", 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                10, 40, 120, 30,
                hwnd, (HMENU)ID_LOAD_BUTTON, GetModuleHandle(NULL), NULL);
           
            // Creates "Process Image" button (disabled initially).   
            g_app->process_button = CreateWindow(
                "BUTTON", "Process Image", 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_DISABLED,
                140, 40, 120, 30,
                hwnd, (HMENU)ID_PROCESS_BUTTON, GetModuleHandle(NULL), NULL);
            
            //  Creates "Save Results" button (disabled initially).
            g_app->save_button = CreateWindow(
                "BUTTON", "Save Results", 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_DISABLED,
                270, 40, 120, 30,
                hwnd, (HMENU)ID_SAVE_BUTTON, GetModuleHandle(NULL), NULL);
            
            //  Creates a progress bar
            g_app->progress_bar = CreateWindow(
                PROGRESS_CLASS, NULL,
                WS_CHILD | PBS_SMOOTH,
                10, 80, 380, 20,
                hwnd, (HMENU)ID_PROGRESS, GetModuleHandle(NULL), NULL);
            
            // Displays performance metrics or status messages.
            g_app->metrics_label = CreateWindow(
                "STATIC", "No image loaded", 
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                10, 110, 380, 100,
                hwnd, NULL, GetModuleHandle(NULL), NULL);
            
            // Creates a status bar
            g_app->status_bar = CreateWindow(
                STATUSCLASSNAME, "Ready",
                WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
                0, 0, 0, 0,
                hwnd, (HMENU)ID_STATUS_BAR, GetModuleHandle(NULL), NULL);
            
            // Indicates successful creation    
            return 0;
        
        // Ensures the status bar resizes with the main window
        case WM_SIZE: {
            SendMessage(g_app->status_bar, WM_SIZE, 0, 0);
            return 0;
        }
        
        // Handles commands from button clicks
        case WM_COMMAND:
            // Load image
            if (LOWORD(wParam) == ID_LOAD_BUTTON) {
                load_image(); // Calls load_image() to load an image
                return 0;
            } 
            // Process image
            else if (LOWORD(wParam) == ID_PROCESS_BUTTON) {
                process_image(); // Calls process_image() for filtering, greyscale conversion and edge detection
                return 0;
            } 
            // Save images
            else if (LOWORD(wParam) == ID_SAVE_BUTTON) {
                save_images(); // Calls save_images() to save the processed images
                return 0;
            }
            break;
        
        // Begins painting the window
        case WM_PAINT: {
            PAINTSTRUCT ps;
            // Begins painting the window
            HDC hdc = BeginPaint(hwnd, &ps);
            // Sets a custom font for drawing labels
            HFONT font = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 
                                  DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, 
                                  CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
            HFONT oldFont = SelectObject(hdc, font);
            
            // Draw frame around images
            HPEN pen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
            HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
            HPEN oldPen = SelectObject(hdc, pen);
            HBRUSH oldBrush = SelectObject(hdc, brush);
            
            // Draw image labels and frames
            RECT rect;
            
            // Top-left image (Original)
            rect.left = 450; rect.top = 10; rect.right = 750; rect.bottom = 30;
            DrawText(hdc, "Original Image", -1, &rect, DT_CENTER);
            Rectangle(hdc, 450, 40, 750, 340);
            
            // Top-right image (Filtered)
            rect.left = 850; rect.right = 1150;
            DrawText(hdc, "Filtered Image", -1, &rect, DT_CENTER);
            Rectangle(hdc, 850, 40, 1150, 340);
            
            // Bottom-left image (Greyscale)
            rect.left = 450; rect.top = 360; rect.right = 750; rect.bottom = 380;
            DrawText(hdc, "Greyscale Image", -1, &rect, DT_CENTER);
            Rectangle(hdc, 450, 390, 750, 690);
            
            // Bottom-right image (Edge)
            rect.left = 850; rect.right = 1150;
            DrawText(hdc, "Edge Image", -1, &rect, DT_CENTER);
            Rectangle(hdc, 850, 390, 1150, 690);
            
            // Draw images
            rect.left = 455; rect.top = 45; rect.right = 745; rect.bottom = 335;
            draw_image(hdc, g_app->original_bmp, &rect);
        
            rect.left = 855; rect.right = 1145;
            draw_image(hdc, g_app->filtered_bmp, &rect);
            
            rect.left = 455; rect.top = 395; rect.right = 745; rect.bottom = 685;
            draw_image(hdc, g_app->grey_bmp, &rect);
            
            rect.left = 855; rect.right = 1145;
            draw_image(hdc, g_app->edge_bmp, &rect);
            
            // Restores and deletes GDI objects
            SelectObject(hdc, oldPen);
            SelectObject(hdc, oldBrush);
            DeleteObject(pen);
            DeleteObject(brush);
            SelectObject(hdc, oldFont);
            DeleteObject(font);
            EndPaint(hwnd, &ps);
            return 0;
        }
        
        case WM_DESTROY:
            // Frees dynamically allocated image buffers
            if (g_app->original_img) stbi_image_free(g_app->original_img);
            if (g_app->filtered_rgb) free(g_app->filtered_rgb);
            if (g_app->grey_image) free(g_app->grey_image);
            if (g_app->edge_image) free(g_app->edge_image);
            // Deletes created GDI bitmaps
            if (g_app->original_bmp) DeleteObject(g_app->original_bmp);
            if (g_app->filtered_bmp) DeleteObject(g_app->filtered_bmp);
            if (g_app->grey_bmp) DeleteObject(g_app->grey_bmp);
            if (g_app->edge_bmp) DeleteObject(g_app->edge_bmp);
            // Frees global app data and posts quit messages
            free(g_app);
            g_app = NULL;
            PostQuitMessage(0);
            return 0;
    }
    // Passes unhandled messages to the defaulr window procedure
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// ======================================================================================================================
// Main Entry Point
// ======================================================================================================================
/**
 * @brief Main entry point for the Windows application.
 * 
 * @param hInstance Handle to the current instance of the application.
 * @param hPrevInstance Handle to the previous instance of the application (always NULL for Win32).
 * @param lpCmdLine Command line for the application, excluding the program name.
 * @param nCmdShow Controls how the window is to be shown.
 * @return int Exit code of the application.
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc = {0}; // Declares and zeroes out a WNDCLASSEX struct for defining the window class.
    wc.cbSize = sizeof(WNDCLASSEX); // Sets the size
    wc.style = CS_HREDRAW | CS_VREDRAW; // Redraws styles
    wc.lpfnWndProc = WindowProc; // Assigns the window procedure that handles events 
    wc.hInstance = hInstance; // Sets the application instance
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Sets  standard arrow cursor
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Specifies the background brush
    wc.lpszClassName = WND_CLASS_NAME; // Sets the class name for window creation
    
    // Check if registration of the window class with the OS failed
    // Yes: Display an error message and exits the function. 
    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    
    // Allocate memory for global app state
    g_app = malloc(sizeof(AppData));
    // Check if memory allocation failed
    // Yes: Display an error message and exits the function. 
    if (!g_app) {
        MessageBox(NULL, "Memory allocation failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Initializes all fields in the g_app struct to zero
    memset(g_app, 0, sizeof(AppData));
    
    // Creates main window
    g_app->window = CreateWindowEx(
        0, WND_CLASS_NAME, "Image Processing Pipeline",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL
    );
    
    // Check if main window creation failed
    // Yes: Display an error message, frees allocated memory and exits the function. 
    if (g_app->window == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        free(g_app);
        return 0;
    }
    
    // Displays the window using the startup mode (nCmdShow)
    ShowWindow(g_app->window, nCmdShow);
    // Sends a WM_PAINT message immediately to draw the initial contents.
    UpdateWindow(g_app->window);
    
    MSG msg;

    // Run message loop
    // GetMessage(): Waits for and retrieves messages
    // TranslateMessage(): Translates keyboard messages
    // DispatchMessage(): Sends messages to the window procedure (WindowProc) 
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    // Returns the wParam from the WM_QUIT message as the application’s exit code.
    return (int)msg.wParam;
}

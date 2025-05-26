# EEE4120F--YODA: Image Edge Detection Processing (IEDP)

Welcome to the repository for our final-year digital systems project, developed for the EEE4120F course. This project is focused on hardware-accelerated image processing using Verilog HDL and C, and includes implementations of Median Filtering and Sobel Edge Detection.

The project is divided into two main sections:

1. Median Filtering (Noise Reduction)
- Objective: Implement a basic 3×3 median filter in VHDL/Verilog.
- Description: For every pixel in an image, extract a 3×3 neighborhood, sum the RGB values, and determine the median pixel. This pixel replaces the original pixel.

2. Sobel Edge Detection
- Objective: Implement the Sobel filter to detect edges in an image.
- Description: The Sobel operator calculates gradients in the horizontal and vertcal directions to highlight image edges

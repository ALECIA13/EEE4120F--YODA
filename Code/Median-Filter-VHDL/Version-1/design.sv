`timescale 1ns/1ps

module median_filter (
    input wire clk,
    input wire rst_n,
    input wire start,
    input wire [7:0] input_r [0:2][0:2],
    input wire [7:0] input_g [0:2][0:2],
    input wire [7:0] input_b [0:2][0:2],
    output reg [7:0] output_r [0:2][0:2],
    output reg [7:0] output_g [0:2][0:2],
    output reg [7:0] output_b [0:2][0:2],
    output reg done
);

    // State machine states
    localparam IDLE = 2'd0;
    localparam LOAD = 2'd1;
    localparam SORT = 2'd2;
    localparam FINISH = 2'd3;

    // Registers for control
    reg [1:0] state;
    reg [3:0] sort_step;
    reg [3:0] compare_idx;
    reg [7:0] window_r [0:8];
    reg [7:0] window_g [0:8];
    reg [7:0] window_b [0:8];
    reg [23:0] brightness [0:8]; // Increased to 24 bits to avoid overflow

    // Temp variables for swapping
    reg [23:0] temp_brightness;
    reg [7:0] temp_r, temp_g, temp_b;

    // Initial state
    initial begin
        state = IDLE;
        done = 0;
    end

    // Main state machine
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state <= IDLE;
            done <= 0;
            sort_step <= 0;
            compare_idx <= 0;
        end 
        else begin
            case (state)
                IDLE: begin
                    done <= 0;
                    sort_step <= 0;
                    compare_idx <= 0;
                    
                    // Initialize output with input (default for edge pixels)
                    for (int i = 0; i < 3; i = i + 1) begin
                        for (int j = 0; j < 3; j = j + 1) begin
                            output_r[i][j] <= input_r[i][j];
                            output_g[i][j] <= input_g[i][j];
                            output_b[i][j] <= input_b[i][j];
                        end
                    end
                    
                    if (start) begin
                        state <= LOAD;
                    end
                end
                
                LOAD: begin
                    // Load the 3x3 window for processing the center pixel
                    window_r[0] <= input_r[0][0]; window_g[0] <= input_g[0][0]; window_b[0] <= input_b[0][0];
                    window_r[1] <= input_r[0][1]; window_g[1] <= input_g[0][1]; window_b[1] <= input_b[0][1];
                    window_r[2] <= input_r[0][2]; window_g[2] <= input_g[0][2]; window_b[2] <= input_b[0][2];
                    window_r[3] <= input_r[1][0]; window_g[3] <= input_g[1][0]; window_b[3] <= input_b[1][0];
                    window_r[4] <= input_r[1][1]; window_g[4] <= input_g[1][1]; window_b[4] <= input_b[1][1];
                    window_r[5] <= input_r[1][2]; window_g[5] <= input_g[1][2]; window_b[5] <= input_b[1][2];
                    window_r[6] <= input_r[2][0]; window_g[6] <= input_g[2][0]; window_b[6] <= input_b[2][0];
                    window_r[7] <= input_r[2][1]; window_g[7] <= input_g[2][1]; window_b[7] <= input_b[2][1];
                    window_r[8] <= input_r[2][2]; window_g[8] <= input_g[2][2]; window_b[8] <= input_b[2][2];
                    
                    // Calculate brightness for each pixel in the window
                    for (int i = 0; i < 9; i = i + 1) begin
                        brightness[i] <= input_r[i/3][i%3] + input_g[i/3][i%3] + input_b[i/3][i%3];
                    end
                    
                    state <= SORT;
                end
                
                SORT: begin
                    // Bubble sort - one comparison per clock cycle
                    if (sort_step < 8) begin
                        if (compare_idx < (8 - sort_step)) begin
                            // Compare and swap if needed
                            if (brightness[compare_idx] > brightness[compare_idx + 1]) begin
                                // Swap brightness
                                temp_brightness = brightness[compare_idx];
                                brightness[compare_idx] = brightness[compare_idx + 1];
                                brightness[compare_idx + 1] = temp_brightness;
                                
                                // Swap RGB values
                                temp_r = window_r[compare_idx];
                                window_r[compare_idx] = window_r[compare_idx + 1];
                                window_r[compare_idx + 1] = temp_r;
                                
                                temp_g = window_g[compare_idx];
                                window_g[compare_idx] = window_g[compare_idx + 1];
                                window_g[compare_idx + 1] = temp_g;
                                
                                temp_b = window_b[compare_idx];
                                window_b[compare_idx] = window_b[compare_idx + 1];
                                window_b[compare_idx + 1] = temp_b;
                            end
                            
                            compare_idx <= compare_idx + 1;
                        end
                        else begin
                            // Move to next sort step
                            sort_step <= sort_step + 1;
                            compare_idx <= 0;
                        end
                    end
                    else begin
                        // Sorting complete, get median (index 4)
                        output_r[1][1] <= window_r[4];
                        output_g[1][1] <= window_g[4];
                        output_b[1][1] <= window_b[4];
                        
                        state <= FINISH;
                    end
                end
                
                FINISH: begin
                    done <= 1;
                    state <= IDLE;
                end
                
                default: state <= IDLE;
            endcase
        end
    end

endmodule

`timescale 1ns / 1ps

module tb_system;

    parameter W = 120;
    parameter H = 120;
    parameter TOTAL_PIXELS = W * H;
    parameter TOTAL_RGB_BYTES = TOTAL_PIXELS * 3;

    reg clk = 0;
    reg rst = 1;

    always #5 clk = ~clk;

    reg [15:0] addr = 0;
    wire [7:0] rgb_data;
    reg rgb_valid = 0;

    wire [7:0] gray_out;
    wire gray_valid;

    wire [7:0] filt_out;
    wire filt_valid;

    reg [7:0] output_mem [0:TOTAL_PIXELS-1];
    integer output_index = 0;
    integer gray_pixel_count = 0;

    // BRAM (holds RGB input)
    bram_rgb #(8, 16, "rgb_image.mem") bram_inst (
        .clk(clk),
        .addr(addr),
        .data_out(rgb_data)
    );

    // RGB → Grayscale
    rgb_to_gray gray_inst (
        .clk(clk),
        .rst(rst),
        .rgb_in(rgb_data),
        .rgb_valid(rgb_valid),
        .gray_out(gray_out),
        .gray_valid(gray_valid)
    );

    // Median Filter
    median_filter #(W, H) filter_inst (
        .clk(clk),
        .rst(rst),
        .pixel_in(gray_out),
        .pixel_valid(gray_valid),
        .pixel_out(filt_out),
        .pixel_out_valid(filt_valid)
    );

    // Simulation Logic
    initial begin
        $display("Starting unified grayscale + median filter testbench...");
        #20 rst = 0;

        forever begin
            @(posedge clk);

            if (addr < TOTAL_RGB_BYTES) begin
                rgb_valid <= 1;
                addr <= addr + 1;
                if (addr == 0)
                    $display("Started feeding RGB data...");
            end else begin
                rgb_valid <= 0;
            end

            if (gray_valid) begin
                gray_pixel_count = gray_pixel_count + 1;
                if (gray_pixel_count <= 5)
                    $display("Gray pixel %0d: %02x", gray_pixel_count, gray_out);
            end

            if (filt_valid && gray_pixel_count >= 9) begin
                output_mem[output_index] <= filt_out;
                if (output_index < 5)
                    $display("Filtered pixel %0d: %02x", output_index, filt_out);
                output_index = output_index + 1;

                if (output_index == TOTAL_PIXELS) begin
                    $writememh("filtered.mem", output_mem);
                    $display("Filtered output written to filtered.mem");
                    $display("Total output pixels: %0d", output_index);
                    $finish;
                end
            end
        end
    end

endmodule

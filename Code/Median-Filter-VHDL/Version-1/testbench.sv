`timescale 1ns/1ps

module median_filter_tb;

    // Clock and reset signals
    reg clk;
    reg rst_n;
    reg start;

    // Input and output arrays
    reg  [7:0] input_r [0:2][0:2];
    reg  [7:0] input_g [0:2][0:2];
    reg  [7:0] input_b [0:2][0:2];
    wire [7:0] output_r [0:2][0:2];
    wire [7:0] output_g [0:2][0:2];
    wire [7:0] output_b [0:2][0:2];
    wire done;

    // Instantiate the median filter module
    median_filter dut (
        .clk(clk),
        .rst_n(rst_n),
        .start(start),
        .input_r(input_r),
        .input_g(input_g),
        .input_b(input_b),
        .output_r(output_r),
        .output_g(output_g),
        .output_b(output_b),
        .done(done)
    );

    // Clock generation
    always #5 clk = ~clk;

    // Test procedure
    initial begin
        // Dump waveform
        $dumpfile("dump.vcd");
        $dumpvars(0, median_filter_tb);

        // Initialize signals
        clk = 0;
        rst_n = 0;
        start = 0;

        // Initialize input image (3x3 RGB)
        input_r[0][0] = 8'd2;   input_g[0][0] = 8'd4;   input_b[0][0] = 8'd7;
        input_r[0][1] = 8'd30;  input_g[0][1] = 8'd46;  input_b[0][1] = 8'd23;
        input_r[0][2] = 8'd23;   input_g[0][2] = 8'd65;   input_b[0][2] = 8'd86;
        input_r[1][0] = 8'd34;  input_g[1][0] = 8'd57;  input_b[1][0] = 8'd3;
        input_r[1][1] = 8'd34;   input_g[1][1] = 8'd87;   input_b[1][1] = 8'd94;
        input_r[1][2] = 8'd123;   input_g[1][2] = 8'd143;   input_b[1][2] = 8'd67;
        input_r[2][0] = 8'd23;  input_g[2][0] = 8'd43;  input_b[2][0] = 8'd197;
        input_r[2][1] = 8'd33;   input_g[2][1] = 8'd76;   input_b[2][1] = 8'd97;
        input_r[2][2] = 8'd34;   input_g[2][2] = 8'd78;   input_b[2][2] = 8'd54;

        // Reset
        #10 rst_n = 1;

        // Start the median filter
        #10 start = 1;
        #10 start = 0;

        // Wait for completion
        wait (done == 1);
        #10;

        // Display original image
        $display("Original image:");
        for (int i = 0; i < 3; i++) begin
            for (int j = 0; j < 3; j++) begin
                $write("(%3d, %3d, %3d) ", input_r[i][j], input_g[i][j], input_b[i][j]);
            end
            $display("");
        end

        // Display processed image
        $display("\nProcessed image:");
        for (int i = 0; i < 3; i++) begin
            for (int j = 0; j < 3; j++) begin
                $write("(%3d, %3d, %3d) ", output_r[i][j], output_g[i][j], output_b[i][j]);
            end
            $display("");
        end

        // Finish simulation
        #20 $finish;
    end

    // Monitor done signal
    always @(posedge done) begin
        $display("Median filter processing completed at time %0t", $time);
    end

endmodule

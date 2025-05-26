`timescale 1ns / 1ps

module tb_system;

    parameter W = 60;
    parameter H = 60;
    parameter TOTAL_PIXELS = W * H;

    reg clk = 0, rst = 1;
    always #5 clk = ~clk; // 10ns clock period

    wire [7:0] gray_out, filt_out;
    wire gray_valid, filt_valid;

    reg [7:0] output_mem [0:TOTAL_PIXELS-1];
    integer output_index = 0;
    integer i;

    // Timing variables (all declared at the top)
    integer input_cycle_start = 0;
    integer gray_first_output_cycle = 0;
    integer gray_last_output_cycle = 0;
    integer filt_first_output_cycle = 0;
    integer filt_last_output_cycle = 0;
    integer first_output_cycle = 0;
    integer last_output_cycle = 0;
    integer n_outputs = 0;
    reg first_gray_output_seen = 0;
    reg last_gray_output_seen = 0;
    reg first_output_seen = 0;

    integer latency_cycles;
    integer latency_ns;
    integer total_cycles;
    integer total_ns;
    real throughput_px_per_cycle;
    real throughput_px_per_ns;
    integer gray_module_cycles;
    integer gray_module_ns;
    integer median_module_cycles;
    integer median_module_ns;
    integer gray_pixel_count = 0;

    // Utility: returns 1 if val is any X
    function is_x;
        input [7:0] val;
        begin
            is_x = (^val === 1'bx);
        end
    endfunction

    // DUTs
    rgb_to_gray #(.MEM_FILE("input_image.mem"), .TOTAL_BYTES(TOTAL_PIXELS*3))
        gray_inst (.clk(clk), .rst(rst), .gray_out(gray_out), .gray_valid(gray_valid));

    median_filter #(.W(W), .H(H))
        filter_inst (.clk(clk), .rst(rst),
                     .pixel_in(gray_out), .pixel_valid(gray_valid),
                     .pixel_out(filt_out), .pixel_out_valid(filt_valid));

    // Initialize output memory
    initial begin
        for (i = 0; i < TOTAL_PIXELS; i = i + 1)
            output_mem[i] = 8'h00;
    end

    // Main simulation control
    initial begin
        $display("Starting testbench...");
        #20 rst = 0;
        input_cycle_start = $time;

        wait (output_index == TOTAL_PIXELS);
        repeat (3) @(posedge clk);

        $writememh("filtered.mem", output_mem);
        $display("Filtered output written to filtered.mem");
        $display("Total output pixels: %0d", output_index);

        // Assign metrics here, after output_index is complete
        latency_cycles = (first_output_cycle - input_cycle_start)/10; // 10ns per cycle
        latency_ns = (first_output_cycle - input_cycle_start);
        total_cycles = (last_output_cycle - input_cycle_start)/10;
        total_ns = (last_output_cycle - input_cycle_start);
        throughput_px_per_cycle = (total_cycles > 0) ? (1.0 * TOTAL_PIXELS) / total_cycles : 0;
        throughput_px_per_ns = (total_ns > 0) ? (1.0 * TOTAL_PIXELS) / total_ns : 0;
        gray_module_cycles = (gray_last_output_cycle - gray_first_output_cycle)/10;
        gray_module_ns = (gray_last_output_cycle - gray_first_output_cycle);
        median_module_cycles = (filt_last_output_cycle - filt_first_output_cycle)/10;
        median_module_ns = (filt_last_output_cycle - filt_first_output_cycle);

        $display("------ Pipeline Metrics ------");
        $display("Latency (input to first output): %0d cycles, %0d ns", latency_cycles, latency_ns);
        $display("Total time (input to last output): %0d cycles, %0d ns", total_cycles, total_ns);
        $display("Throughput: %0.4f pixels/clock, %0.4f pixels/ns", throughput_px_per_cycle, throughput_px_per_ns);
        $display("------ Module-specific Metrics ------");
        $display("Grayscale module: %0d cycles, %0d ns for %0d pixels", gray_module_cycles, gray_module_ns, gray_pixel_count);
        $display("Median filter module: %0d cycles, %0d ns for %0d pixels", median_module_cycles, median_module_ns, output_index);
        $display("-----------------------------");

        $finish;
    end

    // Track grayscale output timing
    always @(posedge clk) begin
        if (gray_valid) begin
            if (!first_gray_output_seen) begin
                gray_first_output_cycle = $time;
                first_gray_output_seen = 1;
                $display("First grayscale pixel at time %0t ns", $time);
            end
            gray_pixel_count = gray_pixel_count + 1;
            gray_last_output_cycle = $time;
        end
    end

    // Only collect if output is truly valid (not X)
    always @(posedge clk) begin
        if (filt_valid && output_index < TOTAL_PIXELS) begin
            if (!is_x(filt_out)) begin
                output_mem[output_index] <= filt_out;
                if (!first_output_seen) begin
                    first_output_cycle = $time;
                    filt_first_output_cycle = $time;
                    first_output_seen = 1;
                    $display("First filtered pixel (index 0): %02x at time %0t ns", filt_out, $time);
                end
                filt_last_output_cycle = $time;
                if (output_index == TOTAL_PIXELS-1)
                    $display("Last filtered pixel (index %0d): %02x at time %0t ns", output_index, filt_out, $time);
                last_output_cycle = $time;
                output_index = output_index + 1;
            end
        end
    end

endmodule

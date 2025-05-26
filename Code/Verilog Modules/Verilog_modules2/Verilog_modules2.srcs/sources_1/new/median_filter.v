`timescale 1ns / 1ps

module median_filter #(
    parameter W = 60,
    parameter H = 60
)(
    input wire clk,
    input wire rst,
    input wire [7:0] pixel_in,
    input wire pixel_valid,
    output reg [7:0] pixel_out,
    output reg pixel_out_valid
);

    reg [7:0] line_buf1 [0:W-1];
    reg [7:0] line_buf2 [0:W-1];
    reg [7:0] window [0:8];
    reg [7:0] lb2_col, lb2_col_1, lb2_col_2;
    reg [7:0] lb1_col, lb1_col_1, lb1_col_2;
    integer col = 0, row = 0;
    integer i;

    task sort_window;
        integer i, j;
        reg [7:0] temp;
        begin
            for (i = 0; i < 9; i = i + 1)
                for (j = i + 1; j < 9; j = j + 1)
                    if (window[i] > window[j]) begin
                        temp = window[i];
                        window[i] = window[j];
                        window[j] = temp;
                    end
        end
    endtask

    always @(posedge clk or posedge rst) begin
        if (rst) begin
            pixel_out <= 0;
            pixel_out_valid <= 0;
            col <= 0;
            row <= 0;
            for (i = 0; i < W; i = i + 1) begin
                line_buf1[i] <= 0;
                line_buf2[i] <= 0;
            end
            for (i = 0; i < 9; i = i + 1) begin
                window[i] <= 0;
            end
            lb2_col   <= 0;
            lb2_col_1 <= 0;
            lb2_col_2 <= 0;
            lb1_col   <= 0;
            lb1_col_1 <= 0;
            lb1_col_2 <= 0;
        end else if (pixel_valid) begin
            // -- Use pixel_in if on first row (row==0), otherwise use buffer
            lb2_col   = (row >= 2) ? line_buf2[col]    : pixel_in;
            lb2_col_1 = (row >= 2 && col >= 1) ? line_buf2[col-1] : pixel_in;
            lb2_col_2 = (row >= 2 && col >= 2) ? line_buf2[col-2] : pixel_in;
            lb1_col   = (row >= 1) ? line_buf1[col]    : pixel_in;
            lb1_col_1 = (row >= 1 && col >= 1) ? line_buf1[col-1] : pixel_in;
            lb1_col_2 = (row >= 1 && col >= 2) ? line_buf1[col-2] : pixel_in;

            window[0] = lb2_col_2;
            window[1] = lb2_col_1;
            window[2] = lb2_col;
            window[3] = lb1_col_2;
            window[4] = lb1_col_1;
            window[5] = lb1_col;
            window[6] = pixel_in;
            window[7] = pixel_in;
            window[8] = pixel_in;

            pixel_out_valid <= 1;
            if ((row >= 2) && (col >= 2)) begin
                sort_window();
                pixel_out <= window[4];
            end else begin
                pixel_out <= pixel_in;
            end

            line_buf2[col] <= line_buf1[col];
            line_buf1[col] <= pixel_in;

            if (col == W - 1) begin
                col <= 0;
                row <= row + 1;
            end else begin
                col <= col + 1;
            end
        end else begin
            pixel_out_valid <= 0;
        end
    end
endmodule

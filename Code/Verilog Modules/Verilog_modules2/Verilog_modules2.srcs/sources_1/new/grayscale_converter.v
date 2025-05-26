`timescale 1ns / 1ps

module rgb_to_gray #(
    parameter MEM_FILE = "input_image.mem",
    parameter TOTAL_BYTES = 10800 // for 60x60 RGB image
)(
    input wire clk,
    input wire rst,
    output reg [7:0] gray_out,
    output reg gray_valid
);

    reg [7:0] mem [0:TOTAL_BYTES-1];
    reg [1:0] state = 0;
    reg [15:0] addr = 0;
    reg [7:0] r, g, b;
    reg ready = 0;

    integer i;

    initial begin
        $display("Loading RGB image from '%s'", MEM_FILE);
        $readmemh(MEM_FILE, mem);
        $display("mem[0]=%02x, mem[1]=%02x, mem[2]=%02x", mem[0], mem[1], mem[2]);
    end

    always @(posedge clk or posedge rst) begin
        if (rst) begin
            state <= 0;
            addr <= 0;
            gray_out <= 0;
            gray_valid <= 0;
            r <= 0;
            g <= 0;
            b <= 0;
            ready <= 0;
        end else begin
            gray_valid <= 0;
            case (state)
                0: begin
                    if (addr < TOTAL_BYTES) begin
                        r <= mem[addr];
                        addr <= addr + 1;
                        state <= 1;
                    end
                end
                1: begin
                    if (addr < TOTAL_BYTES) begin
                        g <= mem[addr];
                        addr <= addr + 1;
                        state <= 2;
                    end
                end
                2: begin
                    if (addr < TOTAL_BYTES) begin
                        b <= mem[addr];
                        addr <= addr + 1;
                        // Now that R, G, B are guaranteed to be loaded, compute gray
                        gray_out <= (r * 77 + g * 150 + b * 29) >> 8;
                        gray_valid <= 1;
                        state <= 0;
                    end
                end
            endcase
        end
    end
endmodule

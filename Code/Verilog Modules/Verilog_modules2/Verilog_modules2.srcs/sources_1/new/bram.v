`timescale 1ns/1ps

module bram_rgb #(
    parameter DATA_WIDTH = 8,
    parameter ADDR_WIDTH = 16,
    parameter MEM_FILE = "rgb_image.mem"
)(
    input wire clk,
    input wire [ADDR_WIDTH-1:0] addr,
    output reg [DATA_WIDTH-1:0] data_out
);

    reg [DATA_WIDTH-1:0] mem [0:(1<<ADDR_WIDTH)-1];

    initial begin
        $readmemh(MEM_FILE, mem);
    end

    always @(posedge clk) begin
        data_out <= mem[addr];
    end

endmodule

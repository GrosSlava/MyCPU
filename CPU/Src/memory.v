
`default_nettype none

`include "memory.vh"



/**
	RAM IO.
*/
module Memory
(
	input wire i_clk,
	input wire [1:0] i_read,
	input wire [1:0] i_write,
	input wire [7:0] i_address,

	inout wire [23:0] io_bus
);
integer i;
reg [7:0] l_mem [255:0];

assign io_bus[7:0] = (i_read == `MEM_READ_1 || i_read == `MEM_READ_2 || i_read == `MEM_READ_3) ? l_mem[i_address] : 8'bZZZZZZZZ;
assign io_bus[15:8] = (i_read == `MEM_READ_2 || i_read == `MEM_READ_3) ? l_mem[i_address + 1] : 8'bZZZZZZZZ;
assign io_bus[23:16] = (i_read == `MEM_READ_3) ? l_mem[i_address + 2] : 8'bZZZZZZZZ;

initial begin
	for( i = 0; i < 255; i++ ) begin
		l_mem[i] = 8'b00000000;
	end

	// 0: SUB X0, X0
	l_mem[0] = 8'b00000000;
	l_mem[1] = 8'b00100110;

	// 1: ADD X0, #1
	l_mem[2] = 8'b00000001;
	l_mem[3] = 8'b00100000;

	// 2: LED X0
	l_mem[4] = 8'b00000000;
	l_mem[5] = 8'b00000010;

	// 2: J #1
	l_mem[6] = 8'b00000001;
	l_mem[7] = 8'b10100000;

	// 4: HLT
	l_mem[8] = 8'b00000000;
	l_mem[9] = 8'b00000001;
end

always @(posedge i_clk) begin
	if( i_write == `MEM_WRITE_1 ) begin
		l_mem[i_address] <= io_bus[7:0];
	end else if( i_write == `MEM_WRITE_2 ) begin
		l_mem[i_address] <= io_bus[7:0];
		l_mem[i_address + 1] <= io_bus[15:8];
	end else if( i_write == `MEM_WRITE_3 ) begin
		l_mem[i_address] <= io_bus[7:0];
		l_mem[i_address + 1] <= io_bus[15:8];
		l_mem[i_address + 2] <= io_bus[23:16];
	end
end

endmodule // Memory


`default_nettype none

`include "alu.vh"



/**
	Module for arithmetic & logic operations.
	Implements unary, binary, thernary operations.
*/
module ALU2
(
	input wire i_clk,
	input wire i_reset,
	input wire i_readData,
	input wire i_readFlags,
	input wire [4:0] i_op,

	inout wire [23:0] io_bus,
	output wire [3:0] o_nzcv
);

reg [23:0] l_data;
reg [3:0] l_nzcv;

assign io_bus[7:0] = (i_readData == 1'b1) ? l_data[7:0] : 8'bZZZZZZZZ;
assign o_nzcv = (i_readFlags == 1'b1) ? l_nzcv : 4'bZZZZ;

initial begin
	l_data <= 23'b00000000000000000000000;
	l_nzcv <= 4'b0000;
end

always @(negedge i_clk or posedge i_reset) begin
	if( i_reset == 1'b1 ) begin
		l_data <= 23'b00000000000000000000000;
		l_nzcv <= 4'b0000;
	end else begin
		case( i_op )
		`ALU_NOP: begin
			// NOP
		end
		`ALU_SUM_2: begin
			{l_nzcv[1], l_data[7:0]} <= io_bus[15:8] + io_bus[7:0];
			l_nzcv[3] <= ((io_bus[15:8] + io_bus[7:0]) & 8'b10000000) == 8'b10000000;
			l_nzcv[2] <= ~(|(io_bus[15:8] + io_bus[7:0]));
			l_nzcv[0] = { l_nzcv[1], l_data[7] } == 2'b01;
		end
		`ALU_SUB_2: begin
			{l_nzcv[1], l_data[7:0]} <= io_bus[15:8] - io_bus[7:0];
			l_nzcv[3] <= ((io_bus[15:8] - io_bus[7:0]) & 8'b10000000) == 8'b10000000;
			l_nzcv[2] <= ~(|(io_bus[15:8] - io_bus[7:0]));
			l_nzcv[0] = { l_nzcv[1], l_data[7] } == 2'b01;
		end
		endcase
	end
end

endmodule // ALU2

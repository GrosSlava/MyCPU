
`default_nettype none

`include "register.vh"



/**
	CPU register.
*/
module Register
(
	input wire i_clk,
	input wire[2:0] i_op,
	input wire[1:0] i_read,
	input wire i_reset,

	inout wire [23:0] io_bus
);

reg [7:0] l_data;

initial begin
	l_data <= 8'b00000000;
end

assign io_bus[7:0] = (i_read == `REG_READ_TO_0) ? l_data : 8'bZZZZZZZZ;
assign io_bus[15:8] = (i_read == `REG_READ_TO_1) ? l_data : 8'bZZZZZZZZ;
assign io_bus[23:16] = (i_read == `REG_READ_TO_2) ? l_data : 8'bZZZZZZZZ;

always @(posedge i_clk or posedge i_reset) begin
	if( i_reset == 1'b1 ) begin
		l_data <= 8'b00000000;
	end else begin
		case( i_op )
		`REG_OP_NOP: begin
			// NOP
		end
		`REG_OP_CLEAR: begin
			l_data <= 8'b00000000;
		end
		`REG_OP_FILL: begin
			l_data <= 8'b11111111;
		end
		`REG_OP_WRITE: begin
			l_data <= io_bus[7:0];
		end
		`REG_OP_INC: begin
			l_data <= l_data + 1'b1;
		end
		`REG_OP_DEC: begin
			l_data <= l_data - 1'b1;
		end
		`REG_OP_INV: begin
			l_data <= ~l_data;
		end
		`REG_OP_BOOL: begin
			l_data <= (l_data == 8'b00000000) ? 8'b00000000 : 8'b00000001;
		end
		endcase
	end
end

endmodule // Register


/**
	CPU system register.
*/
module SysRegister
(
	input wire i_clk,
	input wire i_write,
	input wire [1:0] i_read,
	input wire i_reset,

	inout wire [23:0] io_bus,
	output reg [7:0] o_data
);

initial begin
	o_data <= 8'b00000000;
end

assign io_bus[7:0] = (i_read == `REG_READ_TO_0) ? o_data : 8'bZZZZZZZZ;
assign io_bus[15:8] = (i_read == `REG_READ_TO_1) ? o_data : 8'bZZZZZZZZ;
assign io_bus[23:16] = (i_read == `REG_READ_TO_2) ? o_data : 8'bZZZZZZZZ;

always @(posedge i_clk or posedge i_reset) begin
	if( i_reset == 1'b1 ) begin
		o_data <= 8'b00000000;
	end else if( i_write == 1'b1 ) begin
		o_data <= io_bus[7:0];
	end
end

endmodule // SysRegister

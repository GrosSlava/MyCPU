
`default_nettype none

`include "isa.vh"
`include "memory.vh"
`include "register.vh"
`include "alu.vh"



`define REGISTER_IO(name)                \
	output reg [2:0] o_reg``name``Op,    \
	output reg [1:0] o_reg``name``Read

`define REGISTER_RESET(name)           \
	o_reg``name``Op <= `REG_OP_NOP;    \
	o_reg``name``Read <= `REG_READ_NO;

`define SYS_REGISTER_IO(name)           \
	output reg o_reg``name``Write,      \
	output reg [1:0] o_reg``name``Read

`define SYS_REGISTER_RESET(name)        \
	o_reg``name``Write <= 1'b0;         \
	o_reg``name``Read <= `REG_READ_NO;

`define REG_READ(index, op)               \
	case( index )                         \
	3'b000: begin o_regX0Read <= op; end  \
	3'b001: begin o_regX1Read <= op; end  \
	3'b010: begin o_regX2Read <= op; end  \
	3'b011: begin o_regX3Read <= op; end  \
	3'b100: begin o_regX4Read <= op; end  \
	3'b101: begin o_regX5Read <= op; end  \
	3'b110: begin o_regX6Read <= op; end  \
	3'b111: begin o_regX7Read <= op; end  \
	endcase
`define REG_OP(index, op)               \
	case( index )                       \
	3'b000: begin o_regX0Op <= op; end  \
	3'b001: begin o_regX1Op <= op; end  \
	3'b010: begin o_regX2Op <= op; end  \
	3'b011: begin o_regX3Op <= op; end  \
	3'b100: begin o_regX4Op <= op; end  \
	3'b101: begin o_regX5Op <= op; end  \
	3'b110: begin o_regX6Op <= op; end  \
	3'b111: begin o_regX7Op <= op; end  \
	endcase


`define CYCLE_FETCH   2'b00 // Fetch instruction from memory
`define CYCLE_DECODE  2'b01 // Decode instruction
`define CYCLE_PROCESS 2'b10 // Process instruction
`define CYCLE_INC_PC  2'b11 // Go to next instr

`define GO_TO_NEXT_INSTR() l_cycleStep <= `CYCLE_INC_PC;
`define GO_TO_NEXT_INSTR_NO_PC() l_cycleStep <= `CYCLE_FETCH;
`define FAST_GO_TO_NEXT_INSTR()       \
	o_memAddress <= o_memAddress + 2; \
	l_cycleStep <= `CYCLE_FETCH;

`define CLEAR_BUFF() l_buff <= 23'bZZZZZZZZZZZZZZZZZZZZZZZ;



`define CTRL_BUS_WRITE_NO 3'b000 // do not write local bus to bus
`define CTRL_BUS_WRITE_0  3'b001 // write 1-st part of local bus to 1-st part of the bas
`define CTRL_BUS_WRITE_1  3'b010 // write 2-nd part of local bus to 2-nd part of the bas
`define CTRL_BUS_WRITE_2  3'b100 // write 3-rd part of local bus to 3-rd part of the bas


/**
	Main module of instructions processing.
*/
module Controller
(
	input wire i_clk,
	input wire i_reset,

	output reg o_halt,

	output reg [1:0] o_memRead,
	output reg [1:0] o_memWrite,
	output reg [7:0] o_memAddress,

	output reg o_aluReadData,
	output reg o_aluReadFlags,
	output reg [4:0] o_aluOp,
	input wire [3:0] i_nzcv,

	inout wire [23:0] io_bus,

	`REGISTER_IO(X0),
	`REGISTER_IO(X1),
	`REGISTER_IO(X2),
	`REGISTER_IO(X3),
	`REGISTER_IO(X4),
	`REGISTER_IO(X5),
	`REGISTER_IO(X6),
	`REGISTER_IO(X7),

	`SYS_REGISTER_IO(LED)
);

// bus
reg [2:0] l_busF;
reg [23:0] l_buff;
assign io_bus[7:0] = (l_busF & `CTRL_BUS_WRITE_0) ? l_buff[7:0] : 8'bZZZZZZZZ;
assign io_bus[15:8] = (l_busF & `CTRL_BUS_WRITE_1) ? l_buff[15:8] : 8'bZZZZZZZZ;
assign io_bus[23:16] = (l_busF & `CTRL_BUS_WRITE_2) ? l_buff[23:16] : 8'bZZZZZZZZ;

// states
reg [1:0] l_cycleStep;
reg [1:0] l_instructionStep;
reg [15:0] l_instr;

initial begin
	o_halt <= 1'b0;

	o_memRead <= `MEM_READ_NO;
	o_memWrite <= `MEM_WRITE_NO;
	o_memAddress <= 8'b00000000;

	o_aluReadData <= 1'b0;
	o_aluReadFlags <= 1'b0;
	o_aluOp <= `ALU_NOP;

	l_cycleStep <= `CYCLE_FETCH;
	l_instructionStep <= 2'b00;
	l_instr <= 16'b0000000000000000;

	l_busF <= `CTRL_BUS_WRITE_NO;
	l_buff <= 23'b00000000000000000000000;

	`REGISTER_RESET(X0);
	`REGISTER_RESET(X1);
	`REGISTER_RESET(X2);
	`REGISTER_RESET(X3);
	`REGISTER_RESET(X4);
	`REGISTER_RESET(X5);
	`REGISTER_RESET(X6);
	`REGISTER_RESET(X7);

	`SYS_REGISTER_RESET(LED);
end

always @(posedge i_clk or posedge i_reset) begin
	if( i_reset == 1'b1 ) begin
		o_halt <= 1'b0;

		o_memRead <= `MEM_READ_NO;
		o_memWrite <= `MEM_WRITE_NO;
		o_memAddress <= 8'b00000000;

		o_aluReadData <= 1'b0;
		o_aluReadFlags <= 1'b0;
		o_aluOp <= `ALU_NOP;

		l_cycleStep <= `CYCLE_FETCH;
		l_instructionStep <= 2'b00;
		l_instr <= 16'b0000000000000000;

		l_busF <= `CTRL_BUS_WRITE_NO;
		l_buff <= 23'b00000000000000000000000;

		`REGISTER_RESET(X0);
		`REGISTER_RESET(X1);
		`REGISTER_RESET(X2);
		`REGISTER_RESET(X3);
		`REGISTER_RESET(X4);
		`REGISTER_RESET(X5);
		`REGISTER_RESET(X6);
		`REGISTER_RESET(X7);

		`SYS_REGISTER_RESET(LED);
	end else begin
		case( l_cycleStep )
		`CYCLE_FETCH: begin
			o_memRead <= `MEM_READ_2;
			l_cycleStep <= `CYCLE_DECODE;
		end
		`CYCLE_DECODE: begin
			o_memRead <= `MEM_READ_NO;
			l_instr <= io_bus[15:0];
			l_cycleStep <= `CYCLE_PROCESS;
			l_instructionStep <= 2'b00;
		end
		`CYCLE_PROCESS: begin
			`include "isaImpl.vh"
		end
		`CYCLE_INC_PC: begin
			`FAST_GO_TO_NEXT_INSTR();
		end
		endcase
	end
end

endmodule // Controller


`undef REGISTER_IO
`undef REGISTER_RESET
`undef SYS_REGISTER_IO
`undef SYS_REGISTER_RESET
`undef REG_READ
`undef REG_OP
`undef GO_TO_NEXT_INSTR
`undef GO_TO_NEXT_INSTR_NO_PC
`undef FAST_GO_TO_NEXT_INSTR
`undef CLEAR_BUFF

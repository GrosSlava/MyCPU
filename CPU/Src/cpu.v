
`default_nettype none



`define REGISTER_DECL(name)       \
	wire [2:0] reg``name``Op;     \
	wire [1:0] reg``name``Read;   \
	Register reg``name            \
	(                             \
		.i_clk(sysClock),         \
		.i_op(reg``name``Op),     \
		.i_read(reg``name``Read), \
		.i_reset(sysReset),       \
		.io_bus(bus)              \
	);

`define REGISTER_ASSIGN(name)            \
	.o_reg``name``Op(reg``name``Op),     \
	.o_reg``name``Read(reg``name``Read)

`define SYS_REGISTER_DECL(name)     \
	wire reg``name``Write;          \
	wire [1:0] reg``name``Read;     \
	wire [7:0] reg``name``Data;     \
	SysRegister reg``name           \
	(                               \
		.i_clk(sysClock),           \
		.i_write(reg``name``Write), \
		.i_read(reg``name``Read),   \
		.i_reset(sysReset),         \
		.io_bus(bus),               \
		.o_data(reg``name``Data)    \
	);

`define SYS_REGISTER_ASSIGN(name)           \
	.o_reg``name``Write(reg``name``Write),  \
	.o_reg``name``Read(reg``name``Read)     \


/**
	CPU entry module.
*/
module CPU
(
	input wire clk,
	input wire reset,
	input wire btn2,

	output wire [5:0] led
);

// bus between modules
wire [23:0] bus;

// flag from reset button
wire sysReset;
assign sysReset = (reset == 1'b1) ? 1'b0 : 1'b1;

// system clock 
wire sysClock;
wire sysHalt;

// memory
wire [1:0] memRead;
wire [1:0] memWrite;
wire [7:0] memAddress;

// alu
wire aluReadData;
wire aluReadFlags;
wire [4:0] aluOp;
wire [3:0] aluFlags;

Clock systemClock
(
	.i_clk(clk),
	.i_halt(sysHalt),
	.o_clk(sysClock)
);

`REGISTER_DECL(X0);
`REGISTER_DECL(X1);
`REGISTER_DECL(X2);
`REGISTER_DECL(X3);
`REGISTER_DECL(X4);
`REGISTER_DECL(X5);
`REGISTER_DECL(X6);
`REGISTER_DECL(X7);

`SYS_REGISTER_DECL(LED);
assign led [5:0] = ~regLEDData[5:0];

Memory memory
(
	.i_clk(sysClock),
	.i_read(memRead),
	.i_write(memWrite),
	.i_address(memAddress),
	.io_bus(bus)
);

ALU2 alu
(
	.i_clk(sysClock),
	.i_reset(sysReset),
	.i_readData(aluReadData),
	.i_readFlags(aluReadFlags),
	.i_op(aluOp),
	.io_bus(bus),
	.o_nzcv(aluFlags)
);

Controller controller
(
	.i_clk(sysClock),
	.i_reset(sysReset),

	.o_halt(sysHalt),

	.o_memRead(memRead),
	.o_memWrite(memWrite),
	.o_memAddress(memAddress),

	.o_aluReadData(aluReadData),
	.o_aluReadFlags(aluReadFlags),
	.o_aluOp(aluOp),
	.i_nzcv(aluFlags),

	.io_bus(bus),
	
	`REGISTER_ASSIGN(X0),
	`REGISTER_ASSIGN(X1),
	`REGISTER_ASSIGN(X2),
	`REGISTER_ASSIGN(X3),
	`REGISTER_ASSIGN(X4),
	`REGISTER_ASSIGN(X5),
	`REGISTER_ASSIGN(X6),
	`REGISTER_ASSIGN(X7),

	`SYS_REGISTER_ASSIGN(LED)
);

endmodule // CPU


`undef REGISTER_DECL
`undef REGISTER_ASSIGN
`undef SYS_REGISTER_DECL
`undef SYS_REGISTER_ASSIGN

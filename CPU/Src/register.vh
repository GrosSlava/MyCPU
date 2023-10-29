
`ifndef REGISTER_VH
`define REGISTER_VH


`define REG_OP_NOP    3'b000 // No op
`define REG_OP_CLEAR  3'b001 // Set reg to all 0
`define REG_OP_FILL   3'b010 // Set reg to all 1
`define REG_OP_WRITE  3'b011 // Write reg from bus
`define REG_OP_INC    3'b100 // Increment reg
`define REG_OP_DEC    3'b101 // Decrement reg
`define REG_OP_INV    3'b110 // Invert reg
`define REG_OP_BOOL   3'b111 // Convert reg to bool

`define REG_READ_NO    2'b00 // disable reading from register
`define REG_READ_TO_0  2'b01 // read from reg to bus 0
`define REG_READ_TO_1  2'b10 // read from reg to bus 1
`define REG_READ_TO_2  2'b11 // read from reg to bus 2

`endif // REGISTER_VH

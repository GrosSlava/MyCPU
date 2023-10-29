
`ifndef ISA_VH
`define ISA_VH


// [15:13] = group
`define GROUP_SYS      3'b000
`define GROUP_ARITHM   3'b001
`define GROUP_ARITHM2  3'b010
`define GROUP_BITWISE  3'b011
`define GROUP_BITWISE2 3'b100
`define GROUP_BRANCH   3'b101
`define GROUP_MEMORY   3'b110

// System: op = 0b000xxxxx
`define NOP         8'b00000000 // xxxxxxxx
`define SYS         8'b00000001 // xxxxxxxx 
    `define SYS_HLT 8'b00000000
`define LED_X       8'b00000010 // rn = xxx
`define MSR_S_X     8'b00000011 // sr = xxxxx rn = xxx
`define MRS_X_S     8'b00000100 // rd = xxx sr = xxxxx 

// Arithmetic: op = 0b001xxxx
`define ADD_X_I     7'b0010000 // rd = xxx imm = xxxxxx           (affect: nzcv)
`define ADD_X_X     7'b0010001 // rd = xxx rn = xxx               (affect: nzcv)
`define SUB_X_I     7'b0010010 // rd = xxx imm = xxxxxx           (affect: nzcv)
`define SUB_X_X     7'b0010011 // rd = xxx rn = xxx               (affect: nzcv)
`define UMUL_X_I    7'b0010100 // rd = xxx imm = xxxxxx           (affect: nzcv)
`define UMUL_X_X    7'b0010101 // rd = xxx rn = xxx               (affect: nzcv)
`define SMUL_X_I    7'b0010110 // rd = xxx imm = xxxxxx           (affect: nzcv)
`define SMUL_X_X    7'b0010111 // rd = xxx rn = xxx               (affect: nzcv)
`define UDIV_X_I    7'b0011000 // rd = xxx imm = xxxxxx           (affect: nzcv)
`define UDIV_X_X    7'b0011001 // rd = xxx rn = xxx               (affect: nzcv)
`define SDIV_X_I    7'b0011010 // rd = xxx imm = xxxxxx           (affect: nzcv)
`define SDIV_X_X    7'b0011011 // rd = xxx rn = xxx               (affect: nzcv)

// Arithmetic_2: op = 0b010xxxx
`define ADD_X_X_X   7'b0100000 // rd = xxx rn = xxx rm = xxx      (affect: nzcv)
`define SUB_X_X_X   7'b0100001 // rd = xxx rn = xxx rm = xxx      (affect: nzcv)
`define UMUL_X_X_X  7'b0100010 // rd = xxx rn = xxx rm = xxx      (affect: nzcv)
`define SMUL_X_X_X  7'b0100011 // rd = xxx rn = xxx rm = xxx      (affect: nzcv)
`define UDIV_X_X_X  7'b0100100 // rd = xxx rn = xxx rm = xxx      (affect: nzcv)
`define SDIV_X_X_X  7'b0100101 // rd = xxx rn = xxx rm = xxx      (affect: nzcv)

// Bitwise: op = 0b011xxxx
`define ORR_X_I     7'b0110000 // rd = xxx imm = xxxxxx           (affect: nzcv)
`define ORR_X_X     7'b0110001 // rd = xxx rn = xxx               (affect: nzcv)
`define AND_X_I     7'b0110010 // rd = xxx imm = xxxxxx           (affect: nzcv)
`define AND_X_X     7'b0110011 // rd = xxx rn = xxx               (affect: nzcv)

// Branch: op = 0b101xxxxx
`define J_I         8'b10100000 // imm = 0xxxxxxx

`endif // ISA_VH

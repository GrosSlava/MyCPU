
`ifndef MEMORY_VH
`define MEMORY_VH


`define MEM_READ_NO 2'b00 // disable reading from memory
`define MEM_READ_1  2'b01 // read from memory to bus 1 byte
`define MEM_READ_2  2'b10 // read from memory to bus 2 byte
`define MEM_READ_3  2'b11 // read from memory to bus 3 byte

`define MEM_WRITE_NO 2'b00 // disable writing to memory
`define MEM_WRITE_1  2'b01 // write to memory from bus 1 byte
`define MEM_WRITE_2  2'b10 // write to memory from bus 2 byte
`define MEM_WRITE_3  2'b11 // write to memory from bus 3 byte

`endif // MEMORY_VH

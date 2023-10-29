
case( l_instr[15:13] )
`GROUP_SYS: begin
	case( l_instr[15:8] )
	`NOP: begin
		`FAST_GO_TO_NEXT_INSTR();
	end
	`SYS: begin
		case( l_instr[7:0] )
		`SYS_HLT: begin
			o_halt <= 1'b1;
			`FAST_GO_TO_NEXT_INSTR();
		end
		default: begin
			`FAST_GO_TO_NEXT_INSTR();
		end
		endcase
	end
	`LED_X: begin
		case( l_instructionStep )
		2'b00: begin
			`REG_READ(l_instr[7:5], `REG_READ_TO_0);
			o_regLEDWrite <= 1'b1;
			l_instructionStep <= 2'b01;
		end
		2'b01: begin
			`REG_READ(l_instr[4:2], `REG_READ_NO);
			o_regLEDWrite <= 1'b0;
			`FAST_GO_TO_NEXT_INSTR();
		end
		endcase
	end
	default: begin
		`FAST_GO_TO_NEXT_INSTR();
	end
	endcase
end
`GROUP_ARITHM: begin
	case( l_instr[15:9] )
	`ADD_X_I: begin
		case( l_instructionStep )
		2'b00: begin
			`REG_READ(l_instr[8:6], `REG_READ_TO_1);
			l_buff[7:0] <= { 2'b00, l_instr[5:0] };
			l_busF <= `CTRL_BUS_WRITE_0; 
			o_aluOp <= `ALU_SUM_2;
			l_instructionStep <= 2'b01;
		end
		2'b01: begin
			`REG_READ(l_instr[8:6], `REG_READ_NO);
			`REG_OP(l_instr[8:6], `REG_OP_WRITE);
			l_busF <= `CTRL_BUS_WRITE_NO;
			o_aluOp <= `ALU_NOP;
			o_aluReadData <= 1'b1;
			l_instructionStep <= 2'b10;
		end
		2'b10: begin
			o_aluReadData <= 1'b0;
			`REG_OP(l_instr[8:6], `REG_OP_NOP);
			`FAST_GO_TO_NEXT_INSTR();
		end
		endcase
	end
	`SUB_X_X: begin
		case( l_instructionStep )
		2'b00: begin
			`REG_READ(l_instr[8:6], `REG_READ_TO_1);
			`REG_READ(l_instr[5:3], `REG_READ_TO_0);
			o_aluOp <= `ALU_SUB_2;
			l_instructionStep <= 2'b01;
		end
		2'b01: begin
			`REG_READ(l_instr[8:6], `REG_READ_NO);
			`REG_READ(l_instr[5:3], `REG_READ_NO);
			`REG_OP(l_instr[8:6], `REG_OP_WRITE);
			o_aluOp <= `ALU_NOP;
			o_aluReadData <= 1'b1;
			l_instructionStep <= 2'b10;
		end
		2'b10: begin
			o_aluReadData <= 1'b0;
			`REG_OP(l_instr[8:6], `REG_OP_NOP);
			`FAST_GO_TO_NEXT_INSTR();
		end
		endcase
	end
	default: begin
		`FAST_GO_TO_NEXT_INSTR();
	end
	endcase
end
`GROUP_ARITHM2: begin
	case( l_instr[15:9] )
	default: begin
		`FAST_GO_TO_NEXT_INSTR();
	end
	endcase
end
`GROUP_BITWISE: begin
	case( l_instr[15:9] )
	default: begin
		`FAST_GO_TO_NEXT_INSTR();
	end
	endcase
end
`GROUP_BITWISE2: begin
	case( l_instr[15:9] )
	default: begin
		`FAST_GO_TO_NEXT_INSTR();
	end
	endcase
end
`GROUP_BRANCH: begin
	case( l_instr[15:8] )
	`J_I: begin
		if( l_instr[7] == 1'b0 ) begin
			o_memAddress <= { l_instr[6:0], 1'b0 };
			`GO_TO_NEXT_INSTR_NO_PC();
		end else begin
			`FAST_GO_TO_NEXT_INSTR();
		end
	end
	default: begin
		`FAST_GO_TO_NEXT_INSTR();
	end
	endcase
end
`GROUP_MEMORY: begin
	case( l_instr[15:9] )
	default: begin
		`FAST_GO_TO_NEXT_INSTR();
	end
	endcase
end
default: begin
	`FAST_GO_TO_NEXT_INSTR();
end
endcase

// Copyright 2022 - 2023 GrosSlava.

#include "ISA.h"





void AsmInstruction::PrintError(std::string& OutError, const std::string& Msg, const Token& Tok) const
{
	OutError += Msg;
	OutError += " (";
	OutError += Tok.GetLexeme();
	OutError += " ";
	OutError += std::to_string(Tok.GetLine());
	OutError += ":";
	OutError += std::to_string(Tok.GetPos());
	OutError += ")";
}




ASM_Value_t MyCPU_ISA::MyCPUInstr::GetRegIndex(const Token& RegToken, std::string& OutError) const
{
	static std ::unordered_map<std::string, ASM_Value_t> RegMap = {{"X0", 0}, {"X1", 1}, {"X2", 2}, {"X3", 3}, {"X4", 4}, {"X5", 5}, {"X6", 6}, {"X7", 7}};

	if( RegToken.GetType() != ETokenType::IDENTIFIER )
	{
		PrintError(OutError, "Expected register name", RegToken);
		return -1;
	}

	const auto& LIter = RegMap.find(RegToken.GetLexeme());
	if( LIter == RegMap.end() )
	{
		PrintError(OutError, "Invalid register name", RegToken);
		return -1;
	}
	else
	{
		return LIter->second;
	}
}

bool MyCPU_ISA::MyCPUInstr::CheckMnemonic(const std::vector<Token>& InstrTokens, std::string& OutError) const
{
	if( InstrTokens.size() < 1 )
	{
		OutError = "Expected mnemonic";
		return false;
	}
	if( InstrTokens[0].GetType() != ETokenType::IDENTIFIER || InstrTokens[0].GetLexeme() != GetMnemonic() )
	{
		PrintError(OutError, "Expected mnemonic", InstrTokens[0]);
		return false;
	}
	return true;
}

bool MyCPU_ISA::MyCPUInstr::CheckRegisterAt(const std::vector<Token>& InstrTokens, unsigned int Index, ASM_Value_t& OutRegIndex, std::string& OutError) const
{
	if( InstrTokens.size() < Index + 1 )
	{
		PrintError(OutError, "Expected register name after", InstrTokens[Index - 1]);
		return false;
	}
	OutRegIndex = GetRegIndex(InstrTokens[Index], OutError);
	return OutRegIndex != -1;
}

bool MyCPU_ISA::MyCPUInstr::CheckImmAt(const std::vector<Token>& InstrTokens, unsigned int Index, ASM_Value_t& OutVal, std::string& OutError) const
{
	if( InstrTokens.size() < Index + 1 )
	{
		PrintError(OutError, "Expected number after", InstrTokens[Index - 1]);
		return false;
	}

	if( InstrTokens[Index].GetType() != ETokenType::INTEGER_CONST )
	{
		PrintError(OutError, "Invalid immediate value", InstrTokens[Index]);
		return false;
	}

	OutVal = InstrTokens[Index].GetAsInt();
	return true;
}

bool MyCPU_ISA::MyCPUInstr::CheckLabelAt(const std::vector<Token>& InstrTokens, unsigned int Index, const ASM_LabelsMap& LabelsMap, ASM_Value_t& OutVal, std::string& OutError) const
{
	if( InstrTokens.size() < Index + 1 )
	{
		PrintError(OutError, "Expected label after", InstrTokens[Index - 1]);
		return false;
	}

	if( InstrTokens[Index].GetType() != ETokenType::IDENTIFIER )
	{
		PrintError(OutError, "Invalid label name", InstrTokens[Index]);
		return false;
	}

	const auto& LIter = LabelsMap.find(InstrTokens[Index].GetLexeme());
	if (LIter == LabelsMap.end())
	{
		PrintError(OutError, "Undefined label name", InstrTokens[Index]);
		return false;
	}

	OutVal = LIter->second;
	return true;
}

bool MyCPU_ISA::MyCPUInstr::CheckCommaAt(const std::vector<Token>& InstrTokens, unsigned int Index, std::string& OutError) const
{
	if( InstrTokens.size() < Index + 1 )
	{
		PrintError(OutError, "Expected ',' after", InstrTokens[Index - 1]);
		return false;
	}
	if( InstrTokens[Index].GetType() != ETokenType::COMMA )
	{
		PrintError(OutError, "Expected ','", InstrTokens[Index]);
		return false;
	}

	return true;
}

bool MyCPU_ISA::MyCPUInstr::CheckSharpAt(const std::vector<Token>& InstrTokens, unsigned int Index, std::string& OutError) const
{
	if( InstrTokens.size() < Index + 1 )
	{
		PrintError(OutError, "Expected '#' after", InstrTokens[Index - 1]);
		return false;
	}
	if( InstrTokens[Index].GetType() != ETokenType::SHARP )
	{
		PrintError(OutError, "Expected '#'", InstrTokens[Index]);
		return false;
	}

	return true;
}

bool MyCPU_ISA::MyCPUInstr::CheckToksCount(const std::vector<Token>& InstrTokens, unsigned int Count, std::string& OutError) const
{
	if( InstrTokens.size() > Count )
	{
		PrintError(OutError, "Unexpected expression", InstrTokens[Count]);
		return false;
	}

	return true;
}

void MyCPU_ISA::MyCPUInstr::CheckImmValRange(const Token& CtxTok, ASM_Value_t Imm, unsigned int Sz, std::string& OutWarning) const
{
	if( !IsValueInBitRange(Imm, Sz) )
	{
		std::string LMsg;
		LMsg = "Imm bit size overflow. Extra bits will be ignored. Available bits ";
		LMsg += std::to_string(Sz);
		PrintError(OutWarning, LMsg, CtxTok);
	}
}

void MyCPU_ISA::MyCPUInstr::CheckImmSignValRange(const Token& CtxTok, ASM_Value_t Imm, unsigned int Sz, std::string& OutWarning) const
{
	if( !IsValueInSignBitRange(Imm, Sz) )
	{
		std::string LMsg;
		LMsg = "Imm bit size overflow. Extra bits will be ignored. Available bits ";
		LMsg += std::to_string(Sz);
		PrintError(OutWarning, LMsg, CtxTok);
	}
}

bool MyCPU_ISA::SUB_X_X::TryEncode(const std::vector<Token>& InstrTokens, const ASM_LabelsMap& LabelsMap, std::vector<uint8_t>& OutCode, std::string& OutError, std::string& OutWarning) const
{
	ASM_Value_t Rd;
	ASM_Value_t Rn;

	if( !CheckMnemonic(InstrTokens, OutError) ) return false;
	if( !CheckRegisterAt(InstrTokens, 1, Rd, OutError) ) return false;
	if( !CheckCommaAt(InstrTokens, 2, OutError) ) return false;
	if( !CheckRegisterAt(InstrTokens, 3, Rn, OutError) ) return false;
	if( !CheckToksCount(InstrTokens, 4, OutError) ) return false;

	//          Rd  Rn
	// 0010011 x|xx xxx 000
	const ASM_Value_t B0 = Concat(Concat(Extract(Rd, 1, 0), Rn, 3), 0, 3);
	const ASM_Value_t B1 = Concat(0b0010011, Extract(Rd, 2, 2), 1);
	OutCode.push_back(static_cast<uint8_t>(B0));
	OutCode.push_back(static_cast<uint8_t>(B1));

	return true;
}

bool MyCPU_ISA::ADD_X_I::TryEncode(const std::vector<Token>& InstrTokens, const ASM_LabelsMap& LabelsMap, std::vector<uint8_t>& OutCode, std::string& OutError, std::string& OutWarning) const
{
	ASM_Value_t Rd;
	ASM_Value_t Imm;

	if( !CheckMnemonic(InstrTokens, OutError) ) return false;
	if( !CheckRegisterAt(InstrTokens, 1, Rd, OutError) ) return false;
	if( !CheckCommaAt(InstrTokens, 2, OutError) ) return false;
	if( !CheckSharpAt(InstrTokens, 3, OutError) ) return false;
	if( !CheckImmAt(InstrTokens, 4, Imm, OutError) ) return false;
	if( !CheckToksCount(InstrTokens, 5, OutError) ) return false;

	CheckImmValRange(InstrTokens[4], Imm, 6, OutWarning);

	//          Rd   Imm
	// 0010000 x|xx xxxxxx
	const ASM_Value_t B0 = Concat(Extract(Rd, 1, 0), Imm, 6);
	const ASM_Value_t B1 = Concat(0b0010000, Extract(Rd, 2, 2), 1);
	OutCode.push_back(static_cast<uint8_t>(B0));
	OutCode.push_back(static_cast<uint8_t>(B1));

	return true;
}

bool MyCPU_ISA::LED_X::TryEncode(const std::vector<Token>& InstrTokens, const ASM_LabelsMap& LabelsMap, std::vector<uint8_t>& OutCode, std::string& OutError, std::string& OutWarning) const
{
	ASM_Value_t Rn;

	if( !CheckMnemonic(InstrTokens, OutError) ) return false;
	if( !CheckRegisterAt(InstrTokens, 1, Rn, OutError) ) return false;
	if( !CheckToksCount(InstrTokens, 2, OutError) ) return false;

	//            Rn
	// 00000010 | xxx 00000
	const ASM_Value_t B0 = Concat(Rn, 0, 5);
	const ASM_Value_t B1 = 0b00000010;
	OutCode.push_back(static_cast<uint8_t>(B0));
	OutCode.push_back(static_cast<uint8_t>(B1));

	return true;
}

bool MyCPU_ISA::J_I::TryEncode(const std::vector<Token>& InstrTokens, const ASM_LabelsMap& LabelsMap, std::vector<uint8_t>& OutCode, std::string& OutError, std::string& OutWarning) const
{
	ASM_Value_t Imm = 0;

	if( !CheckMnemonic(InstrTokens, OutError) ) return false;

	if( InstrTokens.size() < 2 )
	{
		PrintError(OutError, "Expected '#' or '@' after", InstrTokens[0]);
		return false;
	}
	switch( InstrTokens[1].GetType() )
	{
	case ETokenType::SHARP:
	{
		if( !CheckImmAt(InstrTokens, 2, Imm, OutError) ) return false;
		if( !CheckToksCount(InstrTokens, 3, OutError) ) return false;
		break;
	}
	case ETokenType::DOG:
	{
		if( !CheckLabelAt(InstrTokens, 2, LabelsMap, Imm, OutError) ) return false;
		if( !CheckToksCount(InstrTokens, 3, OutError) ) return false;
		break;
	}
	default:
	{
		PrintError(OutError, "Expected '#' or '@'", InstrTokens[1]);
		return false;
	}
	}

	Imm = Imm >> 1;
	CheckImmValRange(InstrTokens[2], Imm, 7, OutWarning);

	//              Imm
	// 10100000 | 0xxxxxxx
	const ASM_Value_t B0 = Concat(0, Imm, 7);
	const ASM_Value_t B1 = 0b10100000;
	OutCode.push_back(static_cast<uint8_t>(B0));
	OutCode.push_back(static_cast<uint8_t>(B1));

	return true;
}

bool MyCPU_ISA::SYS_I::TryEncode(const std::vector<Token>& InstrTokens, const ASM_LabelsMap& LabelsMap, std::vector<uint8_t>& OutCode, std::string& OutError, std::string& OutWarning) const
{
	ASM_Value_t Imm;
	static std ::unordered_map<std::string, ASM_Value_t> SysNameMap = {{"HLT", 0}};

	if( !CheckMnemonic(InstrTokens, OutError) ) return false;

	if( InstrTokens.size() < 2 )
	{
		PrintError(OutError, "Expected sys name after", InstrTokens[0]);
		return false;
	}
	if( InstrTokens[1].GetType() != ETokenType::IDENTIFIER )
	{
		PrintError(OutError, "Expected sys name", InstrTokens[1]);
		return false;
	}
	const auto& LIter = SysNameMap.find(InstrTokens[1].GetLexeme());
	if( LIter == SysNameMap.end() )
	{
		PrintError(OutError, "Invalid sys name", InstrTokens[1]);
		return false;
	}
	else
	{
		Imm = LIter->second;
	}

	//              Imm
	// 00000001 | xxxxxxxx
	const ASM_Value_t B0 = Imm;
	const ASM_Value_t B1 = 0b00000001;
	OutCode.push_back(static_cast<uint8_t>(B0));
	OutCode.push_back(static_cast<uint8_t>(B1));

	return true;
}

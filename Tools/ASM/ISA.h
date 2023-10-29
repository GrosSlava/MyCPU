// Copyright 2022 - 2023 GrosSlava.

#pragma once

#include "Token.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>



using ASM_Value_t = unsigned long long int;
using ASM_LabelsMap = std::unordered_map<std::string, ASM_Value_t>;


/**
	Base class for instruction assembly.
*/
class AsmInstruction
{
public:

	AsmInstruction() = default;
	virtual ~AsmInstruction() = default;


public:

	/**
		@return instruction menemonic in uppercase.
	*/
	virtual std::string GetMnemonic() const = 0;
	virtual bool TryEncode(const std::vector<Token>& InstrTokens, const ASM_LabelsMap& LabelsMap, std::vector<uint8_t>& OutCode, std::string& OutError, std::string& OutWarning) const = 0;

protected:

	void PrintError(std::string& OutError, const std::string& Msg, const Token& Tok) const;

protected:

	inline ASM_Value_t Extract(ASM_Value_t Val, unsigned int Msb, unsigned int Lsb) const noexcept
	{
		const ASM_Value_t Mask = (1 << (Msb + 1)) - 1;
		return (Val & Mask) >> Lsb;
	}
	inline ASM_Value_t Concat(ASM_Value_t Val1, ASM_Value_t Val2, unsigned int Val2Sz) const noexcept
	{
		const ASM_Value_t Val2Mask = (1 << Val2Sz) - 1;
		return (Val1 << Val2Sz) | (Val2 & Val2Mask);
	}
	inline ASM_Value_t MaxBitNumber(unsigned int Sz) const noexcept { return (1 << Sz) - 1; }
	inline bool IsValueInBitRange(ASM_Value_t Val, unsigned int Sz) const noexcept { return Val <= MaxBitNumber(Sz); }
	inline bool IsValueInSignBitRange(ASM_Value_t Val, unsigned int Sz) const noexcept
	{
		const signed long long int SVal = static_cast<signed long long int>(Val);
		const signed long long int SMinVal = -(static_cast<signed long long int>(MaxBitNumber(Sz)));
		const signed long long int SMaxVal = static_cast<signed long long int>(MaxBitNumber(Sz));
		return SVal >= SMinVal && SVal <= SMaxVal;
	}
};



namespace MyCPU_ISA
{
class MyCPUInstr : public AsmInstruction
{
protected:

	/**
		@return register index or -1 if invalid.
	*/
	ASM_Value_t GetRegIndex(const Token& RegToken, std::string& OutError) const;

protected:

	bool CheckMnemonic(const std::vector<Token>& InstrTokens, std::string& OutError) const;
	bool CheckRegisterAt(const std::vector<Token>& InstrTokens, unsigned int Index, ASM_Value_t& OutRegIndex, std::string& OutError) const;
	bool CheckImmAt(const std::vector<Token>& InstrTokens, unsigned int Index, ASM_Value_t& OutVal, std::string& OutError) const;
	bool CheckLabelAt(const std::vector<Token>& InstrTokens, unsigned int Index, const ASM_LabelsMap& LabelsMap, ASM_Value_t& OutVal, std::string& OutError) const;
	bool CheckCommaAt(const std::vector<Token>& InstrTokens, unsigned int Index, std::string& OutError) const;
	bool CheckSharpAt(const std::vector<Token>& InstrTokens, unsigned int Index, std::string& OutError) const;
	bool CheckToksCount(const std::vector<Token>& InstrTokens, unsigned int Count, std::string& OutError) const;

protected:

	void CheckImmValRange(const Token& CtxTok, ASM_Value_t Imm, unsigned int Sz, std::string& OutWarning) const;
	void CheckImmSignValRange(const Token& CtxTok, ASM_Value_t Imm, unsigned int Sz, std::string& OutWarning) const;
};

class SUB_X_X : public MyCPUInstr
{
public:

	virtual std::string GetMnemonic() const override { return "SUB"; }
	virtual bool TryEncode(const std::vector<Token>& InstrTokens, const ASM_LabelsMap& LabelsMap, std::vector<uint8_t>& OutCode, std::string& OutError, std::string& OutWarning) const override;
};
class ADD_X_I : public MyCPUInstr
{
public:

	virtual std::string GetMnemonic() const override { return "ADD"; }
	virtual bool TryEncode(const std::vector<Token>& InstrTokens, const ASM_LabelsMap& LabelsMap, std::vector<uint8_t>& OutCode, std::string& OutError, std::string& OutWarning) const override;
};
class LED_X : public MyCPUInstr
{
public:

	virtual std::string GetMnemonic() const override { return "LED"; }
	virtual bool TryEncode(const std::vector<Token>& InstrTokens, const ASM_LabelsMap& LabelsMap, std::vector<uint8_t>& OutCode, std::string& OutError, std::string& OutWarning) const override;
};
class J_I : public MyCPUInstr
{
public:

	virtual std::string GetMnemonic() const override { return "J"; }
	virtual bool TryEncode(const std::vector<Token>& InstrTokens, const ASM_LabelsMap& LabelsMap, std::vector<uint8_t>& OutCode, std::string& OutError, std::string& OutWarning) const override;
};
class SYS_I : public MyCPUInstr
{
public:

	virtual std::string GetMnemonic() const override { return "SYS"; }
	virtual bool TryEncode(const std::vector<Token>& InstrTokens, const ASM_LabelsMap& LabelsMap, std::vector<uint8_t>& OutCode, std::string& OutError, std::string& OutWarning) const override;
};
} // namespace MyCPU_ISA
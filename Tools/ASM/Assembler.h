// Copyright 2022 - 2023 GrosSlava.

#pragma once

#include "Token.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>



class AsmInstruction;


/**
	Config of assembler.
*/
struct AsmConfig
{
public:

	void AddInstr(const AsmInstruction* Instr);

public:

	/**
		Size of instructions encodings in bytes.
		Support unly instructions with same size.
	*/
	unsigned int InstrSizeBytes = 0;
	/**
		Available ISA for assembly.
		Key - Instr mnemonic.
		Value - Instr assembly handlers.
	*/
	std::unordered_map<std::string, std::vector<const AsmInstruction*>> Instructions;
};

/**
	Info about section.
*/
struct AsmSection
{
	/**
		Section name.
	*/
	std::string Name;
	/**
		Section start address.
	*/
	unsigned long long int Address = 0;
};

/**
	Main class of assembler.
*/
class Assembler
{
public:

	Assembler() = default;
	virtual ~Assembler() = default;


public:

	/**
		Assembly given file into OutCode.

		@param FilePath - File to assembly.
		@param Config - Assembler config.
		@param OutCode - Resulting binary array.
		@param OutSections - Array of sections.
		@return success.
	*/
	bool AssemblyFile(const std::string& FilePath, const AsmConfig& Config, std::vector<uint8_t>& OutCode, std::vector<AsmSection>& OutSections);
	/**
		Assembly given string into OutCode.

		@param SourceStr - Str to assembly.
		@param Config - Assembler config.
		@param OutCode - Resulting binary array.
		@param OutDataStart - Array of sections.
		@return success.
	*/
	bool Assembly(const std::string& SourceStr, const AsmConfig& Config, std::vector<uint8_t>& OutCode, std::vector<AsmSection>& OutSections);

public:

	/**
		Dump lexemes after assembly.
	*/
	void DumpLexemes(const std::string& FileName);

public:

	/**
		@return errors log.
	*/
	inline std::string GetErrorsStr() const noexcept { return ErrorStr; }
	/**
		@return count of errors during assembly.
	*/
	inline int GetErrorsCount() const noexcept { return ErrorsCount; }
	/**
		@return count of warnings during assembly.
	*/
	inline int GetWarningsCount() const noexcept { return WarningsCount; }
	/**
		@return count of assembled instr.
	*/
	inline unsigned long long int GetInstrCount() const noexcept { return InstrRows.size(); }

protected:

	void PrintError(const std::string& Msg);
	void PrintError(const std::string& Msg, unsigned long long int Line, unsigned long long int Pos);
	void PrintWarning(const std::string& Msg);
	void PrintWarning(const std::string& Msg, unsigned long long int Line, unsigned long long int Pos);

protected:

	void Clear();
	void Lex(const AsmConfig& Config, const std::string& SourceStr);
	void Parse1(const AsmConfig& Config, std::vector<AsmSection>& OutSections);
	void Parse2(const AsmConfig& Config, std::vector<uint8_t>& OutCode);



private:

	/**
		Current assemble file.	
	*/
	std::string CurrentFile;

private:

	/**
		String for errors.
	*/
	std::string ErrorStr;
	/**
		Count of errors during assembly.
	*/
	int ErrorsCount = 0;
	/**
		Count of warnings during assembly.
	*/
	int WarningsCount = 0;
	/**
		Flag shows that state is dirty and needs to clean at next execute.
	*/
	bool IsDirty = false;

private:

	std::vector<std::vector<Token>> TokenRows;
	std::unordered_map<std::string, unsigned long long int> LabelsMap;
	std::vector<unsigned long long int> InstrRows;
};
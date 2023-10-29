// Copyright 2022 - 2023 GrosSlava.

#include "Assembler.h"
#include "TokenHelperLibrary.h"
#include "ISA.h"

#include <fstream>





void AsmConfig::AddInstr(const AsmInstruction* Instr)
{
	auto& LIter = Instructions.find(Instr->GetMnemonic());
	if( LIter == Instructions.end() )
	{
		std::vector<const AsmInstruction*> LVariants;
		LVariants.push_back(Instr);
		Instructions.insert(std::make_pair(Instr->GetMnemonic(), std::move(LVariants)));
	}
	else
	{
		LIter->second.push_back(Instr);
	}
}



bool Assembler::AssemblyFile(const std::string& FilePath, const AsmConfig& Config, std::vector<uint8_t>& OutCode, std::vector<AsmSection>& OutSections)
{
	Clear();

	std::ifstream LFile(FilePath, std::ios::binary);
	if( !LFile.is_open() )
	{
		PrintError("Can't open file '" + FilePath + "'!");
		return false;
	}

	const unsigned long long int LFileSize = LFile.seekg(0, std::ios::end).tellg();
	LFile.seekg(0);

	std::string FileContent;
	FileContent.resize(LFileSize);
	LFile.read(&FileContent[0], LFileSize);
	LFile.close();

	CurrentFile = FilePath;
	return Assembly(FileContent, Config, OutCode, OutSections);
}

bool Assembler::Assembly(const std::string& SourceStr, const AsmConfig& Config, std::vector<uint8_t>& outCode, std::vector<AsmSection>& OutSections)
{
	Clear();
	IsDirty = true;

	Lex(Config, SourceStr);
	if( ErrorsCount != 0 ) return false;

	Parse1(Config, OutSections);
	if( ErrorsCount != 0 ) return false;

	Parse2(Config, outCode);
	return ErrorsCount == 0;
}

void Assembler::DumpLexemes(const std::string& FileName)
{
	if( TokenRows.empty() ) return;

	std::ofstream LFile(FileName);
	if( !LFile.is_open() )
	{
		PrintError("Can't open file '" + FileName + "'!");
		return;
	}

	for( const std::vector<Token>& LTokensRow : TokenRows )
	{
		for( const Token& LToken : LTokensRow )
		{
			LFile << "[" << LToken.GetTypeAsStr() << " \"" << LToken.GetLexeme() << "\" ";
			if( LToken.GetType() == ETokenType::INTEGER_CONST )
			{
				LFile << "IntValue=" << LToken.GetAsInt() << " ";
			}
			else if( LToken.GetType() == ETokenType::FLOAT_CONST )
			{
				LFile << "FloatValue=" << LToken.GetAsFloat() << " ";
			}
			LFile << std::to_string(LToken.GetLine()) << "," << std::to_string(LToken.GetPos()) << "] ";
		}

		LFile << "\n";
	}

	LFile.close();
}

void Assembler::PrintError(const std::string& Msg)
{
	IsDirty = true;
	++ErrorsCount;

	if( !CurrentFile.empty() )
	{
		ErrorStr += CurrentFile;
		ErrorStr += ": ";
	}
	ErrorStr += "Error: ";
	ErrorStr += Msg;
	ErrorStr += "\n";
}

void Assembler::PrintError(const std::string& Msg, unsigned long long int Line, unsigned long long int Pos)
{
	IsDirty = true;
	++ErrorsCount;

	if( !CurrentFile.empty() )
	{
		ErrorStr += CurrentFile;
		ErrorStr += ":";
	}
	ErrorStr += std::to_string(Line);
	ErrorStr += ":";
	ErrorStr += std::to_string(Pos);
	ErrorStr += ": Error: ";
	ErrorStr += Msg;
	ErrorStr += "\n";
}

void Assembler::PrintWarning(const std::string& Msg)
{
	IsDirty = true;
	++WarningsCount;

	if( !CurrentFile.empty() )
	{
		ErrorStr += CurrentFile;
		ErrorStr += ": ";
	}
	ErrorStr += "Warning: ";
	ErrorStr += Msg;
	ErrorStr += "\n";
}

void Assembler::PrintWarning(const std::string& Msg, unsigned long long int Line, unsigned long long int Pos)
{
	IsDirty = true;
	++WarningsCount;

	if( !CurrentFile.empty() )
	{
		ErrorStr += CurrentFile;
		ErrorStr += ":";
	}
	ErrorStr += std::to_string(Line);
	ErrorStr += ":";
	ErrorStr += std::to_string(Pos);
	ErrorStr += ": Warning: ";
	ErrorStr += Msg;
	ErrorStr += "\n";
}

void Assembler::Clear()
{
	if( !IsDirty ) return;

	CurrentFile.clear();
	WarningsCount = 0;
	ErrorsCount = 0;
	ErrorStr.clear();
	IsDirty = false;

	TokenRows.clear();
	LabelsMap.clear();
	InstrRows.clear();
}



enum class ELexState : unsigned char
{
	DEFAULT,
	InLineComment,
	SkipRow
};

bool PushCurrentLexeme(std::string& Lexeme, unsigned long long int Line, unsigned long long int Pos, std::vector<Token>& OutTokens)
{
	if( Lexeme.empty() ) return true;

	unsigned long long int LLine = Line;
	unsigned long long int LCurrentPos = Pos - Lexeme.size();

	Token LToken(Lexeme, LLine, LCurrentPos);
	OutTokens.push_back(LToken);
	Lexeme.clear();

	return LToken.GetType() != ETokenType::UNDEFINED;
}

void Assembler::Lex(const AsmConfig& Config, const std::string& SourceStr)
{
	std::string CurrentLexeme;
	std::vector<Token> CurrentRow;

	unsigned long long int Line = 1;
	unsigned long long int Pos = 0;
	ELexState LexState = ELexState::DEFAULT;

	for( char LSym : SourceStr )
	{
		if( LSym == '\n' )
		{
			if( !CurrentRow.empty() )
			{
				TokenRows.push_back(std::move(CurrentRow));
				CurrentRow.clear();
			}

			++Line;
			Pos = 0;
		}
		++Pos;

		if( LSym == '\n' )
		{
			LexState = ELexState::DEFAULT;
		}
		else if( LSym == ';' )
		{
			LexState = ELexState::InLineComment;
		}

		if( LexState == ELexState::InLineComment )
		{
			continue;
		}

		if( FTokenHelperLibrary::IsInvalidChar(LSym) )
		{
			std::string LMsg = "Invalid symbol '";
			LMsg += LSym;
			LMsg += "'";
			PrintError(LMsg, Line, Pos);
			CurrentRow.clear();
			LexState = ELexState::SkipRow;
		}

		if( LexState == ELexState::SkipRow )
		{
			continue;
		}

		if( FTokenHelperLibrary::IsSeparateSymbol(LSym) )
		{
			if( FTokenHelperLibrary::IsWhitespace(LSym) )
			{
				if( !PushCurrentLexeme(CurrentLexeme, Line, Pos - 1, CurrentRow) )
				{
					PrintError("Invalid lexeme", Line, Pos - 1);
					CurrentRow.clear();
					LexState = ELexState::SkipRow;
				}
			}
			else
			{
				if( !PushCurrentLexeme(CurrentLexeme, Line, Pos - 1, CurrentRow) )
				{
					PrintError("Invalid lexeme", Line, Pos - 1);
					CurrentRow.clear();
					LexState = ELexState::SkipRow;
				}
				CurrentLexeme += LSym;
				if( !PushCurrentLexeme(CurrentLexeme, Line, Pos, CurrentRow) )
				{
					PrintError("Invalid lexeme", Line, Pos);
					CurrentRow.clear();
					LexState = ELexState::SkipRow;
				}
			}
		}
		else
		{
			CurrentLexeme += LSym;
		}
	}
	if( !PushCurrentLexeme(CurrentLexeme, Line, Pos, CurrentRow) )
	{
		PrintError("Invalid lexeme", Line, Pos);
		CurrentRow.clear();
	}
	if( !CurrentRow.empty() )
	{
		TokenRows.push_back(std::move(CurrentRow));
		CurrentRow.clear();
	}
}

void Assembler::Parse1(const AsmConfig& Config, std::vector<AsmSection>& OutSections)
{
	unsigned long long int LAddress = 0;
	for( unsigned long long int LRowIndex = 0; LRowIndex < TokenRows.size(); ++LRowIndex )
	{
		const std::vector<Token>& LRow = TokenRows[LRowIndex];
		if( LRow.size() == 0 ) continue;

		if( LRow.size() >= 2 )
		{
			switch( LRow[0].GetType() )
			{
			case ETokenType::IDENTIFIER:
			{
				switch( LRow[1].GetType() )
				{
				case ETokenType::COLON:
				{
					if( LRow.size() > 2 )
					{
						PrintError("Invalid label declaration", LRow[2].GetLine(), LRow[2].GetPos());
					}
					else
					{
						if( LabelsMap.find(LRow[0].GetLexeme()) != LabelsMap.end() )
						{
							PrintError("Label redefinition", LRow[0].GetLine(), LRow[0].GetPos());
						}
						else
						{
							LabelsMap.insert(std::make_pair(LRow[0].GetLexeme(), LAddress));
						}
					}
					break;
				}
				default:
				{
					InstrRows.push_back(LRowIndex);
					LAddress += Config.InstrSizeBytes;
					break;
				}
				}

				break;
			}
			case ETokenType::DOT:
			{
				switch( LRow[1].GetType() )
				{
				case ETokenType::IDENTIFIER:
				{
					if( LRow.size() > 2 )
					{
						PrintError("Invalid section declaration", LRow[2].GetLine(), LRow[2].GetPos());
					}
					else
					{
						//TODO section
					}
					break;
				}
				default:
				{
					PrintError("Expected section name", LRow[1].GetLine(), LRow[1].GetPos());
					break;
				}
				}

				break;
			}
			//TODO data declaration
			default:
			{
				PrintError("Unexpected expression", LRow[0].GetLine(), LRow[0].GetPos());
				break;
			}
			}
		}
		else
		{
			if( LRow[0].GetType() != ETokenType::IDENTIFIER )
			{
				PrintError("Unexpected expression", LRow[0].GetLine(), LRow[0].GetPos());
			}
			else
			{
				InstrRows.push_back(LRowIndex);
				LAddress += Config.InstrSizeBytes;
			}
		}
	}
}

void Assembler::Parse2(const AsmConfig& Config, std::vector<uint8_t>& OutCode)
{
	for( unsigned long long int LInstrRowIndex : InstrRows )
	{
		const std::vector<Token>& LInstrTokens = TokenRows[LInstrRowIndex];

		if( LInstrTokens[0].GetType() != ETokenType::IDENTIFIER )
		{
			PrintError("Expected instruction mnemonic", LInstrTokens[0].GetLine(), LInstrTokens[0].GetPos());
			continue;
		}

		const std::string LMnemonic = LInstrTokens[0].GetLexeme();
		const auto& LVariants = Config.Instructions.find(LMnemonic);
		if( LVariants == Config.Instructions.end() )
		{
			PrintError("Unsupported instruction", LInstrTokens[0].GetLine(), LInstrTokens[0].GetPos());
			continue;
		}
		if( LVariants->second.size() == 0 )
		{
			PrintError("No handlers for instruction", LInstrTokens[0].GetLine(), LInstrTokens[0].GetPos());
			continue;
		}

		if( LVariants->second.size() == 1 )
		{
			std::string LErrorMsg;
			std::string LWarningMsg;
			std::vector<uint8_t> LCode;
			if( !LVariants->second[0]->TryEncode(LInstrTokens, LabelsMap, LCode, LErrorMsg, LWarningMsg) )
			{
				PrintError(LErrorMsg);
				continue;
			}
			if( !LWarningMsg.empty() )
			{
				PrintWarning(LWarningMsg);
			}
			OutCode.insert(OutCode.end(), LCode.begin(), LCode.end());
		}
		else
		{
			bool LSuccess = false;
			for( const AsmInstruction* LInstr : LVariants->second )
			{
				std::string LErrorMsg;
				std::string LWarningMsg;
				std::vector<uint8_t> LCode;
				if( LInstr->TryEncode(LInstrTokens, LabelsMap, LCode, LErrorMsg, LWarningMsg) )
				{
					// Select first success and ignore next

					if( !LWarningMsg.empty() )
					{
						PrintWarning(LWarningMsg);
					}

					OutCode.insert(OutCode.end(), LCode.begin(), LCode.end());
					LSuccess = true;
					break;
				}
			}

			if( !LSuccess )
			{
				PrintError("Can't recognise instruction", LInstrTokens[0].GetLine(), LInstrTokens[0].GetPos());
				continue;
			}
		}
	}
}

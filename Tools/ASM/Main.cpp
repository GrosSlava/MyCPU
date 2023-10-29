// Copyright 2022 - 2023 GrosSlava.

#include "Assembler.h"
#include "ISA.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>





bool SaveBinaryFile(const std::vector<uint8_t>& Code, const std::string& File)
{
	std::ofstream LOut(File, std::ios::binary);
	if( !LOut.is_open() )
	{
		fprintf(stderr, "Cant create file '%s'!\n", File.c_str());
		return false;
	}

	for( uint8_t c : Code )
	{
		LOut << c;
	}

	LOut.close();
	return true;
}

void FillSupportedInstr(AsmConfig& Config)
{
	Config.AddInstr(new MyCPU_ISA::SUB_X_X());
	Config.AddInstr(new MyCPU_ISA::ADD_X_I());
	Config.AddInstr(new MyCPU_ISA::LED_X());
	Config.AddInstr(new MyCPU_ISA::J_I());
	Config.AddInstr(new MyCPU_ISA::SYS_I());
}

int main(int argc, char** argv)
{
	if( argc != 3 )
	{
		fprintf(stdout, "usage: ASM <source> <dest>\n");
		return EXIT_SUCCESS;
	}

	std::vector<uint8_t> Code;
	std::vector<AsmSection> Sections;

	AsmConfig LConfig;
	LConfig.InstrSizeBytes = 2;
	FillSupportedInstr(LConfig);

	Assembler LAssembler;
	const bool Success = LAssembler.AssemblyFile(argv[1], LConfig, Code, Sections);
	//LAssembler.DumpLexemes("Lex.txt");

	if( Success )
	{
		if( LAssembler.GetWarningsCount() > 0 )
		{
			fprintf(stdout, "%s", LAssembler.GetErrorsStr().c_str());
			fprintf(stdout, "Warnings count: %d\n", LAssembler.GetWarningsCount());
		}

		if( !SaveBinaryFile(Code, argv[2]) )
		{
			return EXIT_FAILURE;
		}

		fprintf(stdout, "Assembled instructions count: %llu\n", LAssembler.GetInstrCount());
		fprintf(stdout, "Use %llu bytes\n", Code.size());
		return EXIT_SUCCESS;
	}
	else
	{
		fprintf(stderr, "%s", LAssembler.GetErrorsStr().c_str());
		fprintf(stderr, "Errors count: %d\n", LAssembler.GetErrorsCount());
		fprintf(stderr, "Warnings count: %d\n", LAssembler.GetWarningsCount());
		return EXIT_FAILURE;
	}
}
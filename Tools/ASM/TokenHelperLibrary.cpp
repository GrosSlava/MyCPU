// Copyright 2022 - 2023 GrosSlava.

#include "TokenHelperLibrary.h"

#include <cstdlib>
#include <cerrno>





bool FTokenHelperLibrary::IsIntegerLexeme(const std::string& Lexeme, unsigned long long int& outInt) noexcept
{
	char* endptr;
	errno = 0;
	outInt = strtoull(Lexeme.c_str(), &endptr, 0);

	if( errno != 0 || *endptr != '\0' )
	{
		outInt = 0;
		return false;
	}

	return true;
}

bool FTokenHelperLibrary::IsFloatLexeme(const std::string& Lexeme, double& outFloat) noexcept
{
	char* endptr;
	errno = 0;
	outFloat = strtod(Lexeme.c_str(), &endptr);

	if( errno != 0 || *endptr != '\0' )
	{
		outFloat = 0.0;
		return false;
	}

	return true;
}
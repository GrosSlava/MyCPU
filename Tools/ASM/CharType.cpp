// Copyright 2022 - 2023 GrosSlava.

#include "CharType.h"





// clang-format off
const FCharType CharInfoTable[256] = 
{
	FCharType(ECharType::WhiteSpace),														// '\0' 
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(ECharType::WhiteSpace),														// '\b'
	FCharType(ECharType::WhiteSpace),														// '\t'
	FCharType(ECharType::WhiteSpace),														// '\n'
	FCharType(ECharType::WhiteSpace),														// '\v'
	FCharType(ECharType::WhiteSpace),														// '\f'
	FCharType(ECharType::WhiteSpace),														// '\r'
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(ECharType::WhiteSpace),														// ' '
	FCharType(ECharType::OperatorChar),														// '!'
	FCharType(0),																			//
	FCharType(ECharType::OperatorChar),														// '#' 
	FCharType(ECharType::OperatorChar),														// '$'
	FCharType(ECharType::OperatorChar),														// '%'
	FCharType(ECharType::OperatorChar),														// '&'
	FCharType(0),																			//
	FCharType(ECharType::OperatorChar | ECharType::LeftBracket),							// '(' 
	FCharType(ECharType::OperatorChar | ECharType::RightBracket),							// ')' 
	FCharType(ECharType::OperatorChar),														// '*'
	FCharType(ECharType::Sign), // ECharType::OperatorChar						         	// '+'
	FCharType(ECharType::OperatorChar),														// ','
	FCharType(ECharType::Sign),	// ECharType::OperatorChar								    // '-'
	FCharType(ECharType::OperatorChar),														// '.'
	FCharType(ECharType::OperatorChar),														// '/'
	FCharType(ECharType::BinaryDigit | ECharType::DecimalDigit | ECharType::HexDigit),		// '0' 
	FCharType(ECharType::BinaryDigit | ECharType::DecimalDigit | ECharType::HexDigit),		// '1'
	FCharType(ECharType::DecimalDigit | ECharType::HexDigit),								// '2'
	FCharType(ECharType::DecimalDigit | ECharType::HexDigit),								// '3'
	FCharType(ECharType::DecimalDigit | ECharType::HexDigit),								// '4'
	FCharType(ECharType::DecimalDigit | ECharType::HexDigit),								// '5'
	FCharType(ECharType::DecimalDigit | ECharType::HexDigit),								// '6'
	FCharType(ECharType::DecimalDigit | ECharType::HexDigit),								// '7'
	FCharType(ECharType::DecimalDigit | ECharType::HexDigit),								// '8'
	FCharType(ECharType::DecimalDigit | ECharType::HexDigit),								// '9'
	FCharType(ECharType::OperatorChar),														// ':'
	FCharType(ECharType::OperatorChar),														// ';' 
	FCharType(ECharType::OperatorChar),														// '<'
	FCharType(ECharType::OperatorChar),														// '='
	FCharType(ECharType::OperatorChar),														// '>'
	FCharType(ECharType::OperatorChar),														// '?' 
	FCharType(ECharType::OperatorChar),														// '@'
	FCharType(ECharType::Letter | ECharType::HexDigit),										// 'A' 
	FCharType(ECharType::Letter | ECharType::HexDigit),										// 'B' 
	FCharType(ECharType::Letter | ECharType::HexDigit),										// 'C' 
	FCharType(ECharType::Letter | ECharType::HexDigit),										// 'D' 
	FCharType(ECharType::Letter | ECharType::HexDigit),										// 'E' 
	FCharType(ECharType::Letter | ECharType::HexDigit),										// 'F'
	FCharType(ECharType::Letter),															// 'G'
	FCharType(ECharType::Letter),															// 'H' 
	FCharType(ECharType::Letter),															// 'I' 
	FCharType(ECharType::Letter),															// 'J' 
	FCharType(ECharType::Letter),															// 'K' 
	FCharType(ECharType::Letter),															// 'L' 
	FCharType(ECharType::Letter),															// 'M'
	FCharType(ECharType::Letter),															// 'N' 
	FCharType(ECharType::Letter),															// 'O' 
	FCharType(ECharType::Letter),															// 'P' 
	FCharType(ECharType::Letter),															// 'Q' 
	FCharType(ECharType::Letter),															// 'R' 
	FCharType(ECharType::Letter),															// 'S' 
	FCharType(ECharType::Letter),															// 'T'
	FCharType(ECharType::Letter),															// 'U' 
	FCharType(ECharType::Letter),															// 'V' 
	FCharType(ECharType::Letter),															// 'W' 
	FCharType(ECharType::Letter),															// 'X' 
	FCharType(ECharType::Letter),															// 'Y' 
	FCharType(ECharType::Letter),															// 'Z'
	FCharType(ECharType::OperatorChar | ECharType::LeftBracket),							// '[' 
	FCharType(ECharType::OperatorChar),														// '\' 
	FCharType(ECharType::OperatorChar | ECharType::RightBracket),							// ']' 
	FCharType(ECharType::OperatorChar),														// '^' 
	FCharType(0),																			//
	FCharType(0),																			//
	FCharType(ECharType::Letter | ECharType::HexDigit),										// 'a'
	FCharType(ECharType::Letter | ECharType::HexDigit),										// 'b' 
	FCharType(ECharType::Letter | ECharType::HexDigit),										// 'c' 
	FCharType(ECharType::Letter | ECharType::HexDigit),										// 'd' 
	FCharType(ECharType::Letter | ECharType::HexDigit),										// 'e' 
	FCharType(ECharType::Letter | ECharType::HexDigit),										// 'f' 
	FCharType(ECharType::Letter),															// 'g' 
	FCharType(ECharType::Letter),															// 'h'
	FCharType(ECharType::Letter),															// 'i'
	FCharType(ECharType::Letter),															// 'j' 
	FCharType(ECharType::Letter),															// 'k' 
	FCharType(ECharType::Letter),															// 'l' 
	FCharType(ECharType::Letter),															// 'm' 
	FCharType(ECharType::Letter),															// 'n' 
	FCharType(ECharType::Letter),															// 'o' 
	FCharType(ECharType::Letter),															// 'p'
	FCharType(ECharType::Letter),															// 'q' 
	FCharType(ECharType::Letter),															// 'r' 
	FCharType(ECharType::Letter),															// 's' 
	FCharType(ECharType::Letter),															// 't'
	FCharType(ECharType::Letter),															// 'u'
	FCharType(ECharType::Letter),															// 'v'
	FCharType(ECharType::Letter),															// 'w'
	FCharType(ECharType::Letter),															// 'x' 
	FCharType(ECharType::Letter),															// 'y'
	FCharType(ECharType::Letter),															// 'z'
	FCharType(ECharType::OperatorChar | ECharType::LeftBracket),							// '{'
	FCharType(ECharType::OperatorChar),														// '|' 
	FCharType(ECharType::OperatorChar | ECharType::LeftBracket),							// '}'
	FCharType(ECharType::OperatorChar),														// '~'
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0),
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0),
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0),
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0),
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0),
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0),
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0),
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0),
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0),
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0),
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0),
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0),
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0),
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0),
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0),
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0),
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0),
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0),
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0),
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0), 
	FCharType(0)
};
// clang-format on
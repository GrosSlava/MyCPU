// Copyright 2022 - 2023 GrosSlava.

#pragma once

#include <string>



/**
	All language tokens.
*/
enum class ETokenType
{
	// clang-format off
	UNDEFINED,
	IDENTIFIER,

	// constants
	INTEGER_CONST,              // 1, 2, 3..., 0xff, 0XAbC, 0b11101, 0B0001
	FLOAT_CONST,                // 1.34, 0.110, 123.456e3, 123.456E3, 123.456e+3, 123.456E+3, 123.456e-3, 123.456E-3
	CHAR_CONST,                 // 'a', '\n'
	STRING_CONST,               // "abcd", "abc\n"

	// brackets
	LBRA,                       // {
	RBRA,                       // }
	LPAR,                       // (
	RPAR,                       // )
	LSQR,                       // [
	RSQR,                       // ]

	// special symbols
	SEMICOLON,                  // ;
	COLON,                      // :
	COMMA,                      // ,
	DOT,                        // .
	QUESTION,                   // ?
	DOLLAR,						// $
	SHARP,						// #
	DOG,						// @
	SLASH,						// /
	// clang-format on
};


/**
	Base lexeme from source code.
	@see ETokenType, FTokenHelperLibrary.
*/
struct Token
{
public:

	Token() = delete;
	inline Token(const std::string& InLexeme, unsigned long long int InLine, unsigned long long int InPos) : LexemeStr(InLexeme), Line(InLine), Pos(InPos) { DetermineTokenType(); }
	Token(const Token& Other) : Type(Other.Type), LexemeStr(Other.LexemeStr), IntValue(Other.IntValue), FloatValue(Other.FloatValue), Line(Other.Line), Pos(Other.Pos) { }
	Token(Token&& Other) noexcept : Type(Other.Type), LexemeStr(std::move(Other.LexemeStr)), IntValue(Other.IntValue), FloatValue(Other.FloatValue), Line(Other.Line), Pos(Other.Pos)
	{
		Other.Type = ETokenType::UNDEFINED;
		Other.IntValue = 0;
		Other.FloatValue = 0.0;
		Other.Line = 0;
		Other.Pos = 0;
	}

public:

	Token& operator=(const Token& Other)
	{
		if( this == &Other )
		{
			return *this;
		}

		Type = Other.Type;
		LexemeStr = Other.LexemeStr;
		IntValue = Other.IntValue;
		FloatValue = Other.FloatValue;
		Line = Other.Line;
		Pos = Other.Pos;

		return *this;
	}
	Token& operator=(Token&& Other) noexcept
	{
		Type = Other.Type;
		LexemeStr = std::move(Other.LexemeStr);
		IntValue = Other.IntValue;
		FloatValue = Other.FloatValue;
		Line = Other.Line;
		Pos = Other.Pos;

		Other.IntValue = 0;
		Other.FloatValue = 0.0;
		Other.Type = ETokenType::UNDEFINED;
		Other.Line = 0;
		Other.Pos = 0;

		return *this;
	}


public:

	/**
		@return token type in str.
	*/
	std::string GetTypeAsStr() const noexcept;
	/**
		@return token type.
	*/
	inline ETokenType GetType() const noexcept { return Type; }
	/**
		@return original lexeme str.
	*/
	inline const std::string& GetLexeme() const noexcept { return LexemeStr; }
	/**
		@return integer value.
	*/
	inline unsigned long long int GetAsInt() const noexcept { return IntValue; }
	/**
		@return float valie.
	*/
	inline double GetAsFloat() const noexcept { return FloatValue; }
	/**
		@return token line index in file.
	*/
	inline unsigned long long int GetLine() const noexcept { return Line; }
	/**
		@return token position in line.
	*/
	inline unsigned long long int GetPos() const noexcept { return Pos; }
	/**
		@return hash from current lexeme str.
	*/
	inline unsigned long long int GetHash() const noexcept { return std::hash<std::string> {}(LexemeStr); }

private:

	/**
		Set Type based on LexemeStr.
	*/
	void DetermineTokenType();



private:

	/**
		Token type.
	*/
	ETokenType Type = ETokenType::UNDEFINED;
	/**
		Token value from source code.
	*/
	std::string LexemeStr;
	/**
		Token value as integer.
	*/
	unsigned long long int IntValue = 0;
	/**
		Token value as float.
	*/
	double FloatValue = 0.0;

	/**
		Token line index.
	*/
	unsigned long long int Line = 0;
	/**
		Token start position in text file line.
	*/
	unsigned long long int Pos = 0;
};
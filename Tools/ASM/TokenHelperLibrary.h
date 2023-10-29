// Copyright 2022 - 2023 GrosSlava.

#pragma once

#include "CharType.h"

#include <string>



/**
	Helper library for tokens char/string operations.
	Operate only with char/string.
	@see ETokenType.
*/
struct FTokenHelperLibrary
{
	/**
		Chech that c is whitespace or special not visible symbol.
	*/
	static inline bool IsWhitespace(const unsigned char c) noexcept { return CharInfoTable[c].IsWhitespace(); }
	/**
		Chech that c is single-symbol operator.
	*/
	static inline bool IsOperatorChar(const unsigned char c) noexcept { return CharInfoTable[c].IsOperator(); }
	/**
		Check that c is letter.
	*/
	static inline bool IsLetter(const unsigned char c) noexcept { return CharInfoTable[c].IsLetter(); }
	/**
		Check that c is digit.
	*/
	static inline bool IsDigit(const unsigned char c) noexcept { return CharInfoTable[c].IsDecimalDigit(); }
	/**
		Check that c is letter or digit.
	*/
	static inline bool IsLetterOrDigit(const unsigned char c) noexcept { return IsLetter(c) || IsDigit(c); }
	/**
		Check that c is left bracket like '(' or '[' ...
	*/
	static inline bool IsLeftBracket(const unsigned char c) noexcept { return CharInfoTable[c].IsLeftBracket(); }
	/**
		Check that c is right bracket like ')' or ']' ...
	*/
	static inline bool IsRightBracket(const unsigned char c) noexcept { return CharInfoTable[c].IsRightBracket(); }
	/**
		Check that c is any single-symbol bracket.
	*/
	static inline bool IsBracket(const unsigned char c) noexcept { return IsLeftBracket(c) || IsRightBracket(c); }
	/**
		Check that c is sign.
	*/
	static inline bool IsSign(const unsigned char c) noexcept { return CharInfoTable[c].IsSign(); }
	/**
		Check that c is symbol which separate tokens.
	*/
	static inline bool IsSeparateSymbol(const unsigned char c) noexcept { return IsWhitespace(c) || IsOperatorChar(c); }
	/**
		Check that c is incorrect char.
	*/
	static inline bool IsInvalidChar(const unsigned char c) noexcept
	{
		// clang-format off
		return  !IsWhitespace(c)	&&
				!IsOperatorChar(c)	&&
				!IsLetter(c)		&&
				!IsDigit(c)			&&
				!IsBracket(c)		&&
				!IsSign(c)			&&
				(c != '_')			&&
				(c != '"')			&&
				(c != '\'');
		// clang-format on
	}
	/**
		Check that identifier is named correctly.
	*/
	static inline bool IsCorrectIdentifier(const std::string& s) noexcept
	{
		if( s.empty() || (!IsLetter(s[0]) && s[0] != '_') ) return false;

		for( const char c : s )
		{
			if( !IsLetter(c) && !IsDigit(c) && c != '_' )
			{
				return false;
			}
		}

		return true;
	}
	/**
		Check that Lexeme is wrapped by '"'.

		@param Lexeme - Lexeme str to check.
	*/
	static inline bool IsStringLexeme(const std::string& Lexeme) noexcept { return Lexeme.size() >= 2 && Lexeme.front() == '"' && Lexeme.back() == '"'; }
	/**
		Check that Lexeme is wrapped by '\''.

		@param Lexeme - Lexeme str to check.
	*/
	static inline bool IsCharLexeme(const std::string& Lexeme) noexcept { return Lexeme.size() >= 2 && Lexeme.front() == '\'' && Lexeme.back() == '\''; }
	/**
		Check that Lexeme is integer.

		@param Lexeme - Lexeme str to check.
	*/
	static bool IsIntegerLexeme(const std::string& Lexeme, unsigned long long int& outInt) noexcept;
	/**
		Check that Lexeme is float.

		@param Lexeme - Lexeme str to check.
	*/
	static bool IsFloatLexeme(const std::string& Lexeme, double& outFloat) noexcept;
};
// Copyright 2022 - 2023 GrosSlava.

#include "Token.h"
#include "TokenHelperLibrary.h"

#include <unordered_map>





std::string Token::GetTypeAsStr() const noexcept
{
	// clang-format off
#define CASE_TOKEN(TokenType, Str) case ETokenType::TokenType: return Str;

	switch( Type )
	{
	CASE_TOKEN(UNDEFINED,				"UNDEFINED")
	CASE_TOKEN(IDENTIFIER,				"IDENTIFIER")
	CASE_TOKEN(INTEGER_CONST,			"INTEGER_CONST")
	CASE_TOKEN(FLOAT_CONST,				"FLOAT_CONST")
	CASE_TOKEN(CHAR_CONST,				"CHAR_CONST")
	CASE_TOKEN(STRING_CONST,			"STRING_CONST")
	CASE_TOKEN(LBRA,					"LBRA")
	CASE_TOKEN(RBRA,					"RBRA")
	CASE_TOKEN(LPAR,					"LPAR")
	CASE_TOKEN(RPAR,					"RPAR")
	CASE_TOKEN(LSQR,					"LSQR")
	CASE_TOKEN(RSQR,					"RSQR")
	CASE_TOKEN(SEMICOLON,				"SEMICOLON")
	CASE_TOKEN(COLON,					"COLON")
	CASE_TOKEN(COMMA,					"COMMA")
	CASE_TOKEN(DOT,						"DOT")
	CASE_TOKEN(QUESTION,				"QUESTION")
	CASE_TOKEN(DOLLAR,					"DOLLAR")
	CASE_TOKEN(SHARP,					"SHARP")
	CASE_TOKEN(DOG,						"DOG")
	CASE_TOKEN(SLASH,					"SLASH")
	}
	// clang-format on

	return "";
}

void Token::DetermineTokenType()
{
	/*
		Table for token type hashing.
		Key - hash of token lexeme.
		Value - token type.
	*/
	static std::unordered_map<unsigned long long int, ETokenType> TokenTypeCache;

	// clang-format off
	if( FTokenHelperLibrary::IsIntegerLexeme(LexemeStr, IntValue) )	{ Type = ETokenType::INTEGER_CONST;	return;	}
	if( FTokenHelperLibrary::IsFloatLexeme(LexemeStr, FloatValue) )	{ Type = ETokenType::FLOAT_CONST;	return; }
	if( FTokenHelperLibrary::IsCharLexeme(LexemeStr) )		        { Type = ETokenType::CHAR_CONST;	return; }
	if( FTokenHelperLibrary::IsStringLexeme(LexemeStr) )	        { Type = ETokenType::STRING_CONST;	return; }
	// clang-format on

	const size_t LLexemeHash = GetHash();
	auto LTokenTypeCacheIter = TokenTypeCache.find(LLexemeHash);
	if( LTokenTypeCacheIter != TokenTypeCache.end() )
	{
		Type = LTokenTypeCacheIter->second;
		return;
	}


#define CASE_LEXEME(Lexeme, TokenType)                       \
	if( LexemeStr == Lexeme )                                \
	{                                                        \
		Type = ETokenType::TokenType;                        \
		TokenTypeCache.insert(std::pair(LLexemeHash, Type)); \
		return;                                              \
	}

	// clang-format off
	CASE_LEXEME("{",				LBRA)
	CASE_LEXEME("}",				RBRA)
	CASE_LEXEME("(",				LPAR)
	CASE_LEXEME(")",				RPAR)
	CASE_LEXEME("{",				LSQR)
	CASE_LEXEME("}",				RSQR)
	CASE_LEXEME(";",				SEMICOLON)
	CASE_LEXEME(":",				COLON)
	CASE_LEXEME(",",				COMMA)
	CASE_LEXEME(".",				DOT)
	CASE_LEXEME("?",				QUESTION)
	CASE_LEXEME("$",				DOLLAR)
	CASE_LEXEME("#",				SHARP)
	CASE_LEXEME("@",				DOG)
	CASE_LEXEME("/",				SLASH)
	// clang-format on

	if( FTokenHelperLibrary::IsCorrectIdentifier(LexemeStr) )
	{
		for( char& c : LexemeStr )
		{
			c = toupper(c);
		}

		Type = ETokenType::IDENTIFIER;
		return;
	}


	Type = ETokenType::UNDEFINED;
}
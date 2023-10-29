// Copyright 2022 - 2023 GrosSlava.

#pragma once



/**
	Helper enum for constructing mask of char type.
*/
namespace ECharType
{
enum
{
	WhiteSpace = 1 << 0,
	OperatorChar = 1 << 1,
	Letter = 1 << 2,
	BinaryDigit = 1 << 3,
	DecimalDigit = 1 << 4,
	HexDigit = 1 << 5,
	LeftBracket = 1 << 6,
	RightBracket = 1 << 7,
	Sign = 1 << 8
};
} // namespace ECharType

/**
	Info about char.
*/
struct FCharType
{
public:

	FCharType() = default;
	inline explicit FCharType(int InMask) noexcept : Mask(InMask) { }


public:

	inline bool IsWhitespace() const noexcept { return Mask & ECharType::WhiteSpace; }
	inline bool IsOperator() const noexcept { return Mask & ECharType::OperatorChar; }
	inline bool IsLetter() const noexcept { return Mask & ECharType::Letter; }
	inline bool IsBinaryDigit() const noexcept { return Mask & ECharType::BinaryDigit; }
	inline bool IsDecimalDigit() const noexcept { return Mask & ECharType::DecimalDigit; }
	inline bool IsHexDigit() const noexcept { return Mask & ECharType::HexDigit; }
	inline bool IsLeftBracket() const noexcept { return Mask & ECharType::LeftBracket; }
	inline bool IsRightBracket() const noexcept { return Mask & ECharType::RightBracket; }
	inline bool IsSign() const noexcept { return Mask & ECharType::Sign; }




private:

	/**
		Char type bit-mask. Each bit represents one type.
	*/
	unsigned int Mask = 0;
};

/**
	Table of char info for fast access.
	Key - char value as unsigned int.
*/
extern const FCharType CharInfoTable[256];
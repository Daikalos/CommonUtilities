#pragma once

#include <cstdint>

#include <CommonUtilities/Config.h>
#include <CommonUtilities/Math/Vector4.hpp>

namespace CommonUtilities
{
	/// Stores color as unsigned 8-bit variables instead of floats for lesser memory usage.
	/// 
	class Color
	{
	public:
		std::uint8_t r{0};
		std::uint8_t g{0};
		std::uint8_t b{0};
		std::uint8_t a{255};

		constexpr Color();
		constexpr ~Color();

		constexpr Color(std::uint8_t aRed, std::uint8_t aGreen, std::uint8_t aBlue, std::uint8_t aAlpha = 255);
		constexpr explicit Color(std::uint32_t aColor);

		constexpr std::uint32_t ToInteger() const noexcept;

		template<template<typename> class T, typename U>
		constexpr T<U> ToFloatingPoint() const;

		static const Color Black;       
		static const Color White;       
		static const Color Red;         
		static const Color Green;       
		static const Color Blue;        
		static const Color Yellow;      
		static const Color Magenta;     
		static const Color Cyan;        
		static const Color Transparent; 
	};

	constexpr Color::Color() = default;

	constexpr Color::~Color() = default;

	constexpr Color::Color(std::uint8_t aRed, std::uint8_t aGreen, std::uint8_t aBlue, std::uint8_t aAlpha)
		: r(aRed)
		, g(aGreen)
		, b(aBlue)
		, a(aAlpha)
	{

	}

	constexpr Color::Color(std::uint32_t aColor)
		: r(static_cast<std::uint8_t>((aColor & 0xff000000) >> 24))
		, g(static_cast<std::uint8_t>((aColor & 0x00ff0000) >> 16))
		, b(static_cast<std::uint8_t>((aColor & 0x0000ff00) >> 8))
		, a(static_cast<std::uint8_t>(aColor & 0x000000ff))
	{
	}

	constexpr std::uint32_t Color::ToInteger() const noexcept
	{
		return static_cast<std::uint32_t>((r << 24) | (g << 16) | (b << 8) | a);
	}
	template<template<typename> class T, typename U>
	constexpr T<U> Color::ToFloatingPoint() const
	{
		return T<U>
		{ 
			static_cast<U>(r), 
			static_cast<U>(g), 
			static_cast<U>(b), 
			static_cast<U>(a) 
		} * (1 / U{255.0});
	}

	// GLOBAL OPERATORS

	constexpr bool operator==(const Color& aLeft, const Color& aRight)
	{
		return (aLeft.r == aRight.r) && (aLeft.g == aRight.g) && (aLeft.b == aRight.b) && (aLeft.a == aRight.a);
	}

	constexpr bool operator!=(const Color& aLeft, const Color& aRight)
	{
		return !(aLeft == aRight);
	}

	constexpr Color operator+(const Color& aLeft, const Color& aRight)
	{
		const auto clampedAdd = [](std::uint8_t aLhs, std::uint8_t aRhs) -> std::uint8_t
		{
			const int result = int(aLhs) + int(aRhs);
			return static_cast<std::uint8_t>(result < 255 ? result : 255);
		};

		return Color 
		{ 
			clampedAdd(aLeft.r, aRight.r),
			clampedAdd(aLeft.g, aRight.g),
			clampedAdd(aLeft.b, aRight.b),
			clampedAdd(aLeft.a, aRight.a)
		};
	}

	constexpr Color operator-(const Color& aLeft, const Color& aRight)
	{
		const auto clampedSub = [](std::uint8_t aLhs, std::uint8_t aRhs) -> std::uint8_t
		{
			const int result = int(aLhs) - int(aRhs);
			return static_cast<std::uint8_t>(result > 0 ? result : 0);
		};

		return Color
		{
			clampedSub(aLeft.r, aRight.r),
			clampedSub(aLeft.g, aRight.g),
			clampedSub(aLeft.b, aRight.b),
			clampedSub(aLeft.a, aRight.a)
		};
	}

	constexpr Color operator*(const Color& aLeft, const Color& aRight)
	{
		const auto scaledMul = [](std::uint8_t aLhs, std::uint8_t aRhs) -> std::uint8_t
		{
			const std::uint16_t result = static_cast<std::uint16_t>(std::uint16_t(aLhs) * std::uint16_t(aRhs));
			return static_cast<std::uint8_t>(result / 255u);
		};

		return Color
		{
			scaledMul(aLeft.r, aRight.r),
			scaledMul(aLeft.g, aRight.g),
			scaledMul(aLeft.b, aRight.b),
			scaledMul(aLeft.a, aRight.a)
		};
	}

	constexpr Color operator+=(Color& aLeft, const Color& aRight)
	{
		return aLeft = aLeft + aRight;
	}

	constexpr Color operator-=(Color& aLeft, const Color& aRight)
	{
		return aLeft = aLeft - aRight;
	}

	constexpr Color operator*=(Color& aLeft, const Color& aRight)
	{
		return aLeft = aLeft * aRight;
	}

	inline constexpr Color Color::Black(		0,		0,		0);
	inline constexpr Color Color::White(		255,	255,	255);
	inline constexpr Color Color::Red(			255,	0,		0);
	inline constexpr Color Color::Green(		0,		255,	0);
	inline constexpr Color Color::Blue(			0,		0,		255);
	inline constexpr Color Color::Yellow(		255,	255,	0);
	inline constexpr Color Color::Magenta(		255,	0,		255);
	inline constexpr Color Color::Cyan(			0,		255,	255);
	inline constexpr Color Color::Transparent(	0,		0,		0,		0);
}
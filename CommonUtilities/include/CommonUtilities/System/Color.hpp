#pragma once

#include <cstdint>

#include <CommonUtilities/Config.h>
#include <CommonUtilities/Math/Vector4.hpp>

namespace CommonUtilities
{
	/// Stores color as unsigned 8-bit variables instead of floats for smaller memory usage.
	/// 
	class Color
	{
	public:
		std::uint8_t r{0};
		std::uint8_t g{0};
		std::uint8_t b{0};
		std::uint8_t a{255};

		constexpr Color() = default;
		constexpr ~Color() = default;

		constexpr Color(std::uint8_t aRed, std::uint8_t aGreen, std::uint8_t aBlue, std::uint8_t aAlpha = 255);
		constexpr explicit Color(std::uint32_t aColor);

		constexpr std::uint32_t ToInteger() const noexcept;

		template<class T>
		constexpr Vector4<T> ToNormalized() const;

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
	template<class T>
	constexpr Vector4<T> Color::ToNormalized() const
	{
		return Vector4<T>
		{ 
			static_cast<T>(r), 
			static_cast<T>(g), 
			static_cast<T>(b), 
			static_cast<T>(a) 
		} * (1 / T{255.0});
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
		const auto ClampedAdd = [](std::uint8_t aLhs, std::uint8_t aRhs) -> std::uint8_t
		{
			const std::uint16_t result = (std::uint16_t)(std::uint16_t(aLhs) + std::uint16_t(aRhs));
			return static_cast<std::uint8_t>(result < 255 ? result : 255);
		};

		return Color 
		{ 
			ClampedAdd(aLeft.r, aRight.r),
			ClampedAdd(aLeft.g, aRight.g),
			ClampedAdd(aLeft.b, aRight.b),
			ClampedAdd(aLeft.a, aRight.a)
		};
	}

	constexpr Color operator-(const Color& aLeft, const Color& aRight)
	{
		const auto ClampedSub = [](std::uint8_t aLhs, std::uint8_t aRhs) -> std::uint8_t
		{
			const std::uint16_t result = (std::int16_t)(std::int16_t(aLhs) - std::int16_t(aRhs));
			return static_cast<std::uint8_t>(result > 0 ? result : 0);
		};

		return Color
		{
			ClampedSub(aLeft.r, aRight.r),
			ClampedSub(aLeft.g, aRight.g),
			ClampedSub(aLeft.b, aRight.b),
			ClampedSub(aLeft.a, aRight.a)
		};
	}

	constexpr Color operator*(const Color& aLeft, const Color& aRight)
	{
		const auto ScaledMul = [](std::uint8_t aLhs, std::uint8_t aRhs) -> std::uint8_t
		{
			const std::uint16_t result = (std::uint16_t)(std::uint16_t(aLhs) * std::uint16_t(aRhs));
			return static_cast<std::uint8_t>(result / 255u);
		};

		return Color
		{
			ScaledMul(aLeft.r, aRight.r),
			ScaledMul(aLeft.g, aRight.g),
			ScaledMul(aLeft.b, aRight.b),
			ScaledMul(aLeft.a, aRight.a)
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
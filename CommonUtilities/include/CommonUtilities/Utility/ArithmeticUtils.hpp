#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <CommonUtilities/Utility/Concepts.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<IsFloatingPoint T = float>
	CONSTEXPR T PI = static_cast<T>(M_PI);

	template<IsFloatingPoint T = float>
	CONSTEXPR T PI_2 = static_cast<T>(M_PI_2);

	template<IsFloatingPoint T = float>
	CONSTEXPR T PI_4 = static_cast<T>(M_PI_4);

	template<IsFloatingPoint T = float> 
	CONSTEXPR T DEG2RAD = PI<T> / T(180.0);

	template<IsFloatingPoint T = float>
	CONSTEXPR T RAD2DEG = T(180.0) / PI<T>;

	template<IsFloatingPoint T>
	NODISC CONSTEXPR T ToRadians(T aDegrees)
	{
		return aDegrees * (PI<T> / T(180.0));
	}

	template<IsFloatingPoint T>
	NODISC CONSTEXPR T ToDegrees(T aRadians)
	{
		return aRadians * (T(180.0) / PI<T>);
	}

	template<IsArithmetic T>
	NODISC CONSTEXPR T Pow(T aBase, std::int32_t aExponent)
	{
		if (aExponent < 0)
			return Pow(1 / aBase, -aExponent);

		if (aExponent == 0)
			return 1;

		if (aExponent % 2 == 0)
			return Pow(aBase, aExponent / 2) * Pow(aBase, aExponent / 2);

		return aBase * Pow(aBase, (aExponent - 1) / 2) * Pow(aBase, (aExponent - 1) / 2);
	}

	template<IsFloatingPoint T>
	NODISC CONSTEXPR auto Equal(T aFirst, T aSecond, T aEpsilon = static_cast<T>(DBL_EPSILON))
	{
		return std::abs(aFirst - aSecond) <= aEpsilon;
	}

	template<IsArithmetic T>
	NODISC CONSTEXPR int Sign(T aValue)
	{
		return (aValue < T{}) ? -1 : 1;
	}

	template<IsArithmetic T>
	NODISC CONSTEXPR T MapToRange(T aValue, T aMinIn, T aMaxIn, T aMinOut, T aMaxOut)
	{
		float x = (aValue - aMinIn) / static_cast<float>(aMaxIn - aMinIn);
		return static_cast<T>(aMinOut + static_cast<float>(aMaxOut - aMinOut) * x);
	}

	template<IsArithmetic T>
	NODISC CONSTEXPR auto SetPrecision(T aValue, int aPlaces)
	{
		double n = Pow(10.0, aPlaces);
		return std::round(aValue * n) / n;
	}

	inline constexpr float PI_F = PI<float>;
	inline constexpr float PI_2_F = PI_2<float>;
	inline constexpr float PI_4_F = PI_4<float>;

	inline constexpr double PI_D = PI<double>;
	inline constexpr double PI_2_D = PI_2<double>;
	inline constexpr double PI_4_D = PI_4<double>;
}
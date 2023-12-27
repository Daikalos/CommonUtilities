#pragma once

#include <limits>
#include <numbers>

#include <CommonUtilities/Utility/Concepts.hpp>
#include <CommonUtilities/Config.h>

#include <math.h>

namespace CommonUtilities
{
	template<IsFloatingPoint T = float>
	CONSTEXPR T PI_V = std::numbers::pi_v<T>;

	template<IsFloatingPoint T = float>
	CONSTEXPR T PI_2_V = PI_V<T> / T{2};

	template<IsFloatingPoint T = float>
	CONSTEXPR T PI_4_V = PI_2_V<T> / T{2};

	template<IsFloatingPoint T = float> 
	CONSTEXPR T DEG2RAD = PI_V<T> / T{180.0};

	template<IsFloatingPoint T = float>
	CONSTEXPR T RAD2DEG = T{180.0} / PI_V<T>;

	template<IsFloatingPoint T>
	NODISC CONSTEXPR T ToRadians(T aDegrees)
	{
		return aDegrees * DEG2RAD;
	}

	template<IsFloatingPoint T>
	NODISC CONSTEXPR T ToDegrees(T aRadians)
	{
		return aRadians * RAD2DEG;
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
	NODISC CONSTEXPR auto Equal(T aFirst, T aSecond, T aEpsilon = std::numeric_limits<T>::epsilon())
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

	inline constexpr float PI				= PI_V<float>;
	inline constexpr float PI_2				= PI_2_V<float>;
	inline constexpr float PI_4				= PI_4_V<float>;

	inline constexpr double PI_D			= PI_V<double>;
	inline constexpr double PI_2_D			= PI_2_V<double>;
	inline constexpr double PI_4_D			= PI_4_V<double>;

	inline constexpr long double PI_LD		= PI_V<long double>;
	inline constexpr long double PI_2_LD	= PI_2_V<long double>;
	inline constexpr long double PI_4_LD	= PI_4_V<long double>;
}
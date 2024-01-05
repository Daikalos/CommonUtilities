#pragma once

#include <limits>
#include <numbers>
#include <bit>

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

	inline constexpr float PI				= PI_V<float>;
	inline constexpr float PI_2				= PI_2_V<float>;
	inline constexpr float PI_4				= PI_4_V<float>;

	inline constexpr double PI_D			= PI_V<double>;
	inline constexpr double PI_2_D			= PI_2_V<double>;
	inline constexpr double PI_4_D			= PI_4_V<double>;

	inline constexpr long double PI_LD		= PI_V<long double>;
	inline constexpr long double PI_2_LD	= PI_2_V<long double>;
	inline constexpr long double PI_4_LD	= PI_4_V<long double>;

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

	NODISC CONSTEXPR float FastInverseSquareRoot(float aNumber)
	{
		const float v = std::bit_cast<float>(0x5f3759df - (std::bit_cast<std::uint32_t>(aNumber) >> 1));
		return v * (1.5f - (aNumber * 0.5f * v * v));
	}

	NODISC __forceinline float AtanApproximation(float aX)
	{
		//static constexpr float a1  =  0.99997726f;
		//static constexpr float a3  = -0.33262347f;
		//static constexpr float a5  =  0.19354346f;
		//static constexpr float a7  = -0.11643287f;
		//static constexpr float a9  =  0.05265332f;
		//static constexpr float a11 = -0.01172120f;

		//const float xSq = x * x;

		//return x * fmaf(xSq, fmaf(xSq, fmaf(xSq, fmaf(xSq, fmaf(xSq, a11, a9), a7), a5), a3), a1);

		static constexpr float a1 =  0.97239411f;
		static constexpr float a3 = -0.19194795f;

		return aX * std::fmaf(aX * aX, a3, a1);
	}

	/// Approximation of atan2 to favor performance while sacrificing accuracy.
	/// 
	NODISC __forceinline float Atan2Fast(float aY, float aX)
	{
		const float ay = std::abs(aY);
		const float ax = std::abs(aX);

		const bool swap = ax < ay;
		const float atanInput = (swap ? ax / ay : ay / ax);

		float res = AtanApproximation(atanInput);

		if (swap)	res = PI_2 - res;
		if (aX < 0)	res = PI - res;

		return std::copysignf(res, aY);
	}
}
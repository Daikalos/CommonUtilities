#pragma once

#include <math.h>
#include <limits>
#include <numbers>
#include <bit>

#include <CommonUtilities/Utility/Concepts.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T = float>
	constexpr T PI_V = std::numbers::pi_v<T>;

	template<typename T = float>
	constexpr T PI_2_V = PI_V<T> / T{2};

	template<typename T = float>
	constexpr T PI_4_V = PI_2_V<T> / T{2};

	template<typename T = float>
	constexpr T DEG2RAD_V = PI_V<T> / T{180.0};

	template<typename T = float>
	constexpr T RAD2DEG_V = T{180.0} / PI_V<T>;

	template<typename T = float>
	constexpr T EPSILON_V = std::numeric_limits<T>::epsilon();

	template<IsArithmeticType T>
	constexpr T MIN_V = (std::numeric_limits<T>::min)();

	template<IsArithmeticType T>
	constexpr T MAX_V = (std::numeric_limits<T>::max)();

	inline constexpr float			PI			= PI_V<float>;
	inline constexpr float			PI_2		= PI_2_V<float>;
	inline constexpr float			PI_4		= PI_4_V<float>;

	inline constexpr double			PI_D		= PI_V<double>;
	inline constexpr double			PI_2_D		= PI_2_V<double>;
	inline constexpr double			PI_4_D		= PI_4_V<double>;

	inline constexpr long double	PI_LD		= PI_V<long double>;
	inline constexpr long double	PI_2_LD		= PI_2_V<long double>;
	inline constexpr long double	PI_4_LD		= PI_4_V<long double>;

	inline constexpr float			DEG2RAD		= DEG2RAD_V<float>;
	inline constexpr double			DEG2RAD_D	= DEG2RAD_V<double>;
	inline constexpr long double	DEG2RAD_LD	= DEG2RAD_V<long double>;

	inline constexpr float			RAD2DEG		= RAD2DEG_V<float>;
	inline constexpr double			RAD2DEG_D	= RAD2DEG_V<double>;
	inline constexpr long double	RAD2DEG_LD	= RAD2DEG_V<long double>;

	inline constexpr float			EPSILON		= EPSILON_V<float>;
	inline constexpr double			EPSILON_D	= EPSILON_V<double>;
	inline constexpr long double	EPSILON_LD	= EPSILON_V<long double>;

	template<typename T>
	NODISC constexpr T ToRadians(T aDegrees)
	{
		return aDegrees * DEG2RAD_V<T>;
	}

	template<typename T>
	NODISC constexpr T ToDegrees(T aRadians)
	{
		return aRadians * RAD2DEG_V<T>;
	}

	template<IsArithmeticType T>
	NODISC constexpr T Pow(T aBase, std::int32_t aExponent)
	{
		if (aExponent < 0)
			return Pow(1 / aBase, -aExponent);

		if (aExponent == 0)
			return 1;

		if (aExponent % 2 == 0)
			return Pow(aBase, aExponent / 2) * Pow(aBase, aExponent / 2);

		return aBase * Pow(aBase, (aExponent - 1) / 2) * Pow(aBase, (aExponent - 1) / 2);
	}

	template<IsIntegralType T>
	NODISC constexpr auto Wrap(T val, T max, T min = 0)
	{
		const auto range = max - min;

		if (val < min)
			return max - ((min - val) % range);

		if (val >= max)
			return min + ((val - min) % range);

		return val;
	}

	template<IsFloatingPointType T>
	NODISC constexpr auto Wrap(T val, T max, T min = 0)
	{
		const auto range = max - min;

		if (val < min)
			return max - std::fmod(min - val, range);

		if (val >= max)
			return min + std::fmod(val - min, range);

		return val;
	}

	template<IsIntegralType T>
	NODISC constexpr auto WrapLower(T val, T max, T min = 0)
	{
		if (val < min)
			return max - ((min - val) % (max - min));

		return val;
	}

	template<IsIntegralType T>
	NODISC constexpr auto WrapUpper(T val, T max, T min = 0)
	{
		if (val >= max)
			return min + ((val - min) % (max - min));

		return val;
	}

	template<IsFloatingPointType T>
	NODISC constexpr auto WrapLower(T val, T max, T min = 0)
	{
		if (val < min)
			return max - std::fmod(min - val, max - min);

		return val;
	}

	template<IsFloatingPointType T>
	NODISC constexpr auto WrapUpper(T val, T max, T min = 0)
	{
		if (val > max)
			return min + std::fmod(val - min, max - min);

		return val;
	}

	template<IsFloatingPointType T>
	NODISC constexpr auto Equal(T aFirst, T aSecond, T aTolerance = EPSILON_V<T>)
	{
		return std::abs(aFirst - aSecond) <= aTolerance;
	}

	template<IsArithmeticType T>
	NODISC constexpr T Sign(T aValue)
	{
		return static_cast<T>((aValue < T{}) ? -1 : 1);
	}

	template<IsArithmeticType T>
	NODISC constexpr T MapToRange(T aValue, T aMinIn, T aMaxIn, T aMinOut, T aMaxOut)
	{
		float x = (aValue - aMinIn) / static_cast<float>(aMaxIn - aMinIn);
		return static_cast<T>(aMinOut + static_cast<float>(aMaxOut - aMinOut) * x);
	}

	template<IsArithmeticType T>
	NODISC constexpr auto SetPrecision(T aValue, int aPlaces)
	{
		double n = Pow(10.0, aPlaces);
		return std::round(aValue * n) / n;
	}

	template<IsFloatingPointType T>
	NODISC constexpr T ShortestAngleRadians(T aFirstRadians, T aSecondRadians)
	{
		return PI_V<T> - std::abs(std::fmodf(std::abs(aSecondRadians - aFirstRadians), PI_V<T> * 2) - PI_V<T>);
	}
	template<IsFloatingPointType T>
	NODISC constexpr T ShortestAngleDegrees(T aFirstDegrees, T aSecondDegrees)
	{
		return T(180) - std::abs(std::fmodf(std::abs(aSecondDegrees - aFirstDegrees), T(360)) - T(180));
	}

	template<typename T>
	NODISC constexpr const T& Max(const T& aFirst, const T& aSecond)
	{
		return (aFirst < aSecond) ? aSecond : aFirst;
	}

	template<typename T>
	NODISC constexpr const T& Min(const T& aFirst, const T& aSecond)
	{
		return (aSecond < aFirst) ? aSecond : aFirst;
	}

	template<typename T, typename... Args>
	NODISC constexpr const T& Max(const T& aFirst, const T& aSecond, const Args&... someArgs)
	{
		return Max(Max(aFirst, aSecond), someArgs...);
	}

	template<typename T, typename... Args>
	NODISC constexpr const T& Min(const T& aFirst, const T& aSecond, const Args&... someArgs)
	{
		return Min(Min(aFirst, aSecond), someArgs...);
	}

	template<typename T>
	NODISC constexpr T Abs(const T& aValue)
	{
		// since the minus operator will possibly create a new object, we cannot return a reference
		return (aValue >= T{}) ? aValue : -aValue;
	}

	template<typename T>
	NODISC constexpr const T& Clamp(const T& aValue, const T& aMin, const T& aMax)
	{
		assert(!(aMin > aMax) && "Min must be smaller than max!");
		if (aValue < aMin)
		{
			return aMin;
		}
		if (aValue > aMax)
		{
			return aMax;
		}
		return aValue;
	}

	template<typename T>
	NODISC constexpr T Lerp(const T& aStart, const T& aEnd, float aPercentage)
	{
		return static_cast<T>(aStart + aPercentage * (aEnd - aStart));
	}

	template<typename T>
	NODISC constexpr void Swap(T& aFirst, T& aSecond)
	{
		T temp	= std::move(aFirst);
		aFirst	= std::move(aSecond);
		aSecond = std::move(temp);
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
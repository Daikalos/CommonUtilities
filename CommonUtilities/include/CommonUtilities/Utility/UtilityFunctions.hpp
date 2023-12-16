#pragma once

#include <utility>
#include <cassert>

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T>
	NODISC CONSTEXPR const T& Max(const T& aFirst, const T& aSecond)
	{
		return (aFirst < aSecond) ? aSecond : aFirst;
	}

	template<typename T>
	NODISC CONSTEXPR const T& Min(const T& aFirst, const T& aSecond)
	{
		return (aSecond < aFirst) ? aSecond : aFirst;
	}

	template<typename T, typename... Args>
	NODISC CONSTEXPR const T& Max(const T& aFirst, const T& aSecond, const Args&... someArgs)
	{
		return Max(Max(aFirst, aSecond), someArgs...);
	}

	template<typename T, typename... Args>
	NODISC CONSTEXPR const T& Min(const T& aFirst, const T& aSecond, const Args&... someArgs)
	{
		return Min(Min(aFirst, aSecond), someArgs...);
	}

	template<typename T>
	NODISC CONSTEXPR T Abs(const T& aValue)
	{
		// since the minus operator will possibly create a new object, we cannot return a reference
		return (aValue >= T{}) ? aValue : -aValue;
	}

	template<typename T>
	NODISC CONSTEXPR const T& Clamp(const T& aValue, const T& aMin, const T& aMax)
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
	NODISC CONSTEXPR T Lerp(const T& aStart, const T& aEnd, float aPercentage)
	{
		return static_cast<T>(aStart + aPercentage * (aEnd - aStart));
	}

	template<typename T>
	NODISC CONSTEXPR void Swap(T& aFirst, T& aSecond)
	{
		T temp	= std::move(aFirst);
		aFirst	= std::move(aSecond);
		aSecond = std::move(temp);
	}
}
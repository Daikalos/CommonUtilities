#pragma once

#include "ArithmeticUtils.hpp"

namespace CommonUtilities
{
	// Check out https://easings.net/ for visualization of the easings

	template<IsFloatingPointType T>
	NODISC constexpr T EaseInSine(const T& aValue)
	{
		return T(1) - std::cos((aValue * PI_V<T>) / T(2));
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseOutSine(const T& aValue)
	{
		return std::sin((aValue * PI_V<T>) / T(2));
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseInOutSine(const T& aValue)
	{
		return -(std::cos(aValue * PI_V<T>) - T(1)) / T(2);
	}

	template<IsFloatingPointType T>
	NODISC constexpr T EaseInQuad(const T& aValue)
	{
		return aValue * aValue;
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseOutQuad(const T& aValue)
	{
		return T(1) - (T(1) - aValue) * (T(1) - aValue);
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseInOutQuad(const T& aValue)
	{
		return (aValue < T(0.5)) ? (T(2) * aValue * aValue) : (T(1) - std::pow(T(-2) * aValue + T(2), T(2)) / T(2));
	}

	template<IsFloatingPointType T>
	NODISC constexpr T EaseInCubic(const T& aValue)
	{
		return aValue * aValue * aValue;
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseOutCubic(const T& aValue)
	{
		return T(1) - std::pow(T(1) - aValue, T(3));
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseInOutCubic(const T& aValue)
	{
		return (aValue < T(0.5)) ? (T(4) * aValue * aValue * aValue) : (T(1) - std::pow(T(-2) * aValue + T(2), T(3)) / T(2));
	}

	template<IsFloatingPointType T>
	NODISC constexpr T EaseInQuart(const T& aValue)
	{
		return aValue * aValue * aValue * aValue;
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseOutQuart(const T& aValue)
	{
		return T(1) - std::pow(T(1) - aValue, T(4));
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseInOutQuart(const T& aValue)
	{
		return (aValue < T(0.5)) ? (T(8) * aValue * aValue * aValue * aValue) : (T(1) - std::pow(T(-2) * aValue + T(2), T(4)) / T(2));
	}

	template<IsFloatingPointType T>
	NODISC constexpr T EaseInQuint(const T& aValue)
	{
		return aValue * aValue * aValue * aValue * aValue;
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseOutQuint(const T& aValue)
	{
		return T(1) - std::pow(T(1) - aValue, T(5));
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseInOutQuint(const T& aValue)
	{
		return (aValue < T(0.5)) ? (T(16) * aValue * aValue * aValue * aValue * aValue) : (T(1) - std::pow(T(-2) * aValue + T(2), T(5)) / T(2));
	}

	template<IsFloatingPointType T>
	NODISC constexpr T EaseInExpo(const T& aValue)
	{
		return (aValue == T(0)) ? T(0) : std::pow(T(2), T(10) * aValue - T(10));
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseOutExpo(const T& aValue)
	{
		return (aValue == T(1)) ? T(1) : T(1) - std::pow(T(2), T(-10) * aValue);
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseInOutExpo(const T& aValue)
	{
		return (aValue == T(0))
			? T(0) : (aValue == T(1)) 
			? T(1) : (aValue < T(0.5)) 
			? std::pow(T(2), T(20) * aValue - T(10)) / T(2) : (T(2) - std::pow(T(2), T(-20) * aValue + T(10))) / T(2);
	}

	template<IsFloatingPointType T>
	NODISC constexpr T EaseInCirc(const T& aValue)
	{
		return T(1) - std::sqrt(T(1) - std::pow(aValue, T(2)));
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseOutCirc(const T& aValue)
	{
		return std::sqrt(T(1) - std::pow(aValue - T(1), T(2)));
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseInOutCirc(const T& aValue)
	{
		return (aValue < T(0.5))
			? (T(1) - std::sqrt(T(1) - std::pow(T(2) * aValue, T(2)))) / T(2)
			: (std::sqrt(T(1) - std::pow(T(-2) * aValue + T(2), T(2))) + T(1)) / T(2);
	}

	template<IsFloatingPointType T>
	NODISC constexpr T EaseInBack(const T& aValue)
	{
		constexpr T c1 = T(1.70158);
		constexpr T c3 = c1 + T(1);

		return c3 * aValue * aValue * aValue - c1 * aValue * aValue;
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseOutBack(const T& aValue)
	{
		constexpr T c1 = T(1.70158);
		constexpr T c3 = c1 + T(1);

		return T(1) + c3 * std::pow(aValue - T(1), T(3)) + c1 * std::pow(aValue - T(1), T(2));
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseInOutBack(const T& aValue)
	{
		constexpr T c1 = T(1.70158);
		constexpr T c2 = c1 * T(1.525);

		return (aValue < T(0.5))
			? (std::pow(T(2) * aValue, T(2)) * ((c2 + T(1)) * T(2) * aValue - c2)) / T(2)
			: (std::pow(T(2) * aValue - T(2), T(2)) * ((c2 + T(1)) * (aValue * T(2) - T(2)) + c2) + T(2)) / T(2);
	}
}
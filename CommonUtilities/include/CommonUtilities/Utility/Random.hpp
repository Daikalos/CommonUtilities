#pragma once

#include <random>
#include <type_traits>
#include <numeric>

#include <CommonUtilities/Utility/ArithmeticUtils.hpp>
#include <CommonUtilities/Math/Vector2.hpp>
#include <CommonUtilities/Math/Vector3.hpp>

#include <CommonUtilities/Utility/Concepts.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities::rn
{
	inline thread_local std::mt19937_64 dre(std::random_device{}());

	NODISC inline float Random()
	{
		std::uniform_real_distribution<float> uid(0.0f, 1.0f);
		return uid(dre);
	}

	template<IsFloatingPoint T>
	NODISC inline T Random(T aMin, T aMax)
	{
		std::uniform_real_distribution<T> uid(aMin, aMax);
		return uid(dre);
	}
	template<IsIntegral T>
	NODISC inline T Random(T aMin, T aMax)
	{
		std::uniform_int_distribution<T> uid(aMin, aMax);
		return uid(dre);
	}

	template<typename T>
	NODISC inline T RandomDev(T aMiddle, T aDeviation)
	{
		return Random<T>(aMiddle - aDeviation, aMiddle + aDeviation);
	}

	template<typename T, typename... Args>
	NODISC inline T RandomArg(T&& aFirstArg, Args&&... someArgs)
	{
		constexpr std::size_t size = sizeof...(someArgs) + 1;

		const T arr[size] = { std::forward<T>(aFirstArg), std::forward<Args>(someArgs)... };
		return arr[Random<std::size_t>(0LLU, size - 1)];
	}

	///	Creates a numerically increasing list of values up to size that are then shuffled and returned.
	/// 
	template<typename T>
	NODISC inline auto RandomVector(T size)
	{
		std::vector<T> result;
		result.resize(size);

		std::iota(result.begin(), result.end(), 0);
		std::shuffle(result.begin(), result.end(), dre);

		return result;
	}

	///	Creates a numerically increasing list of values up to size that are then shuffled and returned.
	/// 
	template<typename T, std::size_t N>
	NODISC inline auto RandomArray()
	{
		std::array<T, N> result;

		std::iota(result.begin(), result.end(), 0);
		std::shuffle(result.begin(), result.end(), dre);

		return result;
	}

	template<typename T>
	NODISC inline cu::Vector2<T> RandomPointInRect(const cu::Vector2<T>& aCenter, const cu::Vector2<T>& aHalfSize)
	{
		return cu::Vector2<T>(
			RandomDev<T>(aCenter.x, aHalfSize.x), 
			RandomDev<T>(aCenter.y, aHalfSize.y));
	}

	template<typename T>
	NODISC inline cu::Vector2<T> RandomPointInCircle(const cu::Vector2<T>& aCenter, T aRadius)
	{
		float r = aRadius * std::sqrt(Random());
		float theta = Random() * 2.0f * au::PI;

		return cu::Vector2<T>(
			(T)(aCenter.x + r * std::cos(theta)),
			(T)(aCenter.y + r * std::sin(theta)));
	}

	template<typename T>
	NODISC inline cu::Vector2<T> RandomDeflection(const cu::Vector2<T>& aDirection, T aMaxRotation)
	{
		T angle = RandomDev<T>(0.0f, aMaxRotation);

		T cos = std::cos(angle);
		T sin = std::sin(angle);

		return cu::Vector2<T>(
			(T)(cos * aDirection.x - sin * aDirection.y),
			(T)(sin * aDirection.x + cos * aDirection.y));
	}

	template<typename T>
	NODISC inline cu::Vector3<T> RandomPointInRect(const cu::Vector3<T>& aCenter, const cu::Vector3<T>& aHalfSize)
	{
		return cu::Vector2<T>(
			RandomDev<T>(aCenter.x, aHalfSize.x),
			RandomDev<T>(aCenter.y, aHalfSize.y),
			RandomDev<T>(aCenter.z, aHalfSize.z));
	}

	template<typename T>
	NODISC inline cu::Vector3<T> RandomPointInSphere(const cu::Vector3<T>& aCenter, T aRadius)
	{
		float r		= aRadius * std::sqrt(Random());
		float phi	= Random() * 2.0f * au::PI;
		float theta = std::acos(Random(-1.0f, 1.0f));

		const float s = std::sin(theta);
		const float c = std::cos(theta);

		return cu::Vector2<T>(
			(T)(aCenter.x + r * c * std::cos(phi)),
			(T)(aCenter.y + r * s * std::sin(phi)),
			(T)(aCenter.z + r * c);
	}

	COMMON_UTILITIES_API void Seed(std::uint64_t seed = std::mt19937_64::default_seed);
}
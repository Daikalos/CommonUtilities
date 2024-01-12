#pragma once

#include <random>
#include <type_traits>
#include <numeric>

#include <CommonUtilities/Utility/Concepts.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
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

	template<typename T, typename... Args>
	NODISC inline T RandomArg(T&& aFirstArg, Args&&... someArgs)
	{
		constexpr std::size_t size = sizeof...(someArgs) + 1;

		const T arr[size] = { std::forward<T>(aFirstArg), std::forward<Args>(someArgs)... };
		return arr[Random<std::size_t>(0LLU, size - 1)];
	}

	///	Creates a numerically increasing list of values up to size that are then shuffled and returned
	/// 
	template<typename T>
	NODISC inline auto Vector(T size)
	{
		std::vector<T> result;
		result.resize(size);

		std::iota(result.begin(), result.end(), 0);
		std::shuffle(result.begin(), result.end(), dre);

		return result;
	}

	///	Creates a numerically increasing list of values up to size that are then shuffled and returned
	/// 
	template<typename T, std::size_t N>
	NODISC inline auto Array()
	{
		std::array<T, N> result;

		std::iota(result.begin(), result.end(), 0);
		std::shuffle(result.begin(), result.end(), dre);

		return result;
	}

	inline void Seed(std::uint64_t seed = std::mt19937_64::default_seed)
	{
		dre.seed(seed);
	}
}
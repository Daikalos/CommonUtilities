#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace CommonUtilities
{
	/// Packs the values from left most bit to right most bit with the given sizes
	/// 
	template<std::uint64_t... BitSizes> requires ((BitSizes + ...) == sizeof(std::uint64_t) * CHAR_BIT)
	constexpr std::uint64_t PackValues64(const std::array<std::uint64_t, sizeof...(BitSizes)>& aValuesToPack)
	{
		constexpr std::array<std::uint64_t, sizeof...(BitSizes)> BitArr{ BitSizes... };

		std::uint64_t result{};

		std::uint64_t bitDisplacement{};
		for (std::uint64_t i = 0; i < sizeof...(BitSizes); ++i)
		{
			result |= aValuesToPack[i] << uint64_t(sizeof(std::uint64_t) * CHAR_BIT - (BitArr[i] + bitDisplacement));
			bitDisplacement += BitArr[i];
		}

		return result;
	}

	template<typename T>
	inline std::string ToBinary(const T& aVariable)
	{
		const std::byte* bytes = reinterpret_cast<const std::byte*>(&aVariable);

		std::string result;
		result.reserve(sizeof(T) * CHAR_BIT);

		for (int i = sizeof(T) - 1; i >= 0; --i)
		{
			for (int j = CHAR_BIT - 1; j >= 0; --j)
				result += std::to_integer<int>(bytes[i] & std::byte(1 << j)) ? '1' : '0';
		}

		return result;
	}
}
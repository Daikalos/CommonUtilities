#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <array>

namespace CommonUtilities
{
	/// Packs the values from left most bit to right most bit with the given sizes
	/// 
	template<std::uint64_t... BitSizes> requires ((BitSizes + ...) == sizeof(std::uint64_t) * CHAR_BIT)
	constexpr std::uint64_t PackValues64(const std::array<std::uint64_t, sizeof...(BitSizes)>& aValuesToPack)
	{
		constexpr auto ComputeDisplacements = []<std::uint64_t... BitSizes>() constexpr
		{
			constexpr std::array<std::uint64_t, sizeof...(BitSizes)> bitArr{ BitSizes... };
			std::array<std::uint64_t, sizeof...(BitSizes)> bitDisplacements{};

			for (std::uint64_t i = 1; i < sizeof...(BitSizes); ++i)
				bitDisplacements[i] += bitDisplacements[i - 1] + bitArr[i - 1];

			return bitDisplacements;
		};

		constexpr std::array<std::uint64_t, sizeof...(BitSizes)> bitArr { BitSizes... };
		constexpr std::array<std::uint64_t, sizeof...(BitSizes)> bitDisplacementArr = ComputeDisplacements.template operator()<BitSizes...>();

		std::uint64_t result{};

		for (std::uint64_t i = 0; i < sizeof...(BitSizes); ++i)
		{
			result |= aValuesToPack[i] << uint64_t(sizeof(std::uint64_t) * CHAR_BIT - (bitArr[i] + bitDisplacementArr[i]));
		}

		return result;
	}

	/// Extracts N bits from a number with the offset starting from left most bit
	/// 
	template<std::size_t BitSize, std::size_t BitOffset>
	constexpr std::uint64_t ExtractValue64(std::uint64_t aPackedValues)
	{
		constexpr std::uint64_t bitPos = sizeof(std::uint64_t) * CHAR_BIT - (BitOffset + BitSize);
		return (aPackedValues >> bitPos) & ((1ULL << BitSize) - 1);
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
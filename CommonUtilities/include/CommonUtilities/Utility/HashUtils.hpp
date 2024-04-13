#pragma once

#include <span>
#include <string>

#include <CommonUtilities\Config.h>

namespace CommonUtilities::hs
{
	namespace details
	{
		constexpr std::uint32_t JenkinsHashImpl(const std::byte* aKey, std::size_t aLength)
		{
			std::size_t i = 0;
			std::uint32_t hash = 0;

			while (i != aLength)
			{
				hash += static_cast<std::uint32_t>(aKey[i++]);
				hash += hash << 10;
				hash ^= hash >> 6;
			}

			hash += hash << 3;
			hash ^= hash >> 11;
			hash += hash << 15;

			return hash;
		}
	}

	/// FNV-1a implementation
	/// 
	constexpr std::size_t FNV1a(const std::string_view aSignature)
	{
		std::size_t result = 0xcbf29ce484222325; // FNV offset basis

		for (const char c : aSignature)
		{
			result ^= static_cast<std::size_t>(c);
			result *= 0x100000001b3; // FNV prime
		}

		return result;
	}

	template<typename T>
	constexpr std::uint32_t JenkinsHash(const T& aKey)
	{
		return details::JenkinsHashImpl(reinterpret_cast<const std::byte*>(&aKey), sizeof(T));
	}

	template<>
	constexpr std::uint32_t JenkinsHash<std::string>(const std::string& aKey)
	{
		return  details::JenkinsHashImpl(reinterpret_cast<const std::byte*>(aKey.c_str()), aKey.size());
	}

	template<typename T>
	constexpr void HashCombine(const T& aValue, std::size_t& aSeed)
	{
		aSeed ^= static_cast<std::size_t>(aSeed) + 0x9e3779b9 + (aSeed << 6) + (aSeed >> 2);
	}

	template<typename T>
	struct ContainerHash
	{
		NODISC constexpr std::size_t operator()(std::span<const T> someItems) const
		{
			std::size_t seed = someItems.size();

			for (auto x : someItems)
			{
				x = ((x >> 16) ^ x) * 0x45d9f3b;
				x = ((x >> 16) ^ x) * 0x45d9f3b;
				x = (x >> 16) ^ x;

				HashCombine(x, seed);
			}

			return seed;
		}
	};
}
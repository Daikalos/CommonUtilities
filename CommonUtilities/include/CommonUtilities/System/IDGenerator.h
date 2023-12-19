#pragma once

#include <typeinfo>
#include <string_view>

#include <CommonUtilities/Config.h>

namespace CommonUtilities::id
{
	/// FNV-1a implementation
	/// 
	inline consteval std::size_t HashFunction(const std::string_view aSignature)
	{
		std::size_t result = 0xcbf29ce484222325; // FNV offset basis

		for (const char c : aSignature)
		{
			result ^= static_cast<std::size_t>(c);
			result *= 0x100000001b3; // FNV prime
		}

		return result;
	}

	/// Typical implementation of generating unique ids, does not work however across boundaries 
	/// (counter will reset to zero in different boundaries, and can therefore collide)
	/// 
	struct Generator
	{
		static std::size_t Next()
		{
			static std::size_t value{};
			return value++;
		}
	};

	template<typename T>
	struct Type
	{
#ifdef COMMON_UTILITIES_PRETTY_FUNCTION
		static consteval std::size_t ID()
		{
			return HashFunction(COMMON_UTILITIES_PRETTY_FUNCTION);
		}
#else
		static std::size_t ID()
		{
			static const std::size_t value = Generator::Next();
			return value;
		}
#endif
	};
}
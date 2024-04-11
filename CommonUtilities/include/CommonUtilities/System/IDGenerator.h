#pragma once

#include <typeinfo>
#include <string_view>

#include <CommonUtilities/Utility/HashUtils.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities::id
{
	/// Typical implementation of generating unique ids, does not work however across boundaries 
	/// (counter will reset to zero in different boundaries, and can therefore collide), read more
	/// about it here: https://skypjack.github.io/2020-03-14-ecs-baf-part-8/
	/// 
	template<typename T = void>
	struct Generator
	{
		static inline std::size_t Next()
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
			return hs::FNV1a(COMMON_UTILITIES_PRETTY_FUNCTION);
		}
#else
		static constexpr std::size_t ID()
		{
			static const std::size_t value = Generator<>::Next();
			return value;
		}
#endif
	};
}
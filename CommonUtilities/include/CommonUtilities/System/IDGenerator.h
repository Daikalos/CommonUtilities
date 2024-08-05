#pragma once

#include <typeinfo>
#include <string_view>

#include <CommonUtilities/Utility/TypeUtils.hpp>
#include <CommonUtilities/Utility/HashUtils.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities::id
{
	/// Typical implementation of generating unique ids, does not work however across boundaries 
	/// (counter will reset to zero in different boundaries, and can therefore collide), read more
	/// about it here: https://skypjack.github.io/2020-03-14-ecs-baf-part-8/
	/// 
	template<typename T = void>
	class Generator
	{
	public:
		/// Retrieves the current ID and increments the value.
		/// 
		static inline std::size_t Next()
		{
			return ID++;
		}

		/// Resets the ID counter, safe to assume that all of the older IDs are considered invalid 
		/// after this call.
		/// 
		static inline void Reset(std::size_t aInitialID = 1)
		{
			ID = aInitialID;
		}

	private:
		Generator() = delete;
		~Generator() = delete;

		static std::size_t ID;
	};

	template<typename T>
	inline std::size_t Generator<T>::ID = 1; // 0 reserved for NULL

	/// Generates a unique ID for a given type (as an alternative to using RTTI)
	/// 
	template<typename T>
	struct Type
	{
#ifdef COMMON_UTILITIES_PRETTY_FUNCTION
		static consteval std::size_t ID()
		{
			return FNV1a(WrappedTypeName<T>());
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
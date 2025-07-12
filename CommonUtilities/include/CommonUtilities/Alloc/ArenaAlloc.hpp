#pragma once

#include <cstddef>
#include <shared_mutex>

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{ 
	namespace details::arena
	{
		static std::shared_mutex globalMutex;

		COMMON_UTILITIES_API NODISC std::byte* Allocate(std::size_t aNumBytes, std::size_t aAlignment, const std::byte* aHint);
		COMMON_UTILITIES_API void Deallocate(std::byte* aMemory, std::size_t aNumBytes);
	}

	template<typename T>
	class ArenaAlloc
	{
	public:
		using value_type								= T;
		using size_type									= std::size_t;
		using difference_type							= std::ptrdiff_t;
		using propagate_on_container_move_assignment	= std::true_type;
		using is_always_equal							= std::true_type;

		constexpr ArenaAlloc() noexcept {}
		constexpr ~ArenaAlloc() = default;

		constexpr ArenaAlloc(const ArenaAlloc&) noexcept = default;

		template <class U>
		constexpr ArenaAlloc(const ArenaAlloc<U>&) noexcept {}

		constexpr ArenaAlloc& operator=(const ArenaAlloc&) = default;

		NODISC constexpr __declspec(allocator) T* allocate(size_type aNumObjects, const T* aHint = nullptr)
		{
			std::scoped_lock lock(details::arena::globalMutex);
			return reinterpret_cast<T*>(details::arena::Allocate(aNumObjects * sizeof(T), alignof(T), reinterpret_cast<const std::byte*>(aHint)));
		}

		constexpr void deallocate(T* aObject, size_type aNumObjects)
		{
			std::scoped_lock lock(details::arena::globalMutex);
			details::arena::Deallocate(reinterpret_cast<std::byte*>(aObject), aNumObjects * sizeof(T));
		}
	};
}
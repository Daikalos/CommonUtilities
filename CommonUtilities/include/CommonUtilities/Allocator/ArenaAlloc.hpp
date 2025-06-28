#pragma once

#include <cstddef>
#include <vector>

namespace CommonUtilities
{ 
	template<typename T>
	class ArenaAlloc
	{
	public:
		using value_type								= T;
		using size_type									= std::size_t;
		using difference_type							= std::ptrdiff_t;
		using propagate_on_container_move_assignment	= std::true_type;

		constexpr ArenaAlloc() noexcept {}
		constexpr ~ArenaAlloc() = default;

		constexpr ArenaAlloc(const ArenaAlloc&) noexcept = default;

		template <class U>
		constexpr ArenaAlloc(const ArenaAlloc<U>&) noexcept {}

		constexpr ArenaAlloc& operator=(const ArenaAlloc&) = default;

		constexpr __declspec(allocator) T* allocate(size_type aCount)
		{
			return nullptr;
		}

		constexpr void deallocate(T* aObject, size_type aCount)
		{

		}
	};

	std::vector<int, ArenaAlloc<int>> a;
}
#pragma once

#include <vector>

#include <CommonUtilities/Structures/FreeVector.hpp>
#include <CommonUtilities/Config.h>

#include "Random.hpp"

namespace CommonUtilities
{
	template<typename T, typename Alloc = std::allocator<T>>
	class RandomBag
	{
	public:
		using value_type		= T;
		using reference			= T&;
		using const_reference	= const T&;
		using size_type			= std::size_t;
		using container_type	= std::vector<T>;

		using iterator					= typename container_type::iterator;
		using const_iterator			= typename container_type::const_iterator;
		using reverse_iterator			= typename container_type::reverse_iterator;
		using const_reverse_iterator	= typename container_type::const_reverse_iterator;

		constexpr RandomBag() = default;
		constexpr ~RandomBag() noexcept(std::is_nothrow_destructible_v<T>) = default;

		NODISC constexpr auto operator[](size_type aIndex) -> reference;
		NODISC constexpr auto operator[](size_type aIndex) const -> const_reference;

		NODISC constexpr auto at(size_type aIndex) -> reference;
		NODISC constexpr auto at(size_type aIndex) const -> const_reference;

		NODISC constexpr bool empty() const noexcept;
		NODISC constexpr auto size() const noexcept -> size_type;

		NODISC constexpr auto capacity() const noexcept -> size_type;

		NODISC constexpr auto max_size() const noexcept -> size_type;

		constexpr void push_back(int aWeight, const T& aItem);
		constexpr void push_back(int aWeight, T&& aItem);

		template<typename... Args> requires(std::constructible_from<T, Args...>)
		constexpr auto emplace_back(Args&&... someArgs) -> reference;

		constexpr void pop_back();

		constexpr auto erase(const_iterator aPosition) -> iterator;
		constexpr auto erase(const_iterator aFirst, const_iterator aLast) -> iterator;

		template<typename... Args> requires std::constructible_from<T, Args...>
		constexpr void emplace(const_iterator aPosition, int aWeight, Args&&... someArgs);

		constexpr auto insert(const_iterator aPosition, int aWeight, const_reference aElement) -> iterator;
		constexpr auto insert(const_iterator aPosition, int aWeight, T&& aElement) -> iterator;

		constexpr auto peek() const -> const_reference;
		constexpr auto pop() -> value_type;

		constexpr void resize(std::size_t aNewSize);

		constexpr void swap(RandomBag& aOther);

		constexpr void reserve(size_type aCapacity);

		constexpr void clear();

		constexpr void shrink_to_fit();

		NODISC constexpr auto begin() noexcept -> iterator;
		NODISC constexpr auto end() noexcept -> iterator;
		NODISC constexpr auto begin() const noexcept -> const_iterator;
		NODISC constexpr auto end() const noexcept -> const_iterator;
		NODISC constexpr auto cbegin() const noexcept -> const_iterator;
		NODISC constexpr auto cend() const noexcept -> const_iterator;

		NODISC constexpr auto rbegin() noexcept -> reverse_iterator;
		NODISC constexpr auto rend() noexcept -> reverse_iterator;
		NODISC constexpr auto rbegin() const noexcept -> const_reverse_iterator;
		NODISC constexpr auto rend() const noexcept -> const_reverse_iterator;
		NODISC constexpr auto crbegin() const noexcept -> const_reverse_iterator;
		NODISC constexpr auto crend() const noexcept -> const_reverse_iterator;

	private:
		
		int myTotalWeight = 0;
	};
}
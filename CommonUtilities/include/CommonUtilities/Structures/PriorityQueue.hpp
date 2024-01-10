#pragma once

#include <vector>
#include <ranges>

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	namespace pq
	{
		enum class Comparison : bool
		{
			Less = false,
			Greater = true
		};
	}

	/// Custom container_type to allow modifying the elements while still maintaining order due to separating the priority from the item itself.
	/// 
	template<typename T, pq::Comparison C = pq::Comparison::Less>
	class PriorityQueue
	{
	public:
		using value_type		= T;
		using reference			= T&;
		using const_reference	= const T&;
		using size_type			= std::size_t;

		struct Node
		{
			value_type	item;
			float		priority {0.0f};
		};

		using container_type = std::vector<Node>;

		using iterator					= typename container_type::iterator;
		using const_iterator			= typename container_type::const_iterator;
		using reverse_iterator			= typename container_type::reverse_iterator;
		using const_reverse_iterator	= typename container_type::const_reverse_iterator;

		PriorityQueue() = default;

		NODISC constexpr auto operator[](size_type aIndex) -> reference;
		NODISC constexpr auto operator[](size_type aIndex) const -> const_reference;

		NODISC constexpr auto at(size_type aIndex) -> reference;
		NODISC constexpr auto at(size_type aIndex) const -> const_reference;

		NODISC constexpr bool empty() const noexcept;
		NODISC constexpr auto size() const noexcept -> size_type;

		NODISC constexpr auto max_size() const noexcept -> size_type;

		NODISC constexpr auto top() noexcept -> reference;
		NODISC constexpr auto top() const noexcept -> const_reference;

		constexpr void push(const T& aItem, float aPriority);
		constexpr void push(T&& aItem, float aPriority);

		template<typename... Args> requires std::constructible_from<T, Args...>
		constexpr void emplace(float aPriority, Args&&... someArgs);

		constexpr void pop();

		constexpr void reserve(size_type aCapacity);

		constexpr void clear();

		constexpr void shrink_to_fit();

		NODISC constexpr auto begin() const noexcept -> const_iterator;
		NODISC constexpr auto end() const noexcept -> const_iterator;

		NODISC constexpr auto cbegin() const noexcept -> const_iterator;
		NODISC constexpr auto cend() const noexcept -> const_iterator;

		NODISC constexpr auto rbegin() const noexcept -> const_reverse_iterator;
		NODISC constexpr auto rend() const noexcept -> const_reverse_iterator;

		NODISC constexpr auto crbegin() const noexcept -> const_reverse_iterator;
		NODISC constexpr auto crend() const noexcept -> const_reverse_iterator;

	private:
		template<pq::Comparison>
		class Comp {};

		template<>
		struct Comp<pq::Comparison::Less>
		{
			NODISC constexpr bool operator()(const Node& lhs, const Node& rhs) const noexcept
			{
				return lhs.priority < rhs.priority;
			}
		};

		template<>
		struct Comp<pq::Comparison::Greater>
		{
			NODISC constexpr bool operator()(const Node& lhs, const Node& rhs) const noexcept
			{
				return lhs.priority > rhs.priority;
			}
		};

		container_type	myNodes;
		Comp<C>			myComp;
	};

	template<typename T, pq::Comparison C>
	constexpr auto PriorityQueue<T, C>::operator[](size_type aIndex) -> reference
	{
		return myNodes[aIndex].item;
	}
	template<typename T, pq::Comparison C>
	constexpr auto PriorityQueue<T, C>::operator[](size_type aIndex) const -> const_reference
	{
		return myNodes[aIndex].item;
	}

	template<typename T, pq::Comparison C>
	constexpr auto PriorityQueue<T, C>::at(size_type aIndex) -> reference
	{
		return myNodes.at(aIndex).item;
	}
	template<typename T, pq::Comparison C>
	constexpr auto PriorityQueue<T, C>::at(size_type aIndex) const -> const_reference
	{
		return myNodes.at(aIndex).item;
	}

	template<typename T, pq::Comparison C>
	constexpr bool PriorityQueue<T, C>::empty() const noexcept
	{
		return myNodes.empty();
	}
	template<typename T, pq::Comparison C>
	constexpr auto PriorityQueue<T, C>::size() const noexcept -> size_type
	{
		return myNodes.size();
	}

	template<typename T, pq::Comparison C>
	constexpr auto PriorityQueue<T, C>::max_size() const noexcept -> size_type
	{
		return myNodes.max_size();
	}

	template<typename T, pq::Comparison C>
	constexpr auto PriorityQueue<T, C>::top() noexcept -> reference
	{
		return myNodes.front().item;
	}
	template<typename T, pq::Comparison C>
	constexpr auto PriorityQueue<T, C>::top() const noexcept -> const_reference
	{
		return myNodes.front().item;
	}

	template<typename T, pq::Comparison C>
	constexpr void PriorityQueue<T, C>::push(const T& aItem, float aPriority)
	{
		emplace(aPriority, aItem);
	}
	template<typename T, pq::Comparison C>
	constexpr void PriorityQueue<T, C>::push(T&& aItem, float aPriority)
	{
		emplace(aPriority, std::move(aItem));
	}

	template<typename T, pq::Comparison C>
	template<typename ...Args> requires std::constructible_from<T, Args...>
	constexpr void PriorityQueue<T, C>::emplace(float aPriority, Args&&... someArgs)
	{
		myNodes.emplace_back(std::forward<Args>(someArgs)..., aPriority);
		std::ranges::push_heap(myNodes.begin(), myNodes.end(), myComp);
	}

	template<typename T, pq::Comparison C>
	constexpr void PriorityQueue<T, C>::pop()
	{
		std::ranges::pop_heap(myNodes.begin(), myNodes.end(), myComp);
		myNodes.pop_back();
	}

	template<typename T, pq::Comparison C>
	constexpr void PriorityQueue<T, C>::reserve(size_type aCapacity)
	{
		myNodes.reserve(aCapacity);
	}

	template<typename T, pq::Comparison C>
	constexpr void PriorityQueue<T, C>::clear()
	{
		myNodes.clear();
	}

	template<typename T, pq::Comparison C>
	constexpr void PriorityQueue<T, C>::shrink_to_fit()
	{
		myNodes.shrink_to_fit();
	}

	template<typename T, pq::Comparison C>
	constexpr auto PriorityQueue<T, C>::begin() const noexcept -> const_iterator
	{
		return myNodes.begin();
	}
	template<typename T, pq::Comparison C>
	constexpr auto PriorityQueue<T, C>::end() const noexcept -> const_iterator
	{
		return myNodes.begin();
	}

	template<typename T, pq::Comparison C>
	constexpr auto PriorityQueue<T, C>::cbegin() const noexcept -> const_iterator
	{
		return myNodes.cbegin();
	}
	template<typename T, pq::Comparison C>
	constexpr auto PriorityQueue<T, C>::cend() const noexcept -> const_iterator
	{
		return myNodes.cbegin();
	}

	template<typename T, pq::Comparison C>
	constexpr auto PriorityQueue<T, C>::rbegin() const noexcept -> const_reverse_iterator
	{
		return myNodes.rbegin();
	}
	template<typename T, pq::Comparison C>
	constexpr auto PriorityQueue<T, C>::rend() const noexcept -> const_reverse_iterator
	{
		return myNodes.rbegin();
	}

	template<typename T, pq::Comparison C>
	constexpr auto PriorityQueue<T, C>::crbegin() const noexcept -> const_reverse_iterator
	{
		return myNodes.crbegin();
	}
	template<typename T, pq::Comparison C>
	constexpr auto PriorityQueue<T, C>::crend() const noexcept -> const_reverse_iterator
	{
		return myNodes.crbegin();
	}
}
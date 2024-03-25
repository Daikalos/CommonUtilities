#pragma once

#include <vector>
#include <ranges>
#include <algorithm>

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	namespace pq
	{
		enum class HeapType : bool
		{
			Min = false,
			Max = true
		};
	}

	/// Custom priority queue to allow modifying the elements while still maintaining order due to separating the priority from 
	/// the item itself.
	/// 
	template<typename T, pq::HeapType C = pq::HeapType::Min>
	class PriorityQueue
	{
	public:
		using value_type		= T;
		using reference			= T&;
		using const_reference	= const T&;
		using size_type			= std::size_t;

		using Node				= value_type;

		using container_type = std::vector<Node>;

		using iterator					= typename container_type::iterator;
		using const_iterator			= typename container_type::const_iterator;
		using reverse_iterator			= typename container_type::reverse_iterator;
		using const_reverse_iterator	= typename container_type::const_reverse_iterator;

		PriorityQueue() = default;
		~PriorityQueue() noexcept(std::is_nothrow_destructible_v<T>) = default;

		template<typename Iter> requires(std::forward_iterator<Iter> && std::constructible_from<T, typename Iter::value_type>)
		constexpr PriorityQueue(Iter aFirst, Iter aLast);

		constexpr PriorityQueue(std::initializer_list<T> aInitList);

		NODISC constexpr auto operator[](size_type aIndex) -> reference;
		NODISC constexpr auto operator[](size_type aIndex) const -> const_reference;

		NODISC constexpr auto At(size_type aIndex) -> reference;
		NODISC constexpr auto At(size_type aIndex) const -> const_reference;

		NODISC constexpr bool IsEmpty() const noexcept;
		NODISC constexpr auto Size() const noexcept -> size_type;

		NODISC constexpr auto MaxSize() const noexcept -> size_type;

		NODISC constexpr auto Top() noexcept -> reference;
		NODISC constexpr auto Top() const noexcept -> const_reference;

		constexpr void Push(const T& aItem);
		constexpr void Push(T&& aItem);

		template<typename... Args> requires std::constructible_from<T, Args...>
		constexpr void Emplace(Args&&... someArgs);

		constexpr void Pop();

		constexpr void Reserve(size_type aCapacity);

		constexpr void Clear();

		constexpr void ShrinkToFit();

		NODISC constexpr auto begin() const noexcept -> const_iterator;
		NODISC constexpr auto end() const noexcept -> const_iterator;

		NODISC constexpr auto cbegin() const noexcept -> const_iterator;
		NODISC constexpr auto cend() const noexcept -> const_iterator;

		NODISC constexpr auto rbegin() const noexcept -> const_reverse_iterator;
		NODISC constexpr auto rend() const noexcept -> const_reverse_iterator;

		NODISC constexpr auto crbegin() const noexcept -> const_reverse_iterator;
		NODISC constexpr auto crend() const noexcept -> const_reverse_iterator;

	private:
		template<pq::HeapType>
		class Comp {};

		template<>
		struct Comp<pq::HeapType::Min>
		{
			NODISC bool operator()(const Node& lhs, const Node& rhs) const
			{
				return rhs < lhs;
			}
		};

		template<>
		struct Comp<pq::HeapType::Max>
		{
			NODISC bool operator()(const Node& lhs, const Node& rhs) const
			{
				return lhs < rhs;
			}
		};

		container_type		myNodes;
		NOADDRESS Comp<C>	myComp;
	};

	template<typename T, pq::HeapType C>
	template<typename Iter> requires(std::forward_iterator<Iter>&& std::constructible_from<T, typename Iter::value_type>)
	constexpr PriorityQueue<T, C>::PriorityQueue(Iter aFirst, Iter aLast)
		: myNodes(aFirst, aLast)
		, myComp()
	{
		std::ranges::make_heap(myNodes, myComp);
	}

	template<typename T, pq::HeapType C>
	constexpr PriorityQueue<T, C>::PriorityQueue(std::initializer_list<T> aInitList)
		: myNodes(aInitList)
		, myComp()
	{
		std::ranges::make_heap(myNodes, myComp);
	}

	template<typename T, pq::HeapType C>
	constexpr auto PriorityQueue<T, C>::operator[](size_type aIndex) -> reference
	{
		return myNodes[aIndex];
	}
	template<typename T, pq::HeapType C>
	constexpr auto PriorityQueue<T, C>::operator[](size_type aIndex) const -> const_reference
	{
		return myNodes[aIndex];
	}

	template<typename T, pq::HeapType C>
	constexpr auto PriorityQueue<T, C>::At(size_type aIndex) -> reference
	{
		return myNodes.At(aIndex);
	}
	template<typename T, pq::HeapType C>
	constexpr auto PriorityQueue<T, C>::At(size_type aIndex) const -> const_reference
	{
		return myNodes.At(aIndex);
	}

	template<typename T, pq::HeapType C>
	constexpr bool PriorityQueue<T, C>::IsEmpty() const noexcept
	{
		return myNodes.empty();
	}
	template<typename T, pq::HeapType C>
	constexpr auto PriorityQueue<T, C>::Size() const noexcept -> size_type
	{
		return myNodes.size();
	}

	template<typename T, pq::HeapType C>
	constexpr auto PriorityQueue<T, C>::MaxSize() const noexcept -> size_type
	{
		return myNodes.max_size();
	}

	template<typename T, pq::HeapType C>
	constexpr auto PriorityQueue<T, C>::Top() noexcept -> reference
	{
		return myNodes.front();
	}
	template<typename T, pq::HeapType C>
	constexpr auto PriorityQueue<T, C>::Top() const noexcept -> const_reference
	{
		return myNodes.front();
	}

	template<typename T, pq::HeapType C>
	constexpr void PriorityQueue<T, C>::Push(const T& aItem)
	{
		Emplace(aItem);
	}
	template<typename T, pq::HeapType C>
	constexpr void PriorityQueue<T, C>::Push(T&& aItem)
	{
		Emplace(std::move(aItem));
	}

	template<typename T, pq::HeapType C>
	template<typename ...Args> requires std::constructible_from<T, Args...>
	constexpr void PriorityQueue<T, C>::Emplace(Args&&... someArgs)
	{
		myNodes.emplace_back(std::forward<Args>(someArgs)...);
		std::ranges::push_heap(myNodes.begin(), myNodes.end(), myComp);
	}

	template<typename T, pq::HeapType C>
	constexpr void PriorityQueue<T, C>::Pop()
	{
		std::ranges::pop_heap(myNodes.begin(), myNodes.end(), myComp);
		myNodes.pop_back();
	}

	template<typename T, pq::HeapType C>
	constexpr void PriorityQueue<T, C>::Reserve(size_type aCapacity)
	{
		myNodes.reserve(aCapacity);
	}

	template<typename T, pq::HeapType C>
	constexpr void PriorityQueue<T, C>::Clear()
	{
		myNodes.clear();
	}

	template<typename T, pq::HeapType C>
	constexpr void PriorityQueue<T, C>::ShrinkToFit()
	{
		myNodes.shrink_to_fit();
	}

	template<typename T, pq::HeapType C>
	constexpr auto PriorityQueue<T, C>::begin() const noexcept -> const_iterator
	{
		return myNodes.begin();
	}
	template<typename T, pq::HeapType C>
	constexpr auto PriorityQueue<T, C>::end() const noexcept -> const_iterator
	{
		return myNodes.begin();
	}

	template<typename T, pq::HeapType C>
	constexpr auto PriorityQueue<T, C>::cbegin() const noexcept -> const_iterator
	{
		return myNodes.cbegin();
	}
	template<typename T, pq::HeapType C>
	constexpr auto PriorityQueue<T, C>::cend() const noexcept -> const_iterator
	{
		return myNodes.cbegin();
	}

	template<typename T, pq::HeapType C>
	constexpr auto PriorityQueue<T, C>::rbegin() const noexcept -> const_reverse_iterator
	{
		return myNodes.rbegin();
	}
	template<typename T, pq::HeapType C>
	constexpr auto PriorityQueue<T, C>::rend() const noexcept -> const_reverse_iterator
	{
		return myNodes.rbegin();
	}

	template<typename T, pq::HeapType C>
	constexpr auto PriorityQueue<T, C>::crbegin() const noexcept -> const_reverse_iterator
	{
		return myNodes.crbegin();
	}
	template<typename T, pq::HeapType C>
	constexpr auto PriorityQueue<T, C>::crend() const noexcept -> const_reverse_iterator
	{
		return myNodes.crbegin();
	}
}
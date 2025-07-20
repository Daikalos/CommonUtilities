#pragma once

#include <vector>

#include <CommonUtilities/Structures/FreeVector.hpp>
#include <CommonUtilities/Config.h>

#include "Random.hpp"

namespace CommonUtilities
{
	template<typename T>
	struct RandomBagItem
	{
		using value_type = T;

		T	item	{};
		int weight	{0};
	};

	template<typename T, bool Reverse>
	class RandomBagIterator
	{
	public:
		using iterator_concept	= std::contiguous_iterator_tag;
		using iterator_category = std::random_access_iterator_tag;
		using difference_type	= std::ptrdiff_t;
		using value_type		= std::remove_const_t<T>;
		using element_type		= T;
		using pointer			= T*;
		using reference			= T&;

		using item_element_type = std::conditional_t<std::is_const_v<T>, const typename T::value_type, typename T::value_type>;
		using item_pointer		= item_element_type*;
		using item_reference	= item_element_type&;

		static constexpr int DIR = Reverse ? -1 : 1;

		RandomBagIterator() noexcept = default;
		explicit RandomBagIterator(pointer aPtr) noexcept : myPtr(aPtr) {}

		template<bool OtherReverse> // able to convert non-const to const, but not the other way around
		RandomBagIterator(const RandomBagIterator<value_type, OtherReverse>& aRhs) noexcept : myPtr(aRhs.myPtr) {};

		template<bool OtherReverse>
		RandomBagIterator& operator=(const RandomBagIterator<value_type, OtherReverse>& aRhs) noexcept { myPtr = aRhs.myPtr; };

		item_reference operator*() const noexcept	{ return (*myPtr).item; }
		item_pointer operator->() const noexcept	{ return std::addressof(myPtr->item); }

		int get_weight() const noexcept { return (*myPtr).weight; }

		RandomBagIterator& operator++() noexcept	{ myPtr += DIR; return *this; }
		RandomBagIterator& operator--() noexcept	{ myPtr -= DIR; return *this; }

		RandomBagIterator operator++(int) noexcept	{ RandomBagIterator temp = *this; ++(*this); return temp; }
		RandomBagIterator operator--(int) noexcept	{ RandomBagIterator temp = *this; --(*this); return temp; }

		RandomBagIterator& operator+=(difference_type aOffset) noexcept { myPtr += aOffset; return *this; }
		RandomBagIterator& operator-=(difference_type aOffset) noexcept { return *this += -aOffset; }

		NODISC difference_type operator-(const RandomBagIterator& aRhs) const noexcept { return (myPtr - aRhs.myPtr) * DIR; }

		NODISC RandomBagIterator operator+(difference_type aOffset) const noexcept { return (RandomBagIterator(*this) += aOffset * DIR); }
		NODISC RandomBagIterator operator-(difference_type aOffset) const noexcept { return (RandomBagIterator(*this) += -aOffset * DIR); }

		item_reference operator[](difference_type aOffset) const noexcept { return (*(*this + aOffset * DIR)).item; }

		template<class U, bool OtherReverse>
		NODISC bool operator==(const RandomBagIterator<U, OtherReverse>& aRhs) const noexcept { return myPtr == aRhs.myPtr; }

		template<class U, bool OtherReverse>
		NODISC std::strong_ordering operator<=>(const RandomBagIterator<U, OtherReverse>& aRhs) const noexcept
		{
			if constexpr (Reverse)
			{
				return 0 <=> (myPtr <=> aRhs.myPtr);
			}
			else
			{
				return (myPtr <=> aRhs.myPtr);
			}
		}

		NODISC friend RandomBagIterator operator+(difference_type aLhs, const RandomBagIterator& aRhs)
		{
			return (aRhs + aLhs);
		}

	private:
		pointer myPtr;

		template<class U, bool Reverse>
		friend class RandomBagIterator;
	};

	template<typename T, typename Alloc = std::allocator<RandomBagItem<T>>>
	class RandomBag
	{
	public:
		using value_type		= T;
		using reference			= T&;
		using const_reference	= const T&;
		using pointer			= T*;
		using const_pointer		= const T*;
		using size_type			= std::size_t;
		using container_type	= std::vector<RandomBagItem<T>, Alloc>;

		using iterator					= RandomBagIterator<RandomBagItem<T>, false>;
		using const_iterator			= RandomBagIterator<const RandomBagItem<T>, false>;
		using reverse_iterator			= RandomBagIterator<RandomBagItem<T>, true>;
		using const_reverse_iterator	= RandomBagIterator<const RandomBagItem<T>, true>;

		using container_iterator				= typename container_type::iterator;
		using container_const_iterator			= typename container_type::const_iterator;
		using container_reverse_iterator		= typename container_type::reverse_iterator;
		using container_const_reverse_iterator	= typename container_type::const_reverse_iterator;

		constexpr explicit RandomBag(const Alloc& aAllocator);

		constexpr RandomBag() = default;
		constexpr ~RandomBag() noexcept(std::is_nothrow_destructible_v<T>) = default;

		NODISC constexpr auto operator[](size_type aIndex) -> reference;
		NODISC constexpr auto operator[](size_type aIndex) const -> const_reference;

		NODISC constexpr auto at(size_type aIndex) -> reference;
		NODISC constexpr auto at(size_type aIndex) const -> const_reference;

		NODISC constexpr auto front() -> reference;
		NODISC constexpr auto front() const -> const_reference;

		NODISC constexpr auto back() -> reference;
		NODISC constexpr auto back() const -> const_reference;

		NODISC constexpr bool empty() const noexcept;
		NODISC constexpr auto size() const noexcept -> size_type;

		NODISC constexpr auto capacity() const noexcept -> size_type;

		NODISC constexpr auto max_size() const noexcept -> size_type;

		constexpr void push_back(int aWeight, const T& aItem);
		constexpr void push_back(int aWeight, T&& aItem);

		template<typename... Args> requires(std::constructible_from<T, Args...>)
		constexpr auto emplace_back(int aWeight, Args&&... someArgs) -> reference;

		constexpr void pop_back();

		constexpr auto erase(const_iterator aPosition) -> iterator;
		constexpr auto erase(const_iterator aFirst, const_iterator aLast) -> iterator;

		template<typename... Args> requires std::constructible_from<T, Args...>
		constexpr auto emplace(const_iterator aPosition, int aWeight, Args&&... someArgs) -> iterator;

		constexpr auto insert(const_iterator aPosition, int aWeight, const_reference aElement) -> iterator;
		constexpr auto insert(const_iterator aPosition, int aWeight, T&& aElement) -> iterator;

		constexpr auto peek() const -> const_reference;
		constexpr auto pop() -> value_type;

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

		constexpr friend bool operator==(const RandomBag& aLeft, const RandomBag& aRight);

		constexpr friend bool operator==(const RandomBag& aLeft, const std::vector<T, Alloc>& aRight);

		constexpr friend bool operator==(const std::vector<T, Alloc>& aLeft, const RandomBag& aRight);

	private:
		container_type	myItems;
		int				myTotalWeight = 0;
	};

	template<class T, class Alloc>
	constexpr RandomBag<T, Alloc>::RandomBag(const Alloc& aAllocator)
		: myItems(aAllocator)
	{

	}

	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::operator[](size_type aIndex) -> reference
	{
		return myItems[aIndex].item;
	}
	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::operator[](size_type aIndex) const -> const_reference 
	{
		return myItems[aIndex].item;
	}

	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::at(size_type aIndex) -> reference 
	{
		return myItems.at(aIndex).item;
	}
	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::at(size_type aIndex) const -> const_reference 
	{
		return myItems.at(aIndex).item;
	}

	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::front() -> reference
	{
		return myItems.front().item;
	}
	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::front() const -> const_reference 
	{
		return myItems.front().item;
	}

	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::back() -> reference
	{
		return myItems.back().item;
	}
	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::back() const -> const_reference 
	{
		return myItems.back().item;
	}

	template<typename T, typename Alloc>
	constexpr bool RandomBag<T, Alloc>::empty() const noexcept
	{
		return myItems.empty();
	}
	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::size() const noexcept -> size_type
	{
		return myItems.size();
	}

	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::capacity() const noexcept -> size_type
	{
		return myItems.capacity();
	}

	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::max_size() const noexcept -> size_type 
	{
		return myItems.max_size();
	}

	template<typename T, typename Alloc>
	constexpr void RandomBag<T, Alloc>::push_back(int aWeight, const T& aItem) 
	{
		emplace_back(aWeight, aItem);
	}
	template<typename T, typename Alloc>
	constexpr void RandomBag<T, Alloc>::push_back(int aWeight, T&& aItem)
	{
		emplace_back(aWeight, std::move(aItem));
	}

	template<typename T, typename Alloc>
	template<typename... Args> requires(std::constructible_from<T, Args...>)
	constexpr auto RandomBag<T, Alloc>::emplace_back(int aWeight, Args&&... someArgs) -> reference
	{
		myTotalWeight += aWeight;
		return myItems.emplace_back(std::forward<Args>(someArgs)..., aWeight).item;
	}

	template<typename T, typename Alloc>
	constexpr void RandomBag<T, Alloc>::pop_back()
	{
		myTotalWeight -= myItems.back().weight;
		myItems.pop_back();
	}

	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::erase(const_iterator aPosition) -> iterator
	{
		const auto posIndex = std::distance(cbegin(), aPosition);

		myTotalWeight -= aPosition.get_weight();

		const auto eraseIt		= myItems.erase(myItems.cbegin() + posIndex);
		const auto eraseIndex	= std::distance(myItems.begin(), eraseIt);

		return (begin() + eraseIndex);
	}
	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::erase(const_iterator aFirst, const_iterator aLast) -> iterator
	{
		for (auto it = aFirst; it != aLast; ++it)
			myTotalWeight -= it.get_weight();

		const auto firstPos = std::distance(cbegin(), aFirst);
		const auto lastPos	= std::distance(cbegin(), aLast);

		const auto eraseIt		= myItems.erase(myItems.cbegin() + firstPos, myItems.cbegin() + lastPos);
		const auto eraseIndex	= std::distance(myItems.begin(), eraseIt);

		return (begin() + eraseIndex);
	}

	template<typename T, typename Alloc>
	template<typename... Args> requires std::constructible_from<T, Args...>
	constexpr auto RandomBag<T, Alloc>::emplace(const_iterator aPosition, int aWeight, Args&&... someArgs) -> iterator
	{
		const auto posIndex = std::distance(cbegin(), aPosition);

		myTotalWeight += aWeight;

		const auto insertIt		= myItems.emplace(myItems.cbegin() + posIndex, std::forward<Args>(someArgs)..., aWeight);
		const auto insertIndex	= std::distance(myItems.begin(), insertIt);

		return (begin() + insertIndex);
	}

	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::insert(const_iterator aPosition, int aWeight, const_reference aElement) -> iterator
	{
		return emplace(aPosition, aElement);
	}
	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::insert(const_iterator aPosition, int aWeight, T&& aElement) -> iterator
	{
		return emplace(aPosition, std::move(aElement));
	}

	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::peek() const -> const_reference
	{
		assert(!empty() && "Container cannot be empty!");

		int randomVal		= Random(0, myTotalWeight);
		int currentWeight	= 0;

		for (const auto& [item, weight] : myItems)
		{
			currentWeight += weight;
			if (currentWeight >= randomVal)
				return item;
		}

		assert(false && "Reaching here should be impossible");
		// std::unreachable

		return myItems.back().item;
	}
	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::pop() -> value_type 
	{
		assert(!empty() && "Container cannot be empty!");

		int randomVal		= Random(0, myTotalWeight);
		int currentWeight	= 0;

		for (int i = 0; i < myItems.size(); ++i)
		{
			currentWeight += myItems[i].weight;
			if (currentWeight >= randomVal)
			{
				T result = std::move(myItems[i].item);

				myTotalWeight -= myItems[i].weight;
				myItems.erase(myItems.cbegin() + i);

				return result;
			}
		}

		assert(false && "Reaching here should be impossible");

		return T{};
	}

	template<typename T, typename Alloc>
	constexpr void RandomBag<T, Alloc>::swap(RandomBag& aOther)
	{
		std::swap(myItems, aOther.myItems);
		std::swap(myTotalWeight, aOther.myTotalWeight);
	}

	template<typename T, typename Alloc>
	constexpr void RandomBag<T, Alloc>::reserve(size_type aCapacity)
	{
		myItems.reserve(aCapacity);
	}

	template<typename T, typename Alloc>
	constexpr void RandomBag<T, Alloc>::clear()
	{
		myItems.clear();
		myTotalWeight = 0;
	}

	template<typename T, typename Alloc>
	constexpr void RandomBag<T, Alloc>::shrink_to_fit() 
	{
		myItems.shrink_to_fit();
	}

	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::begin() noexcept -> iterator 
	{
		return iterator(myItems.data());
	}
	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::end() noexcept -> iterator
	{
		return iterator(myItems.data() + myItems.size());
	}
	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::begin() const noexcept -> const_iterator
	{
		return const_iterator(myItems.data());
	}
	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::end() const noexcept -> const_iterator
	{
		return const_iterator(myItems.data() + myItems.size());
	}
	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::cbegin() const noexcept -> const_iterator 
	{
		return begin();
	}
	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::cend() const noexcept -> const_iterator
	{
		return end();
	}

	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::rbegin() noexcept -> reverse_iterator 
	{
		return reverse_iterator(myItems.data() + myItems.size() - 1);
	}
	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::rend() noexcept -> reverse_iterator
	{
		return reverse_iterator(myItems.data() - 1);
	}
	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::rbegin() const noexcept -> const_reverse_iterator 
	{
		return const_reverse_iterator(myItems.data() + myItems.size() - 1);
	}
	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::rend() const noexcept -> const_reverse_iterator 
	{
		return const_reverse_iterator(myItems.data() - 1);
	}
	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::crbegin() const noexcept -> const_reverse_iterator 
	{
		return rbegin();
	}
	template<typename T, typename Alloc>
	constexpr auto RandomBag<T, Alloc>::crend() const noexcept -> const_reverse_iterator 
	{
		return rend();
	}

	// GLOBAL FUNCTIONS

	template<typename T, typename Alloc>
	constexpr void swap(RandomBag<T, Alloc>& aLeft, RandomBag<T, Alloc>& aRight)
	{
		aLeft.swap(aRight);
	}

	template<typename T, typename Alloc>
	NODISC constexpr bool operator==(const RandomBag<T, Alloc>& aLeft, const RandomBag<T, Alloc>& aRight) noexcept
	{
		return (aLeft.myItems == aRight.myItems);
	}

	template<typename T, typename Alloc>
	NODISC constexpr bool operator==(const RandomBag<T, Alloc>& aLeft, const std::vector<T, Alloc>& aRight)
	{
		if (aLeft.myItems.size() != aRight.size())
			return false;

		for (std::size_t i = 0; i < aLeft.myItems.size(); ++i)
		{
			if (aLeft.myItems[i].item != aRight[i])
				return false;
		}

		return true;
	}

	template<typename T, typename Alloc>
	NODISC constexpr bool operator==(const std::vector<T, Alloc>& aLeft, const RandomBag<T, Alloc>& aRight)
	{
		return (aRight == aLeft);
	}

	template<typename T, typename Alloc>
	NODISC constexpr bool operator!=(const RandomBag<T, Alloc>& aLeft, const RandomBag<T, Alloc>& aRight) noexcept
	{
		return !(aLeft == aRight);
	}

	template<typename T, typename Alloc>
	NODISC constexpr bool operator!=(const RandomBag<T, Alloc>& aLeft, const std::vector<T, Alloc>& aRight) noexcept
	{
		return !(aLeft == aRight);
	}

	template<typename T, typename Alloc>
	NODISC constexpr bool operator!=(const std::vector<T, Alloc>& aLeft, const RandomBag<T, Alloc>& aRight) noexcept
	{
		return !(aLeft == aRight);
	}
}
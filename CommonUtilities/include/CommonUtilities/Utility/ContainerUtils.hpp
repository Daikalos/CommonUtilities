#pragma once

#include <vector>
#include <array>
#include <ranges>
#include <span>
#include <cassert>
#include <algorithm>

#include <CommonUtilities/Utility/Concepts.hpp>
#include <CommonUtilities/Config.h>

#if !C20_SUPPORT
#error Container utilities use C++20 features; please change the language standard or don't include the header that caused this message.
#endif

namespace CommonUtilities::ctr
{
	template<typename It, typename Func>
	inline auto ForEachUntil(It aBegin, It aEnd, Func&& aFunc)
	{
		return std::ranges::all_of(aBegin, aEnd, std::forward<Func>(aFunc));
	}

	template<typename T>
	inline bool Erase(std::vector<T>& aVector, const T& aCompare)
	{
		auto it = std::ranges::find(aVector, aCompare);

		if (it == aVector.end())
			return false;

		aVector.erase(it);

		return true;
	}

	template<typename T, class Comp> requires (!std::is_same_v<std::decay_t<T>, std::decay_t<Comp>>)
	inline bool Erase(std::vector<T>& aVector, Comp&& aCompare)
	{
		auto it = std::ranges::find_if(aVector, std::forward<Comp>(aCompare));

		if (it == aVector.end()) // do not erase if not found
			return false;

		aVector.erase(it);

		return true;
	}

	template<typename T>
	inline bool EraseCyclic(std::vector<T>& aVector, const T& aItem)
	{
		auto it = std::ranges::find(aVector, aItem);

		if (it == aVector.end())
			return false;

		EraseCyclicAt(aVector, it);

		return true;
	}

	template<typename T, class Comp> requires (!std::is_same_v<std::decay_t<T>, std::decay_t<Comp>>)
	inline bool EraseCyclic(std::vector<T>& aVector, Comp&& aCompare)
	{
		auto it = std::ranges::find_if(aVector, std::forward<Comp>(aCompare));

		if (it == aVector.end())
			return false;

		EraseCyclicAt(aVector, it);

		return true;
	}

	template<typename T>
	inline void EraseCyclicAt(T& aContainer, std::size_t aIndex)
	{
		assert(aIndex < aContainer.size());

		if (aIndex != aContainer.size() - 1)
		{
			aContainer[aIndex] = std::move(aContainer.back());
		}

		aContainer.pop_back();
	}

	template<typename T, typename Iter>
	inline void EraseCyclicAt(T& aContainer, Iter aIterator)
	{
		assert(aIterator != aContainer.end());

		if (aIterator != aContainer.end() - 1)
		{
			*aIterator = std::move(aContainer.back());
		}

		aContainer.pop_back();
	}

	template<typename T>
	inline void MoveTo(std::vector<T>& aVector, std::size_t aOldIndex, std::size_t aNewIndex)
	{
		if (aOldIndex > aNewIndex)
		{
			std::ranges::rotate(aVector.rend() - aOldIndex - 1, aVector.rend() - aOldIndex, aVector.rend() - aNewIndex);
		}
		else
		{
			std::ranges::rotate(aVector.begin() + aOldIndex, aVector.begin() + aOldIndex + 1, aVector.begin() + aNewIndex + 1);
		}
	}

	template<typename T>
	inline auto InsertSorted(std::vector<T>& aVector, const T& aItem)
	{
		// insert at upper bound since that usually means that less items will have to be moved
		return aVector.insert(std::ranges::upper_bound(aVector, aItem), aItem);
	}

	template<typename T, typename Comp>
	inline auto InsertSorted(std::vector<T>& aVector, const T& aItem, Comp&& aCompare)
	{
		return aVector.insert(std::ranges::upper_bound(aVector, aItem, std::forward<Comp>(aCompare)), aItem);
	}

	template<typename T>
	inline bool EraseSorted(std::vector<T>& aVector, const T& aItem)
	{
		const auto lb = std::ranges::lower_bound(aVector, aItem);

		if (lb != aVector.cend() && *lb == aItem)
		{
			const auto ub = std::ranges::upper_bound(lb, aVector.end(), aItem);
			aVector.erase(lb, ub); // remove all equal elements in range

			return true;
		}

		return false;
	}

	template<typename T, typename Comp>
	inline bool EraseSorted(std::vector<T>& aVector, const T& aItem, Comp&& aCompare)
	{
		const auto lb = std::ranges::lower_bound(aVector, aItem, std::forward<Comp>(aCompare));

		if (lb != aVector.cend() && *lb == aItem)
		{
			const auto ub = std::ranges::upper_bound(lb, aVector.end(), aItem, std::forward<Comp>(aCompare));
			aVector.erase(lb, ub); // remove all equal elements in range

			return true;
		}

		return false;
	}

	template<typename T>
	inline bool InsertUniqueSorted(std::vector<T>& aVector, const T& aItem)
	{
		const auto it = std::ranges::upper_bound(aVector, aItem);

		if (it == aVector.cend() || *it != aItem)
		{
			aVector.insert(it, aItem);
			return true;
		}

		return false;
	}

	template<typename T, typename Comp>
	inline bool InsertUniqueSorted(std::vector<T>& aVector, const T& aItem, Comp&& aCompare)
	{
		const auto it = std::ranges::upper_bound(aVector, aItem, std::forward<Comp>(aCompare));

		if (it == aVector.cend() || *it != aItem)
		{
			aVector.insert(it, aItem);
			return true;
		}

		return false;
	}

	template<typename T>
	inline bool EraseUniqueSorted(std::vector<T>& aVector, const T& aItem)
	{
		const auto it = std::ranges::upper_bound(aVector, aItem);

		if (it != aVector.cend() && *it == aItem)
		{
			aVector.erase(it);
			return true;
		}

		return false;
	}

	template<typename T, typename Comp>
	inline bool EraseUniqueSorted(std::vector<T>& aVector, const T& aItem, Comp&& aCompare)
	{
		const auto it = std::ranges::upper_bound(aVector, aItem, std::forward<Comp>(aCompare));

		if (it != aVector.cend() && *it == aItem)
		{
			aVector.erase(it);
			return true;
		}

		return false;
	}

	template<typename T>
	constexpr void Sort(std::span<const T> someItems)
	{
		std::ranges::sort(someItems);
	}

	template<typename T>
	NODISC constexpr T& Sort(T& someItems)
	{
		std::ranges::sort(someItems);
		return someItems;
	}

	template<typename T, typename Comp>
	NODISC constexpr T& Sort(T& someItems, Comp&& aCompare)
	{
		std::ranges::sort(someItems, std::forward<Comp>(aCompare));
		return someItems;
	}

	template<typename T>
	NODISC constexpr T Sort(T&& someItems)
	{
		std::ranges::sort(someItems);
		return someItems;
	}

	template<typename T, typename Comp>
	NODISC constexpr T Sort(T&& someItems, Comp&& aCompare)
	{
		std::ranges::sort(someItems, std::forward<Comp>(aCompare));
		return someItems;
	}

	template<typename T>
	NODISC constexpr void ApplyPermutation(T& aContainer, std::span<const std::size_t> someIndices)
	{
		assert(aContainer.size() == someIndices.size() && "Vector and indices must be of equal size");

		using std::swap; // koenig lookup

		std::vector<bool> done(aContainer.size());
		for (std::size_t i = 0; i < aContainer.size(); ++i)
		{
			if (done[i])
				continue;

			done[i] = true;

			std::size_t current = i;
			std::size_t next = someIndices[i];

			while (i != next)
			{
				done[next] = true;

				swap(aContainer[current], aContainer[next]);

				next = someIndices[next];
				current = next;
			}
		}
	}

	template<typename T>
	NODISC constexpr bool IsSorted(std::span<const T> someItems)
	{
		return std::ranges::is_sorted(someItems);
	}

	template<typename T, typename Comp>
	NODISC constexpr bool IsSorted(std::span<const T> someItems, Comp&& aCompare)
	{
		return std::ranges::is_sorted(someItems, std::forward<Comp>(aCompare));
	}

	template<typename T, typename U, typename V>
	NODISC constexpr V Merge(const T& aFirst, const U& aSecond)
	{
		V result{};
		std::ranges::set_union(aFirst, aSecond, std::begin(result));
		return result;
	}
}
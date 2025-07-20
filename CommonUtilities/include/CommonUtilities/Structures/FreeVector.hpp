#pragma once

#include <vector>
#include <variant>
#include <functional>
#include <cassert>

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	/// Structure that enables for quick insertion and removal from anywhere in the container.
	/// 
	template<class T, class Alloc = std::allocator<std::variant<T, std::int64_t>>>
	class FreeVector
	{
	public:
		using value_type		= T;
		using reference			= T&;
		using const_reference	= const T&;
		using pointer			= T*;
		using const_pointer		= const T*;
		using size_type			= std::size_t;

		using container_type = std::vector<std::variant<T, std::int64_t>, Alloc>;

		constexpr explicit FreeVector(const Alloc& aAllocator);

		constexpr FreeVector() = default;
		constexpr ~FreeVector() = default;

		constexpr FreeVector(const FreeVector&) = default;
		constexpr FreeVector(FreeVector&&) noexcept = default;

		constexpr auto operator=(const FreeVector&) -> FreeVector& = default;
		constexpr auto operator=(FreeVector&&) noexcept -> FreeVector& = default;

		/// \param anIndex: Index to element in container.
		/// 
		/// \returns Reference to element.
		/// 
		NODISC constexpr auto operator[](size_type anIndex) -> reference;

		/// \param anIndex: Index to element in container.
		/// 
		/// \returns Const reference to element.
		/// 
		NODISC constexpr auto operator[](size_type anIndex) const -> const_reference;

		/// \param anIndex: Index to element in container.
		/// 
		/// \returns Reference to element.
		/// 
		NODISC constexpr auto at(size_type anIndex) -> reference;

		/// \param anIndex: Index to element in container.
		/// 
		/// \returns Const reference to element.
		/// 
		NODISC constexpr auto at(size_type anIndex) const -> const_reference;

		/// \param anIndex: Index to element in container.
		/// 
		/// \returns Whether element is valid or not.
		/// 
		NODISC constexpr bool valid(size_type anIndex) const;

		/// \returns If it contains any valid elements.
		/// 
		NODISC constexpr bool empty() const noexcept;

		/// \returns Number of elements in the container.
		/// 
		NODISC constexpr auto size() const noexcept -> size_type;

		/// \returns Number of valid elements in the container.
		/// 
		NODISC constexpr auto count() const noexcept -> size_type;

		/// \returns Capacity of the container.
		/// 
		NODISC constexpr auto capacity() const noexcept -> size_type;

		/// \returns Maximum number of elements allowed.
		/// 
		NODISC constexpr auto max_size() const noexcept -> size_type;

		/// Emplace element in container.
		/// 
		/// \param someArgs: Optional parameters to construct element.
		/// 
		/// \returns Index to access constructed element.
		/// 
		template<typename... Args> requires std::constructible_from<T, Args...>
		constexpr auto emplace(Args&&... someArgs) -> size_type;

		/// Add element to container.
		/// 
		/// \param anElement: Element to add.
		/// 
		/// \returns Index to access added element.
		/// 
		constexpr auto insert(const T& anElement) -> size_type;

		/// Add element to container.
		/// 
		/// \param anElement: Element to add.
		/// 
		/// \returns Index to access added element.
		/// 
		constexpr auto insert(T&& anElement) -> size_type;

		/// Removes element at index. Element at index must be valid.
		/// 
		/// \param anIndex: Index to remove element at.
		/// 
		constexpr void erase(size_type anIndex);

		/// Clears structure of all elements and resets itself to initial state.
		/// 
		constexpr void clear();

		/// Reserve to reduce reallocation.
		/// 
		constexpr void reserve(size_type aCapacity);

		/// Swap two FreeVector.
		/// 
		constexpr void swap(FreeVector& aOther);

		template<typename Func>
		constexpr void for_each(const Func& func) const;

		template<typename Func>
		constexpr void for_each(const Func& func);

		constexpr friend bool operator==(FreeVector& aLeft, FreeVector& aRight);

	private:
		container_type	myData;
		std::int64_t	myFirstFree {-1};
		size_type		myCount		{0};
	};

	template<class T, class Alloc>
	constexpr FreeVector<T, Alloc>::FreeVector(const Alloc& aAllocator)
		: myData(aAllocator)
	{

	}

	template<class T, class Alloc>
	constexpr auto FreeVector<T, Alloc>::operator[](size_type anIndex) -> reference
	{
		return std::get<T>(myData[anIndex]);
	}

	template<class T, class Alloc>
	constexpr auto FreeVector<T, Alloc>::operator[](size_type anIndex) const -> const_reference
	{
		return std::get<T>(myData[anIndex]);
	}

	template<class T, class Alloc>
	constexpr auto FreeVector<T, Alloc>::at(size_type anIndex) -> reference
	{
		return std::get<T>(myData.at(anIndex));
	}

	template<class T, class Alloc>
	constexpr auto FreeVector<T, Alloc>::at(size_type anIndex) const -> const_reference
	{
		return std::get<T>(myData.at(anIndex));
	}

	template<class T, class Alloc>
	constexpr bool FreeVector<T, Alloc>::valid(size_type anIndex) const
	{
		return myData.at(anIndex).index() == 0;
	}

	template<class T, class Alloc>
	constexpr bool FreeVector<T, Alloc>::empty() const noexcept
	{
		return myCount == 0;
	}

	template<class T, class Alloc>
	constexpr auto FreeVector<T, Alloc>::size() const noexcept -> size_type
	{
		return static_cast<size_type>(myData.size());
	}

	template<class T, class Alloc>
	constexpr auto FreeVector<T, Alloc>::count() const noexcept -> size_type
	{
		return myCount;
	}

	template<class T, class Alloc>
	constexpr auto FreeVector<T, Alloc>::capacity() const noexcept -> size_type
	{
		return static_cast<size_type>(myData.capacity());
	}

	template<class T, class Alloc>
	constexpr auto FreeVector<T, Alloc>::max_size() const noexcept -> size_type
	{
		return static_cast<size_type>(myData.max_size());
	}

	template<class T, class Alloc>
	template<typename... Args> requires std::constructible_from<T, Args...>
	constexpr auto FreeVector<T, Alloc>::emplace(Args&&... someArgs) -> size_type
	{
		size_type index = 0;

		if (myFirstFree != -1)
		{
			assert(!valid(myFirstFree));

			index = myFirstFree;

			myFirstFree		= std::get<std::int64_t>(myData[myFirstFree]);
			myData[index]	= std::variant<T, std::int64_t>(std::in_place_index<0>, std::forward<Args>(someArgs)...);
		}
		else
		{
			assert(myData.size() == myCount); // should be the same if no more available space

			index = myData.size();
			myData.emplace_back(std::in_place_index<0>, std::forward<Args>(someArgs)...);
		}

		++myCount;

		return index;
	}

	template<class T, class Alloc>
	constexpr auto FreeVector<T, Alloc>::insert(const T& anElement) -> size_type
	{
		return emplace(anElement);
	}

	template<class T, class Alloc>
	constexpr auto FreeVector<T, Alloc>::insert(T&& anElement) -> size_type
	{
		return emplace(std::move(anElement));
	}

	template<class T, class Alloc>
	constexpr void FreeVector<T, Alloc>::erase(size_type anIndex)
	{
		assert(anIndex >= 0 && anIndex < myData.size() && valid(anIndex));

		myData[anIndex] = myFirstFree;
		myFirstFree = anIndex;

		--myCount;
	}

	template<class T, class Alloc>
	constexpr void FreeVector<T, Alloc>::clear()
	{
		myData.clear();
		myFirstFree = -1;
		myCount = 0;
	}

	template<class T, class Alloc>
	constexpr void FreeVector<T, Alloc>::reserve(size_type aCapacity)
	{
		myData.reserve(aCapacity);
	}

	template<class T, class Alloc>
	constexpr void FreeVector<T, Alloc>::swap(FreeVector& aOther)
	{
		std::swap(myData,		aOther.myData);
		std::swap(myFirstFree,	aOther.myFirstFree);
		std::swap(myCount,		aOther.myCount);
	}

	template<class T, class Alloc>
	template<typename Func>
	constexpr void FreeVector<T, Alloc>::for_each(const Func& func) const
	{
		for (const auto& elt : myData)
		{
			if (elt.index() == 0)
				func(std::get<T>(elt));
		}
	}

	template<class T, class Alloc>
	template<typename Func>
	constexpr void FreeVector<T, Alloc>::for_each(const Func& func)
	{
		for (auto& elt : myData)
		{
			if (elt.index() == 0)
				func(std::get<T>(elt));
		}
	}

	// GLOBAL FUNCTIONS

	template<class T, class Alloc>
	constexpr void swap(FreeVector<T, Alloc>& aLeft, FreeVector<T, Alloc>& aRight)
	{
		aLeft.swap(aRight);
	}

	template<class T, class Alloc>
	NODISC constexpr bool operator==(const FreeVector<T, Alloc>& aLeft, const FreeVector<T, Alloc>& aRight) noexcept
	{
		return	(aLeft.myData == aRight.myData) &&
				(aLeft.myFirstFree == aRight.myFirstFree) &&
				(aLeft.myCount == aRight.myCount);
	}

	template<class T, class Alloc>
	NODISC constexpr bool operator!=(const FreeVector<T, Alloc>& aLeft, const FreeVector<T, Alloc>& aRight) noexcept
	{
		return !(aLeft == aRight);
	}
}
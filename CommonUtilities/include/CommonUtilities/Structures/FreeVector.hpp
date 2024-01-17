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
	template<class T>
	class FreeVector
	{
	public:
		using value_type		= T;
		using reference			= T&;
		using const_reference	= const T&;
		using pointer			= T*;
		using const_pointer		= const T*;
		using size_type			= std::size_t;

		constexpr FreeVector() = default;
		constexpr ~FreeVector() = default;

		constexpr FreeVector(const FreeVector&) = default;
		constexpr FreeVector(FreeVector&&) noexcept = default;

		constexpr auto operator=(const FreeVector&) -> FreeVector& = default;
		constexpr auto operator=(FreeVector&&) noexcept -> FreeVector& = default;

		NODISC constexpr auto operator[](size_type anIndex) -> reference;
		NODISC constexpr auto operator[](size_type anIndex) const -> const_reference;

		NODISC constexpr auto at(size_type anIndex) -> reference;
		NODISC constexpr auto at(size_type anIndex) const -> const_reference;

		NODISC constexpr bool valid(size_type anIndex) const;

		NODISC constexpr bool empty() const noexcept;
		NODISC constexpr auto size() const noexcept -> size_type;
		NODISC constexpr auto count() const noexcept -> size_type;

		NODISC constexpr auto max_size() const noexcept -> size_type;

		template<typename... Args> requires std::constructible_from<T, Args...>
		constexpr auto emplace(Args&&... someArgs) -> size_type;

		constexpr auto insert(const T& anElement) -> size_type;
		constexpr auto insert(T&& anElement) -> size_type;

		constexpr void erase(size_type anIndex);

		constexpr void clear();

		constexpr void reserve(size_type aCapacity);

		template<typename Func>
		constexpr void for_each(const Func& func) const;

	private:
		using container_type = std::vector<std::variant<T, std::int64_t>>;

		container_type	myData;
		std::int64_t	myFirstFree {-1};
		size_type		myCount		{0};
	};

	template<class T>
	constexpr auto FreeVector<T>::operator[](size_type anIndex) -> reference
	{
		return std::get<T>(myData[anIndex]);
	}

	template<class T>
	constexpr auto FreeVector<T>::operator[](size_type anIndex) const -> const_reference
	{
		return std::get<T>(myData[anIndex]);
	}

	template<class T>
	constexpr auto FreeVector<T>::at(size_type anIndex) -> reference
	{
		return std::get<T>(myData.at(anIndex));
	}

	template<class T>
	constexpr auto FreeVector<T>::at(size_type anIndex) const -> const_reference
	{
		return std::get<T>(myData.at(anIndex));
	}

	template<class T>
	constexpr bool FreeVector<T>::valid(size_type anIndex) const
	{
		return myData.at(anIndex).index() == 0;
	}

	template<class T>
	constexpr bool FreeVector<T>::empty() const noexcept
	{
		return myCount == 0;
	}

	template<class T>
	constexpr auto FreeVector<T>::size() const noexcept -> size_type
	{
		return static_cast<size_type>(myData.size());
	}

	template<class T>
	constexpr auto FreeVector<T>::count() const noexcept -> size_type
	{
		return myCount;
	}

	template<class T>
	constexpr auto FreeVector<T>::max_size() const noexcept -> size_type
	{
		return static_cast<size_type>(myData.max_size());
	}

	template<class T>
	template<typename... Args> requires std::constructible_from<T, Args...>
	constexpr auto FreeVector<T>::emplace(Args&&... someArgs) -> size_type
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

	template<class T>
	constexpr auto FreeVector<T>::insert(const T& anElement) -> size_type
	{
		return emplace(anElement);
	}

	template<class T>
	constexpr auto FreeVector<T>::insert(T&& anElement) -> size_type
	{
		return emplace(std::move(anElement));
	}

	template<class T>
	constexpr void FreeVector<T>::erase(size_type anIndex)
	{
		assert(anIndex >= 0 && anIndex < myData.size() && valid(anIndex));

		myData[anIndex] = myFirstFree;
		myFirstFree = anIndex;

		--myCount;
	}

	template<class T>
	constexpr void FreeVector<T>::clear()
	{
		myData.clear();
		myFirstFree = -1;
		myCount = 0;
	}

	template<class T>
	constexpr void FreeVector<T>::reserve(size_type aCapacity)
	{
		myData.reserve(aCapacity);
	}

	template<class T>
	template<typename Func>
	constexpr void FreeVector<T>::for_each(const Func& func) const
	{
		for (const auto& elt : myData)
		{
			if (elt.index() == 0)
				func(std::get<T>(elt));
		}
	}
}
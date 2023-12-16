#pragma once

#include <array>
#include <utility>
#include <cassert>

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T, std::size_t SIZE>
	class StackVector
	{
	public:
		using value_type			= T;
		using pointer				= T*;
		using const_pointer			= const T*;
		using reference				= T&;
		using const_reference		= const T&;
		using size_type				= std::size_t;

		using array_type				= std::array<T, SIZE>;
		using iterator					= typename array_type::iterator;
		using const_iterator			= typename array_type::const_iterator;
		using reverse_iterator			= typename array_type::reverse_iterator;
		using const_reverse_iterator	= typename array_type::const_reverse_iterator;

		CONSTEXPR StackVector(); 
		CONSTEXPR StackVector(const StackVector& aOther);
		CONSTEXPR StackVector(StackVector&& aOther) noexcept;
		CONSTEXPR StackVector(std::initializer_list<T> aInitList);
		CONSTEXPR ~StackVector();

		CONSTEXPR auto operator=(const StackVector& aOther) -> StackVector&;
		CONSTEXPR auto operator=(StackVector&& aOther) noexcept -> StackVector&;

		NODISC CONSTEXPR auto operator[](size_type anIndex) const -> const_reference;
		NODISC CONSTEXPR auto operator[](size_type anIndex) -> reference;

		template<typename... Args> requires std::constructible_from<T, Args...>
		CONSTEXPR auto emplace_back(Args&&... someArgs) -> reference;

		CONSTEXPR void push_back(const T& anObject);
		CONSTEXPR void push_back(T&& anObject);

		CONSTEXPR void insert(size_type anIndex, const_reference anObject);

		CONSTEXPR void erase_cyclic(const_reference anObject);
		CONSTEXPR void erase_cyclic_at(size_type anIndex);

		CONSTEXPR void erase(const_reference anObject);
		CONSTEXPR void erase_at(size_type anIndex);

		CONSTEXPR void clear();

		NODISC CONSTEXPR auto size() const -> size_type;
		NODISC CONSTEXPR auto max_size() const -> size_type;
		NODISC CONSTEXPR bool empty() const;

		NODISC CONSTEXPR auto begin() noexcept -> iterator;
		NODISC CONSTEXPR auto end() noexcept -> iterator;
		NODISC CONSTEXPR auto begin() const noexcept -> const_iterator;
		NODISC CONSTEXPR auto end() const noexcept -> const_iterator;
		NODISC CONSTEXPR auto cbegin() const noexcept -> const_iterator;
		NODISC CONSTEXPR auto cend() const noexcept -> const_iterator;

		NODISC CONSTEXPR auto rbegin() noexcept -> reverse_iterator;
		NODISC CONSTEXPR auto rend() noexcept -> reverse_iterator;
		NODISC CONSTEXPR auto rbegin() const noexcept -> const_reverse_iterator;
		NODISC CONSTEXPR auto rend() const noexcept -> const_reverse_iterator;
		NODISC CONSTEXPR auto crbegin() const noexcept -> const_reverse_iterator;
		NODISC CONSTEXPR auto crend() const noexcept -> const_reverse_iterator;

	private:
		typedef std::make_signed_t<size_type> SignedType;

		std::array<T, SIZE> myArray;
		size_type mySize;
	};

	template<typename T, std::size_t SIZE>
	CONSTEXPR StackVector<T, SIZE>::StackVector()
		: myArray(), mySize()
	{

	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR StackVector<T, SIZE>::StackVector(const StackVector& aOther)
		: StackVector()
	{
		for (; mySize < aOther.size(); ++mySize)
		{
			myArray[mySize] = aOther.myArray[mySize];
		}
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR StackVector<T, SIZE>::StackVector(StackVector&& aOther) noexcept
		: StackVector()
	{
		std::move(aOther.begin(), aOther.end(), begin());
		mySize = aOther.mySize;
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR StackVector<T, SIZE>::StackVector(std::initializer_list<T> aInitList)
		: StackVector()
	{
		assert(aInitList.size() <= SIZE && "Initializer list must be smaller than the maximum capacity!");

		for (; mySize < aInitList.size(); ++mySize)
		{
			myArray[mySize] = *(aInitList.begin() + mySize);
		}
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR StackVector<T, SIZE>::~StackVector() = default;

	template<typename T, std::size_t SIZE>
	CONSTEXPR auto StackVector<T, SIZE>::operator=(const StackVector& aOther) -> StackVector&
	{
		for (mySize = 0; mySize < aOther.size(); ++mySize)
		{
			myArray[mySize] = aOther.myArray[mySize];
		}

		return *this;
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR auto StackVector<T, SIZE>::operator=(StackVector&& aOther) noexcept -> StackVector&
	{
		std::move(aOther.begin(), aOther.end(), begin());

		mySize = aOther.mySize;
		aOther.mySize = 0;

		return *this;
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR auto StackVector<T, SIZE>::operator[](size_type anIndex) const -> const_reference
	{
		assert(anIndex >= 0 && anIndex < mySize && "Index must be within range of the array!");
		return myArray[anIndex];
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR auto StackVector<T, SIZE>::operator[](size_type anIndex) -> reference
	{
		assert(anIndex >= 0 && anIndex < mySize && "Index must be within range of the array!");
		return myArray[anIndex];
	}

	template<typename T, std::size_t SIZE>
	template<typename... Args> requires std::constructible_from<T, Args...>
	CONSTEXPR auto StackVector<T, SIZE>::emplace_back(Args&&... someArgs) -> reference
	{
		assert(mySize < SIZE && "No more capacity available!");

		myArray[mySize] = T(std::forward<Args>(someArgs)...);

		return myArray[mySize++];
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR void StackVector<T, SIZE>::push_back(const T& anObject)
	{
		emplace_back(anObject);
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR void StackVector<T, SIZE>::push_back(T&& anObject)
	{
		emplace_back(std::move(anObject));
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR void StackVector<T, SIZE>::insert(size_type anIndex, const_reference anObject)
	{
		assert(anIndex >= 0 && anIndex <= mySize && "Index must be within range of the array!");
		assert(mySize < SIZE && "No more capacity available!");

		for (SignedType index = static_cast<SignedType>(mySize) - 1; index >= static_cast<SignedType>(anIndex); --index)
		{
			myArray[index + 1] = std::move(myArray[index]);
		}

		myArray[anIndex] = anObject;

		++mySize;
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR void StackVector<T, SIZE>::erase_cyclic(const_reference anObject)
	{
		for (size_type index = 0; index < mySize; ++index)
		{
			if (myArray[index] == anObject)
			{
				erase_cyclic_at(index);
				break;
			}
		}
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR void StackVector<T, SIZE>::erase_cyclic_at(size_type anIndex)
	{
		assert(anIndex >= 0 && anIndex < mySize && "Index must be within range of array!");

		myArray[anIndex] = std::move(myArray[mySize - 1]);
		--mySize;
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR void StackVector<T, SIZE>::erase(const_reference anObject)
	{
		for (size_type index = 0; index < mySize; ++index)
		{
			if (myArray[index] == anObject)
			{
				erase_at(index);
				break;
			}
		}
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR void StackVector<T, SIZE>::erase_at(size_type anIndex)
	{
		for (size_type index = anIndex; index < mySize - 1; ++anIndex)
		{
			myArray[index] = std::move(myArray[index + 1]);
		}

		--mySize;
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR void StackVector<T, SIZE>::clear()
	{
		mySize = 0;
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR auto StackVector<T, SIZE>::size() const -> size_type
	{
		return mySize;
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR auto StackVector<T, SIZE>::max_size() const -> size_type
	{
		return static_cast<size_type>(myArray.size());
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR bool StackVector<T, SIZE>::empty() const
	{
		return mySize == 0;
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR auto StackVector<T, SIZE>::begin() noexcept -> iterator
	{
		return iterator(myArray.data(), 0);
	}
	template<typename T, std::size_t SIZE>
	CONSTEXPR auto StackVector<T, SIZE>::end() noexcept -> iterator
	{
		return iterator(myArray.data(), mySize);
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR auto StackVector<T, SIZE>::begin() const noexcept -> const_iterator
	{
		return const_iterator(myArray.data(), 0);
	}
	template<typename T, std::size_t SIZE>
	CONSTEXPR auto StackVector<T, SIZE>::end() const noexcept -> const_iterator
	{
		return const_iterator(myArray.data(), mySize);
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR auto StackVector<T, SIZE>::cbegin() const noexcept -> const_iterator
	{
		return begin();
	}
	template<typename T, std::size_t SIZE>
	CONSTEXPR auto StackVector<T, SIZE>::cend() const noexcept -> const_iterator
	{
		return end();
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR auto StackVector<T, SIZE>::rbegin() noexcept -> reverse_iterator
	{
		return reverse_iterator(end());
	}
	template<typename T, std::size_t SIZE>
	CONSTEXPR auto StackVector<T, SIZE>::rend() noexcept -> reverse_iterator
	{
		return reverse_iterator(begin());
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR auto StackVector<T, SIZE>::rbegin() const noexcept -> const_reverse_iterator
	{
		return const_reverse_iterator(end());
	}
	template<typename T, std::size_t SIZE>
	CONSTEXPR auto StackVector<T, SIZE>::rend() const noexcept -> const_reverse_iterator
	{
		return const_reverse_iterator(begin());
	}

	template<typename T, std::size_t SIZE>
	CONSTEXPR auto StackVector<T, SIZE>::crbegin() const noexcept -> const_reverse_iterator
	{
		return const_reverse_iterator(end());
	}
	template<typename T, std::size_t SIZE>
	CONSTEXPR auto StackVector<T, SIZE>::crend() const noexcept -> const_reverse_iterator
	{
		return const_reverse_iterator(begin());
	}
}
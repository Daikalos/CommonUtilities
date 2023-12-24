#pragma once

#include <utility>
#include <cassert>
#include <iterator>
#include <new>
#include <memory>
#include <stdexcept>
#include <limits>

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T, bool Reverse>
	class StaticIterator
	{
	public:
		using iterator_concept	= std::contiguous_iterator_tag;
		using iterator_category = std::random_access_iterator_tag;
		using difference_type	= std::ptrdiff_t;
		using value_type		= std::remove_const_t<T>;
		using element_type		= T;
		using pointer			= T*;
		using reference			= T&;

		static constexpr int DIR = Reverse ? -1 : 1;

	public:
		StaticIterator() noexcept = default;
		explicit StaticIterator(pointer aPtr) noexcept : myPtr(aPtr) {}

		template<bool OtherReverse> // able to convert non-const to const, but not the other way around
		StaticIterator(const StaticIterator<value_type, OtherReverse>& aRight) noexcept : myPtr(aRight.myPtr) {};

		template<bool OtherReverse>
		StaticIterator& operator=(const StaticIterator<value_type, OtherReverse>& aRight) noexcept { myPtr = aRight.myPtr; };

	public:
		reference operator*() const noexcept { return *myPtr; }
		pointer operator->() const noexcept { return myPtr; }

		StaticIterator& operator++() noexcept { myPtr += DIR; return *this; }
		StaticIterator& operator--() noexcept { myPtr -= DIR; return *this; }

		StaticIterator operator++(int) noexcept { StaticIterator temp = *this; ++(*this); return temp; }
		StaticIterator operator--(int) noexcept { StaticIterator temp = *this; --(*this); return temp; }

		StaticIterator& operator+=(difference_type aOffset) noexcept { myPtr += aOffset; return *this; }
		StaticIterator& operator-=(difference_type aOffset) noexcept { return *this += -aOffset; }

		NODISC difference_type operator-(const StaticIterator& aRight) const noexcept { return (myPtr - aRight.myPtr) * DIR; }

		NODISC StaticIterator operator+(difference_type aOffset) const noexcept { return (StaticIterator(*this) += aOffset * DIR); }
		NODISC StaticIterator operator-(difference_type aOffset) const noexcept { return (StaticIterator(*this) += -aOffset * DIR); }

		reference operator[](difference_type aOffset) const noexcept { return *(*this + aOffset * DIR); }

		template<class U, bool OtherReverse>
		NODISC bool operator==(const StaticIterator<U, OtherReverse>& aRight) const noexcept { return myPtr == aRight.myPtr; }

		template<class U, bool OtherReverse>
		NODISC std::strong_ordering operator<=>(const StaticIterator<U, OtherReverse>& aRight) const noexcept
		{
			if constexpr (Reverse)
			{
				return 0 <=> (myPtr <=> aRight.myPtr);
			}
			else
			{
				return (myPtr <=> aRight.myPtr);
			}
		}

		NODISC friend StaticIterator operator+(difference_type aLeft, const StaticIterator& aRight)
		{
			return (aRight + aLeft);
		}

	private:
		pointer myPtr;

		template<class U, bool Reverse>
		friend class StaticIterator;
	};

	template<typename T, std::size_t Capacity>
	class StaticVector
	{
	public:
		using value_type		= T;
		using reference			= T&;
		using const_reference	= const T&;
		using pointer			= T*;
		using const_pointer		= const T*;
		using size_type			= std::size_t;
		using difference_type	= std::ptrdiff_t;

		using iterator					= StaticIterator<value_type, false>;
		using const_iterator			= StaticIterator<const value_type, false>;
		using reverse_iterator			= StaticIterator<value_type, true>;
		using const_reverse_iterator	= StaticIterator<const value_type, true>;

		static constexpr bool CopyConstructableAndCopyAssignable = std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>;

		static constexpr bool NoThrowCopyConstructableAndCopyAssignable = std::is_nothrow_constructible_v<T> && std::is_nothrow_copy_assignable_v<T>;

		static constexpr bool NoThrowMoveConstructor = std::is_nothrow_move_constructible_v<T> ||
			((!std::is_nothrow_move_constructible_v<T> || !std::is_move_constructible_v<T>) && std::is_nothrow_copy_constructible_v<T>);

		static constexpr bool MoveConstructableAndMoveAssignable = std::is_move_constructible_v<T> && std::is_move_assignable_v<T>;

		static constexpr bool NoThrowMoveConstructableAndMoveAssignable = std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<T>;

		static constexpr bool NoThrowMoveAssignment = (std::is_nothrow_destructible_v<T> && NoThrowMoveConstructableAndMoveAssignable) ||
			((!NoThrowMoveConstructableAndMoveAssignable || !MoveConstructableAndMoveAssignable) && NoThrowCopyConstructableAndCopyAssignable);

		constexpr StaticVector() noexcept;
		constexpr ~StaticVector() noexcept(std::is_nothrow_destructible_v<T>);

		constexpr StaticVector(std::size_t aSize) requires(std::is_default_constructible_v<T>);
		constexpr StaticVector(std::size_t aSize, const_reference aElement) requires(std::is_copy_constructible_v<T>);
		template<typename Iter> requires(std::forward_iterator<Iter> && std::constructible_from<T, typename Iter::value_type>)
		constexpr StaticVector(Iter aFirst, Iter aLast);
		constexpr StaticVector(std::initializer_list<T> aInitList);
		template<typename U> requires(std::constructible_from<T, U> && !std::is_same_v<T, U>)
		constexpr StaticVector(std::initializer_list<U> aInitList);

		constexpr StaticVector(const StaticVector& aOther) noexcept
			requires(std::is_trivially_constructible_v<T> && std::is_copy_constructible_v<T>);
		constexpr StaticVector(const StaticVector& aOther) noexcept(std::is_nothrow_copy_constructible_v<T>)
			requires(!std::is_trivially_constructible_v<T> && std::is_copy_constructible_v<T>);

		template<std::size_t OtherCapacity> requires(std::is_copy_constructible_v<T> && (Capacity != OtherCapacity))
		constexpr StaticVector(const StaticVector<T, OtherCapacity>& aOther) noexcept(std::is_nothrow_copy_constructible_v<T> && (OtherCapacity < Capacity));

		constexpr StaticVector(StaticVector&& aOther) noexcept
			requires(std::is_trivially_move_constructible_v<T> && std::is_move_constructible_v<T>);
		constexpr StaticVector(StaticVector&& aOther) noexcept(NoThrowMoveConstructor)
			requires(!std::is_trivially_move_constructible_v<T> && (std::is_move_constructible_v<T> || std::is_copy_constructible_v<T>));

		template<std::size_t OtherCapacity> requires((std::is_move_constructible_v<T> || std::is_copy_constructible_v<T>) && (Capacity != OtherCapacity))
		constexpr StaticVector(StaticVector<T, OtherCapacity>&& aOther) noexcept(NoThrowMoveConstructor && (Capacity > OtherCapacity));

		constexpr auto operator=(const StaticVector& aOther) noexcept -> StaticVector&
			requires(std::is_trivially_copyable_v<T> && std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>);
		constexpr auto operator=(const StaticVector& aOther) noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_assignable_v<T> && std::is_nothrow_destructible_v<T>) -> StaticVector&
			requires(!std::is_trivially_copyable_v<T> && std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>);

		template<std::size_t OtherCapacity> requires(std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T> && (Capacity != OtherCapacity))
		constexpr auto operator=(const StaticVector& aOther) noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_assignable_v<T> && std::is_nothrow_destructible_v<T> && (Capacity > OtherCapacity)) -> StaticVector&;

		constexpr auto operator=(StaticVector&& aOther) noexcept -> StaticVector&
			requires(std::is_trivially_move_assignable_v<T> && std::is_move_assignable_v<T> && std::is_trivially_move_constructible_v<T>);
		constexpr auto operator=(StaticVector&& aOther) noexcept(NoThrowMoveAssignment) -> StaticVector&
			requires(!std::is_trivially_move_assignable_v<T> && (CopyConstructableAndCopyAssignable || MoveConstructableAndMoveAssignable));

		template<std::size_t OtherCapacity> requires(((std::is_copy_constructible_v<T>&& std::is_copy_assignable_v<T>) || (std::is_move_constructible_v<T> && std::is_move_assignable_v<T>)) && (Capacity != OtherCapacity))
		constexpr auto operator=(StaticVector&& aOther) noexcept(NoThrowMoveAssignment && (Capacity > OtherCapacity)) -> StaticVector&;

		template<typename U> requires(std::constructible_from<T, U> && std::is_copy_assignable_v<T> && std::is_copy_constructible_v<T>)
		constexpr void assign(std::initializer_list<U> aInitList);
		constexpr void assign(std::size_t aCount, const_reference aElement);
		template<typename Iter> requires(std::forward_iterator<Iter> && std::is_convertible_v<typename Iter::value_type, T>)
		constexpr void assign(Iter aFirst, Iter aLast);

		NODISC constexpr auto operator[](size_type aIndex) -> reference;
		NODISC constexpr auto operator[](size_type aIndex) const -> const_reference;

		NODISC constexpr auto at(size_type aIndex) -> reference;
		NODISC constexpr auto at(size_type aIndex) const -> const_reference;

		NODISC constexpr auto front() -> reference;
		NODISC constexpr auto front() const -> const_reference;

		NODISC constexpr auto back() -> reference;
		NODISC constexpr auto back() const -> const_reference;

		NODISC constexpr auto data() noexcept -> pointer;
		NODISC constexpr auto data() const noexcept -> const_pointer;

		NODISC constexpr auto size() const noexcept -> size_type;
		NODISC constexpr bool empty() const noexcept;

		NODISC constexpr auto capacity() const noexcept -> size_type;
		NODISC constexpr auto free_space() const noexcept -> size_type;

		NODISC constexpr auto max_size() const noexcept -> size_type;

		constexpr void push_back(const T& aElement);
		constexpr void push_back(T&& aElement);

		template<typename... Args> requires(std::constructible_from<T, Args...>)
		constexpr auto emplace_back(Args&&... someArgs) -> reference;

		constexpr void pop_back();

		constexpr auto erase(const_iterator aPosition) noexcept(std::is_nothrow_move_assignable_v<T> && std::is_nothrow_destructible_v<T>)->iterator;
		constexpr auto erase(const_iterator aFirst, const_iterator aLast) noexcept(std::is_nothrow_move_assignable_v<T> && std::is_nothrow_destructible_v<T>)->iterator;

		template<typename... Args> requires(std::constructible_from<T, Args...>)
		constexpr auto emplace(const_iterator aPosition, Args&&... someArgs) -> iterator;

		constexpr auto insert(const_iterator aPosition, const_reference aElement) -> iterator;
		constexpr auto insert(const_iterator aPosition, T&& aElement) -> iterator;

		template<typename Iter> requires(std::forward_iterator<Iter> && std::constructible_from<T, typename Iter::value_type> && std::is_copy_assignable_v<T>)
		constexpr auto insert(const_iterator aPosition, Iter aFirst, Iter aLast) -> iterator;

		constexpr void resize(std::size_t aNewSize) requires(std::is_default_constructible_v<T>);
		constexpr void resize(std::size_t aNewSize, const_reference aElement) requires(std::is_copy_constructible_v<T>);

		constexpr void swap(StaticVector& aOther) noexcept(std::is_nothrow_swappable_v<T> && ((!std::is_move_constructible_v<T> && std::is_nothrow_copy_assignable_v<T>) || std::is_nothrow_move_constructible_v<T>))
			requires(std::is_swappable_v<T> && (std::is_copy_constructible_v<T> || std::is_move_constructible_v<T>));

		constexpr void clear() noexcept(std::is_nothrow_destructible_v<T>);

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
		NODISC constexpr T* ptr_at(size_type aIndex);
		NODISC constexpr const T* ptr_at(size_type aIndex) const;

		alignas(T) std::byte myData[sizeof(T) * Capacity]{};
		size_type mySize{0};
	};

	template<typename T, std::size_t Capacity>
	constexpr StaticVector<T, Capacity>::StaticVector() noexcept = default;

	template<typename T, std::size_t Capacity>
	constexpr StaticVector<T, Capacity>::~StaticVector() noexcept(std::is_nothrow_destructible_v<T>)
	{
		if constexpr (!std::is_trivially_destructible_v<T>)
		{
			std::destroy(begin(), end());
		}
	}

	template<typename T, std::size_t Capacity>
	constexpr StaticVector<T, Capacity>::StaticVector(std::size_t aSize) requires(std::is_default_constructible_v<T>)
		: myData(), mySize(aSize)
	{
		if (mySize > Capacity)
		{
			mySize = 0;
			throw std::runtime_error("Static vector does not have enough capacity for such size!");
		}

		std::uninitialized_value_construct(begin(), end());
	}

	template<typename T, std::size_t Capacity>
	constexpr StaticVector<T, Capacity>::StaticVector(std::size_t aSize, const_reference aElement) requires(std::is_copy_constructible_v<T>)
		: myData(), mySize(aSize)
	{
		if (mySize > Capacity)
		{
			mySize = 0;
			throw std::runtime_error("Static vector does not have enough capacity for such size!");
		}

		std::uninitialized_fill(begin(), end(), aElement);
	}

	template<typename T, std::size_t Capacity>
	template<typename Iter> requires(std::forward_iterator<Iter> && std::constructible_from<T, typename Iter::value_type>)
	constexpr StaticVector<T, Capacity>::StaticVector(Iter aFirst, Iter aLast)
		: myData(), mySize(std::distance(aFirst, aLast))
	{
		if (mySize > Capacity)
		{
			mySize = 0;
			throw std::runtime_error("Static vector does not have enough capacity for such size!");
		}

		std::uninitialized_copy(aFirst, aLast, begin());
	}

	template<typename T, std::size_t Capacity>
	constexpr StaticVector<T, Capacity>::StaticVector(std::initializer_list<T> aInitList)
		: StaticVector(const_iterator(aInitList.begin()), const_iterator(aInitList.end()))
	{

	}

	template<typename T, std::size_t Capacity>
	template<typename U> requires(std::constructible_from<T, U> && !std::is_same_v<T, U>)
	constexpr StaticVector<T, Capacity>::StaticVector(std::initializer_list<U> aInitList)
		: StaticVector(const_iterator(aInitList.begin()), const_iterator(aInitList.end()))
	{

	}

	template<typename T, std::size_t Capacity>
	constexpr StaticVector<T, Capacity>::StaticVector(const StaticVector& aOther) noexcept
		requires(std::is_trivially_constructible_v<T> && std::is_copy_constructible_v<T>) = default;

	template<typename T, std::size_t Capacity>
	constexpr StaticVector<T, Capacity>::StaticVector(const StaticVector& aOther) noexcept(std::is_nothrow_copy_constructible_v<T>)
		requires(!std::is_trivially_constructible_v<T> && std::is_copy_constructible_v<T>)
		: mySize(aOther.size())
	{
		std::uninitialized_copy(aOther.begin(), aOther.end(), begin());
	}

	template<typename T, std::size_t Capacity>
	template<std::size_t OtherCapacity> requires(std::is_copy_constructible_v<T> && (Capacity != OtherCapacity))
	constexpr StaticVector<T, Capacity>::StaticVector(const StaticVector<T, OtherCapacity>& aOther) noexcept(std::is_nothrow_copy_constructible_v<T> && (OtherCapacity < Capacity))
		: mySize(aOther.size())
	{
		if constexpr (OtherCapacity > Capacity)
		{
			if (aOther.size() > Capacity)
			{
				mySize = 0;
				throw std::runtime_error("Vector can't store that many elements!");
			}
		}

		std::uninitialized_copy(aOther.begin(), aOther.end(), begin());
	}

	template<typename T, std::size_t Capacity>
	constexpr StaticVector<T, Capacity>::StaticVector(StaticVector&& aOther) noexcept
		requires(std::is_trivially_move_constructible_v<T> && std::is_move_constructible_v<T>) = default;

	template<typename T, std::size_t Capacity>
	constexpr StaticVector<T, Capacity>::StaticVector(StaticVector&& aOther) noexcept(NoThrowMoveConstructor)
		requires(!std::is_trivially_move_constructible_v<T> && (std::is_move_constructible_v<T> || std::is_copy_constructible_v<T>))
	{
		if constexpr ((std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) && std::is_move_constructible_v<T>)
		{
			std::uninitialized_move(aOther.begin(), aOther.end(), begin());
		}
		else
		{
			std::uninitialized_copy(aOther.begin(), aOther.end(), begin());
		}

		aOther.clear();
	}

	template<typename T, std::size_t Capacity>
	template<std::size_t OtherCapacity> requires((std::is_move_constructible_v<T> || std::is_copy_constructible_v<T>) && (Capacity != OtherCapacity))
	constexpr StaticVector<T, Capacity>::StaticVector(StaticVector<T, OtherCapacity>&& aOther) noexcept(NoThrowMoveConstructor && (Capacity > OtherCapacity))
	{
		if constexpr (OtherCapacity > Capacity)
		{
			if (aOther.size() > Capacity)
			{
				mySize = 0;
				throw std::runtime_error("Vector can't store that many elements!");
			}
		}

		if constexpr ((std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) && std::is_move_constructible_v<T>)
		{
			std::uninitialized_move(aOther.begin(), aOther.end(), begin());
		}
		else
		{
			std::uninitialized_copy(aOther.begin(), aOther.end(), begin());
		}

		aOther.clear();
	}

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::operator=(const StaticVector& aOther) noexcept -> StaticVector&
		requires(std::is_trivially_copyable_v<T> && std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>) = default;

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::operator=(const StaticVector& aOther) noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_assignable_v<T> && std::is_nothrow_destructible_v<T>)->StaticVector&
		requires(!std::is_trivially_copyable_v<T> && std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>)
	{
		if (this == &aOther)
		{
			return *this;
		}

		if (mySize <= aOther.size())
		{
			std::copy(aOther.begin(), aOther.begin() + mySize, begin());
			std::uninitialized_copy(aOther.begin() + mySize, aOther.end(), end());
		}
		else
		{
			std::copy(aOther.begin(), aOther.end(), begin());
			if constexpr (!std::is_trivially_destructible_v<T>)
			{
				std::destroy(begin() + aOther.size(), end());
			}
		}

		mySize = aOther.size();

		return *this;
	}

	template<typename T, std::size_t Capacity>
	template<std::size_t OtherCapacity> requires(std::is_copy_constructible_v<T>&& std::is_copy_assignable_v<T> && (Capacity != OtherCapacity))
	inline constexpr auto StaticVector<T, Capacity>::operator=(const StaticVector& aOther) noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_assignable_v<T> && std::is_nothrow_destructible_v<T> && (Capacity > OtherCapacity)) -> StaticVector&
	{
		if (this == &aOther)
		{
			return *this;
		}

		if constexpr (OtherCapacity > Capacity)
		{
			if (aOther.size() > Capacity)
			{
				throw std::runtime_error("Vector can't store that many elements!");
			}
		}

		if constexpr (std::is_trivially_copyable_v<T>)
		{
			std::copy(aOther.begin(), aOther.end(), begin());
		}
		else
		{
			if (mySize <= aOther.size())
			{
				std::copy(aOther.begin(), aOther.begin() + mySize, begin());
				std::uninitialized_copy(aOther.begin() + mySize, aOther.end(), end());
			}
			else
			{
				std::copy(aOther.begin(), aOther.end(), begin());
				if constexpr (!std::is_trivially_destructible_v<T>)
				{
					std::destroy(begin() + aOther.size(), end());
				}
			}
		}

		mySize = aOther.size();

		return *this;
	}

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::operator=(StaticVector&& aOther) noexcept -> StaticVector&
		requires(std::is_trivially_move_assignable_v<T> && std::is_move_assignable_v<T> && std::is_trivially_move_constructible_v<T>) = default;

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::operator=(StaticVector&& aOther) noexcept(NoThrowMoveAssignment)->StaticVector&
		requires(!std::is_trivially_move_assignable_v<T> && (CopyConstructableAndCopyAssignable || MoveConstructableAndMoveAssignable))
	{
		if (this == &aOther)
		{
			return *this;
		}

		if constexpr (!MoveConstructableAndMoveAssignable || !NoThrowMoveConstructableAndMoveAssignable)
		{
			*this = aOther;
			aOther.clear();
			return *this;
		}
		else
		{
			if (mySize <= aOther.size())
			{
				std::copy(std::make_move_iterator(aOther.begin()), std::make_move_iterator(aOther.begin() + mySize), begin());
				std::uninitialized_move(aOther.begin() + mySize, aOther.end(), end());
			}
			else
			{
				std::copy(std::make_move_iterator(aOther.begin()), std::make_move_iterator(aOther.end()), begin());
				if constexpr (!std::is_trivially_destructible_v<T>)
				{
					std::destroy(begin() + aOther.size(), end());
				}
			}
		}

		mySize = aOther.size();
		aOther.clear();

		return *this;
	}

	template<typename T, std::size_t Capacity>
	template<std::size_t OtherCapacity> requires(((std::is_copy_constructible_v<T>&& std::is_copy_assignable_v<T>) || (std::is_move_constructible_v<T> && std::is_move_assignable_v<T>)) && (Capacity != OtherCapacity))
	inline constexpr auto StaticVector<T, Capacity>::operator=(StaticVector&& aOther) noexcept(NoThrowMoveAssignment && (Capacity > OtherCapacity)) -> StaticVector&
	{
		if (this == &aOther)
		{
			return *this;
		}

		if constexpr (!MoveConstructableAndMoveAssignable || !NoThrowMoveConstructableAndMoveAssignable)
		{
			*this = aOther;
			aOther.clear();
			return *this;
		}
		else
		{
			if constexpr (OtherCapacity > Capacity)
			{
				if (aOther.size() > Capacity)
				{
					throw std::runtime_error("Vector can't store that many elements!");
				}
			}

			if constexpr (std::is_trivially_move_assignable_v<T>)
			{
				std::copy(std::make_move_iterator(aOther.begin()), std::make_move_iterator(aOther.end()), begin());
			}
			else
			{
				if (mySize <= aOther.size())
				{
					std::copy(std::make_move_iterator(aOther.begin()), std::make_move_iterator(aOther.begin() + mySize), begin());
					std::uninitialized_move(aOther.begin() + mySize, aOther.end(), end());
				}
				else
				{
					std::copy(std::make_move_iterator(aOther.begin()), std::make_move_iterator(aOther.end()), begin());
					if constexpr (!std::is_trivially_destructible_v<T>)
					{
						std::destroy(begin() + aOther.size(), end());
					}
				}
			}
		}

		mySize = aOther.size();
		aOther.clear();

		return *this;
	}

	template<typename T, std::size_t Capacity>
	template<typename U> requires(std::constructible_from<T, U> && std::is_copy_assignable_v<T> && std::is_copy_constructible_v<T>)
	constexpr void StaticVector<T, Capacity>::assign(std::initializer_list<U> aInitList)
	{
		assign(aInitList.begin(), aInitList.end());
	}

	template<typename T, std::size_t Capacity>
	constexpr void StaticVector<T, Capacity>::assign(std::size_t aCount, const_reference aElement)
	{
		if (aCount > Capacity)
		{
			throw std::runtime_error("Vector can't store that many elements!");
		}

		if constexpr (std::is_trivially_copyable_v<T>)
		{
			std::fill(begin(), begin() + aCount, aElement);
		}
		else
		{
			if (mySize <= aCount)
			{
				std::fill(begin(), begin() + mySize, aElement);
				std::uninitialized_fill(end(), end() + aCount, aElement);
			}
			else
			{
				std::fill(begin(), begin() + aCount, aElement);
				if constexpr (!std::is_trivially_destructible_v<T>)
				{
					std::destroy(begin() + aCount, end());
				}
			}
		}

		mySize = aCount;
	}

	template<typename T, std::size_t Capacity>
	template<typename Iter> requires(std::forward_iterator<Iter> && std::is_convertible_v<typename Iter::value_type, T>)
	constexpr void StaticVector<T, Capacity>::assign(Iter aFirst, Iter aLast)
	{
		const auto newSize = std::distance(aFirst, aLast);

		if (newSize > Capacity)
		{
			throw std::runtime_error("Vector can't store that many elements!");
		}

		if constexpr (std::is_trivially_copyable_v<T>)
		{
			std::copy(aFirst, aLast, begin());
		}
		else
		{
			if (mySize <= newSize)
			{
				std::copy(aFirst, aFirst + mySize, begin());
				std::uninitialized_copy(aFirst + mySize, aLast, begin() + mySize);
			}
			else
			{
				std::copy(aFirst, aLast, begin());
				if constexpr (!std::is_trivially_default_constructible_v<T>)
				{
					std::destroy(begin() + newSize, end());
				}
			}
		}

		mySize = newSize;
	}

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::operator[](size_type aIndex) -> reference
	{
		return *ptr_at(aIndex);
	}
	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::operator[](size_type aIndex) const -> const_reference
	{
		return *ptr_at(aIndex);
	}

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::at(size_type aIndex) -> reference
	{
		if (aIndex >= mySize)
		{
			throw std::out_of_range("Index is out of bounds!");
		}

		return *ptr_at(aIndex);
	}
	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::at(size_type aIndex) const -> const_reference
	{
		if (aIndex >= mySize)
		{
			throw std::out_of_range("Index is out of bounds!");
		}

		return *ptr_at(aIndex);
	}

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::front() -> reference
	{
		return *ptr_at(0);
	}
	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::front() const -> const_reference
	{
		return *ptr_at(0);
	}

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::back() -> reference
	{
		return *ptr_at(mySize - 1);
	}
	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::back() const -> const_reference
	{
		return *ptr_at(mySize - 1);
	}

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::data() noexcept -> pointer
	{
		return ptr_at(0);
	}
	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::data() const noexcept -> const_pointer
	{
		return ptr_at(0);
	}

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::size() const noexcept -> size_type
	{
		return mySize;
	}
	template<typename T, std::size_t Capacity>
	constexpr bool StaticVector<T, Capacity>::empty() const noexcept
	{
		return mySize == 0;
	}

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::capacity() const noexcept -> size_type
	{
		return Capacity;
	}
	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::free_space() const noexcept -> size_type
	{
		return Capacity - mySize;
	}

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::max_size() const noexcept -> size_type
	{
		// if you see error here, add #define NOMINMAX before including <cmath>
		return std::numeric_limits<size_type>::max();
	}

	template<typename T, std::size_t Capacity>
	constexpr void StaticVector<T, Capacity>::push_back(const T& aElement)
	{
		emplace_back(aElement);
	}
	template<typename T, std::size_t Capacity>
	constexpr void StaticVector<T, Capacity>::push_back(T&& aElement)
	{
		emplace_back(std::move(aElement));
	}

	template<typename T, std::size_t Capacity>
	template<typename... Args> requires(std::constructible_from<T, Args...>)
	constexpr auto StaticVector<T, Capacity>::emplace_back(Args&&... someArgs) -> reference
	{
		if (mySize == Capacity)
		{
			throw std::runtime_error("Vector has reached capacity, no more elements are allowed!");
		}

		std::construct_at(ptr_at(mySize), std::forward<Args>(someArgs)...);
		++mySize;

		return *end();
	}

	template<typename T, std::size_t Capacity>
	constexpr void StaticVector<T, Capacity>::pop_back()
	{
		if (empty())
		{
			throw std::runtime_error("Vector is empty, nothing to pop");
		}

		if constexpr (!std::is_trivially_destructible_v<T>)
		{
			std::destroy_at(ptr_at(mySize - 1));
		}

		--mySize;
	}

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::erase(const_iterator aPosition) noexcept(std::is_nothrow_move_assignable_v<T> && std::is_nothrow_destructible_v<T>) -> iterator
	{
		const auto eraseIter = begin() + std::distance(cbegin(), aPosition);

		if (eraseIter == end())
		{
			pop_back();
		}
		else
		{
			for (auto it = eraseIter; it != (end() - 1); ++it)
			{
				*it = std::move(*(it + 1));
			}

			std::destroy_at(std::to_address(end() - 1));

			--mySize;
		}

		return eraseIter;
	}

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::erase(const_iterator aFirst, const_iterator aLast) noexcept(std::is_nothrow_move_assignable_v<T> && std::is_nothrow_destructible_v<T>) -> iterator
	{
		const auto eraseIterFirst = begin() + std::distance(cbegin(), aFirst);
		const auto eraseIterLast = begin() + std::distance(cbegin(), aLast);

		auto it1 = eraseIterFirst;
		auto it2 = eraseIterLast;

		while (it2 != end())
		{
			*it1 = std::move(*it2);

			++it1;
			++it2;
		}

		std::destroy(it1, end());

		mySize = std::distance(begin(), it1);

		return eraseIterFirst;
	}

	template<typename T, std::size_t Capacity>
	template<typename... Args> requires(std::constructible_from<T, Args...>)
	constexpr auto StaticVector<T, Capacity>::emplace(const_iterator aPosition, Args&&... someArgs) -> iterator
	{
		if (mySize == Capacity)
		{
			throw std::runtime_error("Vector has reached capacity, no more elements are allowed!");
		}

		const auto insertIter = begin() + std::distance(cbegin(), aPosition);

		if (insertIter == end())
		{
			emplace_back(std::forward<Args>(someArgs)...);
		}
		else
		{
			std::construct_at(std::to_address(end()), std::move(*(end() - 1)));

			std::move_backward(insertIter, end() - 1, end());

			*insertIter = T{ std::forward<Args>(someArgs)... };

			++mySize;
		}

		return insertIter;
	}

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::insert(const_iterator aPosition, const_reference aElement) -> iterator
	{
		return emplace(aPosition, aElement);
	}
	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::insert(const_iterator aPosition, T&& aElement) -> iterator
	{
		return emplace(aPosition, std::move(aElement));
	}

	template<typename T, std::size_t Capacity>
	template<typename Iter> requires(std::forward_iterator<Iter> && std::constructible_from<T, typename Iter::value_type> && std::is_copy_assignable_v<T>)
	constexpr auto StaticVector<T, Capacity>::insert(const_iterator aPosition, Iter aFirst, Iter aLast) -> iterator
	{
		const auto count = std::distance(aFirst, aLast);
		const auto newSize = mySize + count;

		if (newSize > Capacity)
		{
			throw std::runtime_error("Vector can't store that many elements!");
		}

		const auto insertIter = begin() + std::distance(cbegin(), aPosition);

		if (insertIter == end())
		{
			std::uninitialized_copy(aFirst, aLast, end());
		}
		else
		{
			std::uninitialized_move(end() - count, end(), end());

			std::move_backward(insertIter, end() - count, end());

			std::copy(aFirst, aLast, insertIter);
		}

		mySize = newSize;

		return insertIter;
	}

	template<typename T, std::size_t Capacity>
	constexpr void StaticVector<T, Capacity>::resize(std::size_t aNewSize)
		requires(std::is_default_constructible_v<T>)
	{
		if (aNewSize > Capacity)
		{
			throw std::runtime_error("Vector can't store that many elements!");
		}

		if (aNewSize > mySize)
		{
			std::uninitialized_value_construct(end(), begin() + aNewSize);
		}
		else
		{
			std::destroy(begin() + aNewSize, end());
		}

		mySize = aNewSize;
	}
	template<typename T, std::size_t Capacity>
	constexpr void StaticVector<T, Capacity>::resize(std::size_t aNewSize, const_reference aElement)
		requires(std::is_copy_constructible_v<T>)
	{
		if (aNewSize > Capacity)
		{
			throw std::runtime_error("Vector can't store that many elements!");
		}

		if (aNewSize > mySize)
		{
			std::uninitialized_fill(end(), begin() + aNewSize, aElement);
		}
		else
		{
			std::destroy(begin() + aNewSize, end());
		}

		mySize = aNewSize;
	}

	template<typename T, std::size_t Capacity>
	constexpr void StaticVector<T, Capacity>::swap(StaticVector& aOther) noexcept(std::is_nothrow_swappable_v<T> && ((!std::is_move_constructible_v<T> && std::is_nothrow_copy_assignable_v<T>) || std::is_nothrow_move_constructible_v<T>))
		requires(std::is_swappable_v<T> && (std::is_copy_constructible_v<T> || std::is_move_constructible_v<T>))
	{
		if (this == &aOther)
		{
			return;
		}

		auto leftIt = begin();
		auto rightIt = aOther.begin();

		for (; leftIt != end() && rightIt != aOther.end(); ++leftIt, ++rightIt)
		{
			std::swap(*leftIt, *rightIt);
		}

		if (leftIt != end())
		{
			if constexpr (std::is_move_constructible_v<T>)
			{
				std::uninitialized_move(leftIt, end(), aOther.end());
			}
			else
			{
				std::uninitialized_copy(leftIt, end(), aOther.end());
			}

			if constexpr (!std::is_trivially_destructible_v<T>)
			{
				std::destroy(leftIt, end());
			}
		}

		if (rightIt != aOther.end())
		{
			if constexpr (std::is_move_constructible_v<T>)
			{
				std::uninitialized_move(rightIt, aOther.end(), end());
			}
			else
			{
				std::uninitialized_copy(rightIt, aOther.end(), end());
			}

			if constexpr (!std::is_trivially_destructible_v<T>)
			{
				std::destroy(rightIt, aOther.end());
			}
		}

		std::swap(mySize, aOther.mySize);
	}

	template<typename T, std::size_t Capacity>
	constexpr void StaticVector<T, Capacity>::clear() noexcept(std::is_nothrow_destructible_v<T>)
	{
		if constexpr (std::is_trivially_destructible_v<T>)
		{
			mySize = 0;
		}
		else
		{
			if constexpr (std::is_nothrow_destructible_v<T>)
			{
				std::destroy(begin(), end());
				mySize = 0;
			}
			else
			{
				while (!empty())
				{
					pop_back();
				}
			}
		}
	}

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::begin() noexcept -> iterator
	{
		return iterator(ptr_at(0));
	}
	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::end() noexcept -> iterator
	{
		return iterator(ptr_at(mySize));
	}

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::begin() const noexcept -> const_iterator
	{
		return const_iterator(ptr_at(0));
	}
	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::end() const noexcept -> const_iterator
	{
		return const_iterator(ptr_at(mySize));
	}

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::cbegin() const noexcept -> const_iterator
	{
		return begin();
	}
	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::cend() const noexcept -> const_iterator
	{
		return end();
	}

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::rbegin() noexcept -> reverse_iterator
	{
		return reverse_iterator(ptr_at(mySize - 1));
	}
	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::rend() noexcept -> reverse_iterator
	{
		return reverse_iterator(ptr_at(-1));
	}

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::rbegin() const noexcept -> const_reverse_iterator
	{
		return const_reverse_iterator(ptr_at(mySize - 1));
	}
	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::rend() const noexcept -> const_reverse_iterator
	{
		return const_reverse_iterator(ptr_at(-1));
	}

	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::crbegin() const noexcept -> const_reverse_iterator
	{
		return rbegin();
	}
	template<typename T, std::size_t Capacity>
	constexpr auto StaticVector<T, Capacity>::crend() const noexcept -> const_reverse_iterator
	{
		return rend();
	}

	template<typename T, std::size_t Capacity>
	constexpr T* StaticVector<T, Capacity>::ptr_at(size_type aIndex)
	{
		return std::launder(reinterpret_cast<T*>(&myData[0] + sizeof(T) * aIndex));
	}

	template<typename T, std::size_t Capacity>
	constexpr const T* StaticVector<T, Capacity>::ptr_at(size_type aIndex) const
	{
		return std::launder(reinterpret_cast<const T*>(&myData[0] + sizeof(T) * aIndex));
	}

	// GLOBAL FUNCTIONS

	template<typename T, std::size_t Capacity> requires(std::is_swappable_v<T> && (std::is_copy_constructible_v<T> || std::is_move_constructible_v<T>))
	constexpr void swap(StaticVector<T, Capacity>& aLeft, StaticVector<T, Capacity>& aRight)
	{
		aLeft.swap(aRight);
	}

	template<typename T, std::size_t LeftCapacity, std::size_t RightCapacity> requires(std::equality_comparable<T>)
	constexpr bool operator==(const StaticVector<T, LeftCapacity>& aLeft, const StaticVector<T, RightCapacity>& aRight) noexcept
	{
		return std::equal(aLeft.begin(), aLeft.end(), aRight.begin(), aRight.end());
	}
}
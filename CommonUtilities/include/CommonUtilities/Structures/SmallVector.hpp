#pragma once

#include <array>
#include <vector>
#include <iterator>

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T, bool Reverse>
	class SmallIterator
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

		SmallIterator() noexcept = default;
		explicit SmallIterator(pointer aPtr) noexcept : myPtr(aPtr) {}

		template<bool OtherReverse> // able to convert non-const to const, but not the other way around
		SmallIterator(const SmallIterator<value_type, OtherReverse>& aRhs) noexcept : myPtr(aRhs.myPtr) {};

		template<bool OtherReverse>
		SmallIterator& operator=(const SmallIterator<value_type, OtherReverse>& aRhs) noexcept { myPtr = aRhs.myPtr; };

		reference operator*() const noexcept	{ return *myPtr; }
		pointer operator->() const noexcept		{ return myPtr; }

		SmallIterator& operator++() noexcept	{ myPtr += DIR; return *this; }
		SmallIterator& operator--() noexcept	{ myPtr -= DIR; return *this; }

		SmallIterator operator++(int) noexcept	{ SmallIterator temp = *this; ++(*this); return temp; }
		SmallIterator operator--(int) noexcept	{ SmallIterator temp = *this; --(*this); return temp; }

		SmallIterator& operator+=(difference_type aOffset) noexcept { myPtr += aOffset; return *this; }
		SmallIterator& operator-=(difference_type aOffset) noexcept { return *this += -aOffset; }

		NODISC difference_type operator-(const SmallIterator& aRhs) const noexcept { return (myPtr - aRhs.myPtr) * DIR; }

		NODISC SmallIterator operator+(difference_type aOffset) const noexcept { return (SmallIterator(*this) += aOffset * DIR); }
		NODISC SmallIterator operator-(difference_type aOffset) const noexcept { return (SmallIterator(*this) += -aOffset * DIR); }

		reference operator[](difference_type aOffset) const noexcept { return *(*this + aOffset * DIR); }

		template<class U, bool OtherReverse>
		NODISC bool operator==(const SmallIterator<U, OtherReverse>& aRhs) const noexcept { return myPtr == aRhs.myPtr; }

		template<class U, bool OtherReverse>
		NODISC std::strong_ordering operator<=>(const SmallIterator<U, OtherReverse>& aRhs) const noexcept
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

		NODISC friend SmallIterator operator+(difference_type aLhs, const SmallIterator& aRhs)
		{
			return (aRhs + aLhs);
		}

	private:
		pointer myPtr;

		template<class U, bool Reverse>
		friend class SmallIterator;
	};

	template<typename T, std::size_t N = 32, class Alloc = std::allocator<T>>
	class SmallVector
	{
	public:
		static_assert(!std::is_same_v<T, bool>, "Small vector is not compatible with bool because of std::vector<bool> specialization");

		using value_type		= T;
		using reference			= T&;
		using const_reference	= const T&;
		using pointer			= T*;
		using const_pointer		= const T*;
		using allocator_type	= Alloc;
		using size_type			= std::size_t;
		using difference_type	= std::ptrdiff_t;

		using iterator					= SmallIterator<value_type, false>;
		using const_iterator			= SmallIterator<const value_type, false>;
		using reverse_iterator			= SmallIterator<value_type, true>;
		using const_reverse_iterator	= SmallIterator<const value_type, true>;

		constexpr SmallVector() = default;
		constexpr ~SmallVector() = default;

		constexpr SmallVector(const allocator_type& aAlloc);
		constexpr SmallVector(size_type aSize, const allocator_type& aAlloc = allocator_type());
		constexpr SmallVector(size_type aSize, const_reference aValue, const allocator_type& aAlloc = allocator_type());
		template<class Iter> requires (std::forward_iterator<Iter> && std::constructible_from<T, typename Iter::value_type>)
		constexpr SmallVector(Iter aFirst, Iter aLast, const allocator_type& aAlloc = allocator_type());
		constexpr SmallVector(std::initializer_list<T> aInitList, const allocator_type& aAlloc = allocator_type());

		constexpr SmallVector(const SmallVector& aOther, const allocator_type& aAlloc = allocator_type());
		constexpr SmallVector(SmallVector&& aOther, const allocator_type& aAlloc = allocator_type()) noexcept;

		constexpr auto operator=(const SmallVector& aRhs) -> SmallVector&;
		constexpr auto operator=(SmallVector&& aRhs) noexcept -> SmallVector&;
		constexpr auto operator=(std::initializer_list<T> aInitList) -> SmallVector&;

		NODISC constexpr bool operator==(const SmallVector& aRhs) const;

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

		NODISC constexpr bool empty() const noexcept;
		NODISC constexpr auto size() const noexcept -> size_type;

		NODISC constexpr auto max_size() const noexcept -> size_type;

		NODISC constexpr auto get_allocator() const noexcept -> allocator_type;

		template<typename... Args>
		constexpr auto emplace_back(Args&&... someArgs) -> reference;

		constexpr void push_back(const T& aElement);
		constexpr void push_back(T&& aElement);

		constexpr void pop_back();

		constexpr auto erase(const_iterator aIterator) -> iterator;
		constexpr auto erase(const_iterator aFirst, const_iterator aLast) -> iterator;

		template<typename... Args> requires (std::constructible_from<T, Args...>)
		constexpr auto emplace(const_iterator aIterator, Args&&... someArgs) -> iterator;

		constexpr auto insert(const_iterator aIterator, const T& aElement) -> iterator;
		constexpr auto insert(const_iterator aIterator, T&& aElement) -> iterator;

		constexpr void fill(const_reference aElement);
		constexpr void resize(size_type aSize, const_reference aValue = T());

		constexpr void shrink_to_fit();

		constexpr void clear();

		constexpr friend void swap(SmallVector& aLhs, SmallVector& aRhs);

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
		using signed_type = std::make_signed_t<size_type>;

		NODISC constexpr __forceinline bool IsStack() const noexcept;
		NODISC constexpr __forceinline bool IsHeap() const noexcept;

		std::array<T, N>		myStack	{};
		std::vector<T, Alloc>	myHeap	{};
		size_type				mySize	{0};
	};

	template<typename T, std::size_t N, class Alloc>
	constexpr SmallVector<T, N, Alloc>::SmallVector(const allocator_type& aAlloc)
		: myStack(), myHeap(aAlloc), mySize()
	{

	}

	template<typename T, std::size_t N, class Alloc>
	constexpr SmallVector<T, N, Alloc>::SmallVector(size_type aSize, const allocator_type& aAlloc)
		: myStack(), myHeap(aAlloc), mySize(aSize)
	{
		if (IsHeap())
		{
			myHeap = std::vector<T>(mySize, aAlloc);
		}
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr SmallVector<T, N, Alloc>::SmallVector(size_type aSize, const_reference aValue, const allocator_type& aAlloc)
		: myStack(), myHeap(aAlloc), mySize(aSize)
	{
		if (IsStack())
		{
			std::fill(myStack.begin(), myStack.begin() + mySize, aValue);
		}
		else
		{
			myHeap = std::vector<T>(mySize, aValue, aAlloc);
		}
	}

	template<typename T, std::size_t N, class Alloc>
	template<class Iter> requires (std::forward_iterator<Iter> && std::constructible_from<T, typename Iter::value_type>)
	constexpr SmallVector<T, N, Alloc>::SmallVector(Iter aFirst, Iter aLast, const allocator_type& aAlloc)
		: myStack(), myHeap(), mySize(std::distance(aFirst, aLast))
	{
		if (IsStack())
		{
			std::copy(aFirst, aLast, myStack.begin());
		}
		else
		{
			myHeap = std::vector<T>(aFirst, aLast, aAlloc);
		}
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr SmallVector<T, N, Alloc>::SmallVector(std::initializer_list<T> aInitList, const allocator_type& aAlloc)
		: SmallVector(const_iterator(aInitList.begin()), const_iterator(aInitList.end()), aAlloc)
	{

	}

	template<typename T, std::size_t N, class Alloc>
	constexpr SmallVector<T, N, Alloc>::SmallVector(const SmallVector& aOther, const allocator_type& aAlloc)
		: SmallVector(aOther.begin(), aOther.end(), aAlloc)
	{

	}

	template<typename T, std::size_t N, class Alloc>
	constexpr SmallVector<T, N, Alloc>::SmallVector(SmallVector&& aOther, const allocator_type& aAlloc) noexcept
		: myStack(), myHeap(aAlloc), mySize(aOther.mySize) 
	{
		if (IsStack())
		{
			std::move(aOther.myStack.begin(), aOther.myStack.begin() + mySize, myStack.begin());
		}
		else
		{
			myHeap = std::move(aOther.myHeap);
		}
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::operator=(const SmallVector& aRhs) -> SmallVector&
	{
		if (aRhs.mySize <= N)
		{
			std::copy(aRhs.myStack.begin(), aRhs.myStack.begin() + aRhs.mySize, myStack.begin());
		}
		else
		{
			myHeap = aRhs.myHeap;
		}

		mySize = aRhs.mySize;

		return *this;
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::operator=(SmallVector&& aRhs) noexcept -> SmallVector&
	{
		if (aRhs.mySize <= N)
		{
			std::move(aRhs.myStack.begin(), aRhs.myStack.begin() + aRhs.mySize, myStack.begin());
		}
		else
		{
			myHeap = std::move(aRhs.myHeap);
		}

		mySize = aRhs.mySize;
		aRhs.mySize = 0;

		return *this;
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::operator=(std::initializer_list<T> aInitList) -> SmallVector&
	{
		if (aInitList.size() <= N)
		{
			std::copy(aInitList.begin(), aInitList.end(), myStack.begin());
		}
		else
		{
			myHeap = aInitList;
		}

		mySize = aInitList.size();
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr bool SmallVector<T, N, Alloc>::operator==(const SmallVector& aRhs) const
	{
		if (mySize != aRhs.mySize)
			return false;

		if (IsStack())
		{
			for (size_type index = 0; index < mySize; ++index)
			{
				if (myStack[index] != aRhs[index])
					return false;
			}

			return true;
		}
		
		return (myHeap == aRhs.myHeap);
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::operator[](size_type aIndex) -> reference
	{
		return (IsStack()) ? myStack[aIndex] : myHeap[aIndex];
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::operator[](size_type aIndex) const -> const_reference
	{
		return (IsStack()) ? myStack[aIndex] : myHeap[aIndex];
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::at(size_type aIndex) -> reference
	{
		return (IsStack()) ? myStack.at(aIndex) : myHeap.at(aIndex);
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::at(size_type aIndex) const -> const_reference
	{
		return (IsStack()) ? myStack.at(aIndex) : myHeap.at(aIndex);
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::front() -> reference
	{
		return (IsStack()) ? myStack.front() : myHeap.front();
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::front() const -> const_reference
	{
		return (IsStack()) ? myStack.front() : myHeap.front();
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::back() -> reference
	{
		return (IsStack()) ? myStack[mySize - 1] : myHeap.back();
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::back() const -> const_reference
	{
		return (IsStack()) ? myStack[mySize - 1] : myHeap.back();
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::data() noexcept -> pointer
	{
		return (IsStack()) ? myStack.data() : myHeap.data();
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::data() const noexcept -> const_pointer
	{
		return (IsStack()) ? myStack.data() : myHeap.data();
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr bool SmallVector<T, N, Alloc>::empty() const noexcept
	{
		return mySize == 0;
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::size() const noexcept -> size_type
	{
		return mySize;
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::max_size() const noexcept -> size_type
	{
		return (IsStack()) ? myStack.max_size() : myHeap.max_size();
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::get_allocator() const noexcept -> allocator_type
	{
		return myHeap.get_allocator();
	}

	template<typename T, std::size_t N, class Alloc>
	template<typename... Args>
	constexpr auto SmallVector<T, N, Alloc>::emplace_back(Args&&... someArgs) -> reference
	{
		if (mySize < N)
		{
			myStack[mySize] = T(std::forward<Args>(someArgs)...);
		}
		else
		{
			if (mySize == N)
				std::move(myStack.begin(), myStack.end(), std::back_inserter(myHeap));

			myHeap.emplace_back(std::forward<Args>(someArgs)...);
		}

		++mySize;

		return back();
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr void SmallVector<T, N, Alloc>::push_back(const T& aElement)
	{
		emplace_back(aElement);
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr void SmallVector<T, N, Alloc>::push_back(T&& aElement)
	{
		emplace_back(std::move(aElement));
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr void SmallVector<T, N, Alloc>::pop_back()
	{
		if (empty())
			return;

		if (IsStack())
		{
			--mySize;
		}
		else
		{
			myHeap.pop_back();
			--mySize;

			if (mySize == N)
			{
				std::move(myHeap.begin(), myHeap.end(), myStack.begin());
				myHeap.clear();
			}
		}
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::erase(const_iterator aPosition) -> iterator
	{
		if (empty()) // nothing to erase
			return end();

		const auto posIndex = std::distance(cbegin(), aPosition);

		if (IsStack())
		{
			for (auto index = static_cast<size_type>(posIndex); index < mySize - 1; ++index)
				myStack[index] = std::move(myStack[index + 1]);

			--mySize;
		}
		else
		{
			myHeap.erase(myHeap.begin() + posIndex);
			--mySize;

			if (mySize == N)
			{
				std::move(myHeap.begin(), myHeap.end(), myStack.begin());
				myHeap.clear();
			}
		}

		return begin() + posIndex;
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::erase(const_iterator aFirst, const_iterator aLast) -> iterator
	{
		assert(aFirst < aLast && "Invalid iterators provided");

		if (empty()) // nothing to erase
			return end();

		const auto firstPos = std::distance(cbegin(), aFirst);
		const auto lastPos	= std::distance(cbegin(), aLast);
		const auto count	= lastPos - firstPos;

		if (IsStack())
		{
			for (auto index = static_cast<size_type>(firstPos); index < mySize - count; ++index)
				myStack[index] = std::move(myStack[index + count]);

			mySize -= count;
		}
		else
		{
			myHeap.erase(myHeap.begin() + firstPos, myHeap.begin() + lastPos);
			mySize -= count;

			if (IsStack())
			{
				std::move(myHeap.begin(), myHeap.end(), myStack.begin());
				myHeap.clear();
			}
		}

		return begin() + firstPos;
	}

	template<typename T, std::size_t N, class Alloc>
	template<typename... Args> requires (std::constructible_from<T, Args...>)
	constexpr auto SmallVector<T, N, Alloc>::emplace(const_iterator aPosition, Args&&... someArgs) -> iterator
	{
		const auto posIndex = std::distance(cbegin(), aPosition);

		if (mySize < N)
		{
			if (posIndex < static_cast<decltype(posIndex)>(mySize))
			{
				std::move_backward(myStack.begin() + posIndex, myStack.begin() + mySize - 1, myStack.begin() + mySize);
			}

			myStack[posIndex] = T{ std::forward<Args>(someArgs)... };
		}
		else
		{
			if (mySize == N)
				std::move(myStack.begin(), myStack.end(), std::back_inserter(myHeap));

			myHeap.emplace(myHeap.begin() + posIndex, std::forward<Args>(someArgs)...);
		}

		++mySize;

		return iterator(data() + posIndex);
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::insert(const_iterator aPosition, const T& aElement) -> iterator
	{
		return emplace(aPosition, aElement);
	}
	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::insert(const_iterator aPosition, T&& aElement) -> iterator
	{
		return emplace(aPosition, std::move(aElement));
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr void SmallVector<T, N, Alloc>::fill(const_reference aValue)
	{
		std::fill(begin(), end(), aValue);
	}
	template<typename T, std::size_t N, class Alloc>
	constexpr void SmallVector<T, N, Alloc>::resize(size_type aSize, const_reference aValue)
	{
		if (aSize <= N)
		{
			if (IsHeap())
			{
				std::move(myHeap.begin(), myHeap.begin() + aSize, myStack.begin());
				myHeap.clear();
			}

			if (aSize > mySize)
				std::fill(myStack.begin() + mySize, myStack.begin() + aSize, aValue);
		}
		else
		{
			if (IsStack())
				std::move(myStack.begin(), myStack.end(), std::back_inserter(myHeap));

			myHeap.resize(aSize, aValue);
		}
		
		mySize = aSize;
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr void SmallVector<T, N, Alloc>::shrink_to_fit()
	{
		if (IsHeap())
			myHeap.shrink_to_fit();
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr void SmallVector<T, N, Alloc>::clear()
	{
		if (IsHeap())
			myHeap.clear();

		mySize = 0;
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr void swap(SmallVector<T, N, Alloc>& aLhs, SmallVector<T, N, Alloc>& aRhs)
	{
		std::swap(aLhs, aRhs);
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::begin() noexcept -> iterator
	{
		return iterator(IsStack() ? myStack.data() : myHeap.data());
	}
	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::end() noexcept -> iterator
	{
		return iterator(IsStack() ? (myStack.data() + mySize) : (myHeap.data() + mySize));
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::begin() const noexcept -> const_iterator
	{
		return const_iterator(IsStack() ? myStack.data() : myHeap.data());
	}
	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::end() const noexcept -> const_iterator
	{
		return const_iterator(IsStack() ? (myStack.data() + mySize) : (myHeap.data() + mySize));
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::cbegin() const noexcept -> const_iterator
	{
		return begin();
	}
	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::cend() const noexcept -> const_iterator
	{
		return end();
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::rbegin() noexcept -> reverse_iterator
	{
		return reverse_iterator(IsStack() ? (myStack.data() + mySize - 1) : (myHeap.data() + mySize - 1));
	}
	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::rend() noexcept -> reverse_iterator
	{
		return reverse_iterator(IsStack() ? (myStack.data() - 1) : (myHeap.data() - 1));
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::rbegin() const noexcept -> const_reverse_iterator
	{
		return const_reverse_iterator(IsStack() ? (myStack.data() + mySize - 1) : (myHeap.data() + mySize - 1));
	}
	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::rend() const noexcept -> const_reverse_iterator
	{
		return const_reverse_iterator(IsStack() ? (myStack.data() - 1) : (myHeap.data() - 1));
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::crbegin() const noexcept -> const_reverse_iterator
	{
		return rbegin();
	}
	template<typename T, std::size_t N, class Alloc>
	constexpr auto SmallVector<T, N, Alloc>::crend() const noexcept -> const_reverse_iterator
	{
		return rend();
	}

	template<typename T, std::size_t N, class Alloc>
	constexpr bool SmallVector<T, N, Alloc>::IsStack() const noexcept
	{
		return mySize <= N;
	}
	template<typename T, std::size_t N, class Alloc>
	constexpr bool SmallVector<T, N, Alloc>::IsHeap() const noexcept
	{
		return mySize > N;
	}
}
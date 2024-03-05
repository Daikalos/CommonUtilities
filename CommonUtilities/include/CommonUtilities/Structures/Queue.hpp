#pragma once

#include <array>
#include <cassert>

namespace CommonUtilities
{
	template<typename T, std::size_t N = 1000>
	class Queue
	{
	public:
		Queue();

		int GetSize() const;

		const T& GetFront() const;
		T& GetFront();

		void Enqueue(const T& aValue);

		T Dequeue();

	private:
		std::array<T, N>	myContainer;
		std::size_t			myFirst;
		std::size_t			myEnd;
	};

	template<typename T, std::size_t N>
	inline Queue<T, N>::Queue()
		: myContainer()
		, myFirst(0)
		, myEnd(0)
	{

	}

	template<typename T, std::size_t N>
	inline int Queue<T, N>::GetSize() const
	{
		return (myEnd - myFirst) % N;
	}

	template<typename T, std::size_t N>
	inline const T& Queue<T, N>::GetFront() const
	{
		assert(myFirst != myEnd && "Cannot retrieve element from an empty queue");
		return myContainer[myFirst];
	}
	template<typename T, std::size_t N>
	inline T& Queue<T, N>::GetFront()
	{
		assert(myFirst != myEnd && "Cannot retrieve element from an empty queue");
		return myContainer[myFirst];
	}

	template<typename T, std::size_t N>
	inline void Queue<T, N>::Enqueue(const T& aValue)
	{
		myContainer[myEnd] = aValue;
		myEnd = (myEnd + 1) % N;
	}

	template<typename T, std::size_t N>
	inline T Queue<T, N>::Dequeue()
	{
		assert(myFirst != myEnd && "Cannot retrieve element from an empty queue");
		T value = myContainer[myFirst];
		myFirst = (myFirst + 1) % N;
		return value;
	}
}
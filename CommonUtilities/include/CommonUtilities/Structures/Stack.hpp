#pragma once

#include <vector>
#include <cassert>

namespace CommonUtilities
{
	template<typename T>
	class Stack
	{
	public:
		Stack() = default;
		~Stack() = default;

		int GetSize() const;

		const T& GetTop() const;
		T& GetTop();

		void Push(const T& aValue);
		T Pop();

	private:
		std::vector<T> myContainer;
	};

	template<typename T>
	inline int Stack<T>::GetSize() const
	{
		return myContainer.size();
	}

	template<typename T>
	inline const T& Stack<T>::GetTop() const
	{
		assert(!myContainer.empty() && "Cannot retrieve an element from an empty container");
		return myContainer.back();
	}
	template<typename T>
	inline T& Stack<T>::GetTop()
	{
		assert(!myContainer.empty() && "Cannot retrieve an element from an empty container");
		return myContainer.back();
	}

	template<typename T>
	inline void Stack<T>::Push(const T& aValue)
	{
		myContainer.emplace_back(aValue);
	}
	template<typename T>
	inline T Stack<T>::Pop()
	{
		assert(!myContainer.empty() && "Cannot pop in an empty container!");

		T value = myContainer.back();
		myContainer.pop_back();
		return value;
	}
}
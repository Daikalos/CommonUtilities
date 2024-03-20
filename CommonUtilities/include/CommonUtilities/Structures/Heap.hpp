#pragma once

#include <vector>
#include <stdexcept>

namespace CommonUtilities
{
	template<typename T>
	class Heap
	{
	public:
		Heap() = default;
		~Heap() = default;

		int GetSize() const;
		bool IsEmpty() const;

		const T& GetTop() const;

		void Enqueue(const T& aElement);

		T Dequeue();

	private:
		int GetParent(int aPos);
		int GetLeftChild(int aPos);
		int GetRightChild(int aPos);

		void Swap(int aFirst, int aSecond);

		void MoveDown(int aPos);
		void MoveUp(int aPos);

		std::vector<T> myNodes;
	};

	template<typename T>
	inline int Heap<T>::GetSize() const
	{
		return (int)myNodes.size();
	}
	template<typename T>
	inline bool Heap<T>::IsEmpty() const
	{
		return myNodes.empty();
	}

	template<typename T>
	inline const T& Heap<T>::GetTop() const
	{
		return myNodes.front();
	}

	template<typename T>
	inline void Heap<T>::Enqueue(const T& aElement)
	{
		myNodes.emplace_back(aElement);
		MoveUp(GetSize() - 1);
	}

	template<typename T>
	inline T Heap<T>::Dequeue()
	{
		if (IsEmpty())
		{
			throw std::runtime_error("Cannot dequeue from an empty vector!");
		}

		T root = GetTop();

		myNodes[0] = std::move(myNodes.back());
		myNodes.pop_back();

		MoveDown(0);

		return root;
	}

	template<typename T>
	inline int Heap<T>::GetParent(int aPos)
	{
		return (aPos - 1) / 2;
	}
	template<typename T>
	inline int Heap<T>::GetLeftChild(int aPos)
	{
		return (2 * aPos) + 1;
	}
	template<typename T>
	inline int Heap<T>::GetRightChild(int aPos)
	{
		return (2 * aPos) + 2;
	}

	template<typename T>
	inline void Heap<T>::Swap(int aFirst, int aSecond)
	{
		std::swap(myNodes[aFirst], myNodes[aSecond]);
	}

	template<typename T>
	inline void Heap<T>::MoveDown(int aPos)
	{
		int left = GetLeftChild(aPos);
		int right = GetRightChild(aPos);

		int largest = aPos;
		if (left < GetSize() && myNodes[largest] < myNodes[left])
		{
			largest = left;
		}
		if (right < GetSize() && myNodes[largest] < myNodes[right])
		{
			largest = right;
		}

		if (largest != aPos)
		{
			Swap(aPos, largest);
			MoveDown(largest);
		}
	}
	template<typename T>
	inline void Heap<T>::MoveUp(int aPos)
	{
		if (aPos <= 0)
			return;

		int parent = GetParent(aPos);
		if (myNodes[parent] < myNodes[aPos])
		{
			Swap(aPos, parent);
			MoveUp(parent);
		}
	}
}

#pragma once

#include <cassert>

namespace CommonUtilities
{
	template<typename T>
	class DoublyLinkedList;

	template<typename T>
	class DoublyLinkedListNode
	{
	public:
		DoublyLinkedListNode(const DoublyLinkedListNode<T>&) = delete;
		DoublyLinkedListNode& operator=(const DoublyLinkedListNode<T>&) = delete;

		const T& GetValue() const;
		T& GetValue();

		DoublyLinkedListNode* GetNext() const;
		DoublyLinkedListNode* GetPrevious() const;

	private:
		friend class DoublyLinkedList<T>;

		DoublyLinkedListNode(const T& aValue);
		~DoublyLinkedListNode();

		DoublyLinkedListNode*	myNext;
		DoublyLinkedListNode*	myPrev;
		T						myData;
	};

	template<typename T>
	inline DoublyLinkedListNode<T>::DoublyLinkedListNode(const T& aValue)
		: myNext(nullptr)
		, myPrev(nullptr)
		, myData(aValue)
	{

	}

	template<typename T>
	inline DoublyLinkedListNode<T>::~DoublyLinkedListNode() = default;

	template<typename T>
	inline const T& DoublyLinkedListNode<T>::GetValue() const
	{
		return myData;
	}
	template<typename T>
	inline T& DoublyLinkedListNode<T>::GetValue()
	{
		return myData;
	}

	template<typename T>
	inline DoublyLinkedListNode<T>* DoublyLinkedListNode<T>::GetNext() const
	{
		return myNext;
	}
	template<typename T>
	inline DoublyLinkedListNode<T>* DoublyLinkedListNode<T>::GetPrevious() const
	{
		return myPrev;
	}

	template<typename T>
	class DoublyLinkedList
	{
	public:
		DoublyLinkedList();
		~DoublyLinkedList();

		// Returnerar antalet element i listan
		int GetSize() const;

		// Returnerar första noden i listan, eller nullptr om listan är tom
		DoublyLinkedListNode<T>* GetFirst();

		// Returnerar sista noden i listan, eller nullptr om listan är tom
		DoublyLinkedListNode<T>* GetLast();

		void InsertFirst(const T& aValue);
		void InsertLast(const T& aValue);

		// Skjuter in ett nytt element innan aNode
		void InsertBefore(DoublyLinkedListNode<T>* aNode, const T& aValue);

		// Skjuter in ett nytt element efter aNode
		void InsertAfter(DoublyLinkedListNode<T>* aNode, const T& aValue);

		// Plockar bort noden ur listan och frigör minne. (Det är ok att anta att
		// aNode är en nod i listan, och inte från en annan lista)
		void Remove(DoublyLinkedListNode<T>* aNode);

		// Hittar första elementet i listan som har ett visst värde. Jämförelsen
		// görs med operator==. Om inget element hittas returneras nullptr.
		DoublyLinkedListNode<T>* FindFirst(const T& aValue);

		// Hittar sista elementet i listan som har ett visst värde. Jämförelsen
		// görs med operator==. Om inget element hittas returneras nullptr.
		DoublyLinkedListNode<T>* FindLast(const T& aValue);

		// Plockar bort första elementet i listan som har ett visst värde. 
		// Jämförelsen görs med operator==. Om inget element hittas görs ingenting.
		// Returnerar true om ett element plockades bort, och false annars.
		bool RemoveFirst(const T& aValue);

		// Plockar bort sista elementet i listan som har ett visst värde.
		// Jämförelsen görs med operator==. Om inget element hittas görs ingenting.
		// Returnerar true om ett element plockades bort, och false annars.
		bool RemoveLast(const T& aValue);

	private:
		DoublyLinkedListNode<T>* myHead;
		DoublyLinkedListNode<T>* myTail;
		int mySize;
	};

	template<typename T>
	inline DoublyLinkedList<T>::DoublyLinkedList()
		: myHead(nullptr)
		, myTail(nullptr)
		, mySize(0)
	{

	}

	template<typename T>
	inline DoublyLinkedList<T>::~DoublyLinkedList()
	{
		DoublyLinkedListNode<T>* currNode = myHead;
		while (currNode != nullptr && currNode != (myTail + 1))
		{
			DoublyLinkedListNode<T>* nextNode = currNode->myNext;
			delete currNode;
			currNode = nextNode;
		}
	}

	template<typename T>
	inline int DoublyLinkedList<T>::GetSize() const
	{
		return mySize;
	}

	template<typename T>
	inline DoublyLinkedListNode<T>* DoublyLinkedList<T>::GetFirst()
	{
		return myHead;
	}
	template<typename T>
	inline DoublyLinkedListNode<T>* DoublyLinkedList<T>::GetLast()
	{
		return myTail;
	}

	template<typename T>
	inline void DoublyLinkedList<T>::InsertFirst(const T& aValue)
	{
		if (myHead == nullptr)
		{
			assert(myTail == nullptr);

			myHead = new DoublyLinkedListNode<T>(aValue);
			myTail = myHead;
		}
		else
		{
			DoublyLinkedListNode<T>* newHead = new DoublyLinkedListNode<T>(aValue);

			myHead->myPrev = newHead;
			newHead->myNext = myHead;

			myHead = newHead;
		}

		++mySize;
	}
	template<typename T>
	inline void DoublyLinkedList<T>::InsertLast(const T& aValue)
	{
		if (myTail == nullptr)
		{
			assert(myHead == nullptr);

			myTail = new DoublyLinkedListNode<T>(aValue);
			myHead = myTail;
		}
		else
		{
			DoublyLinkedListNode<T>* newTail = new DoublyLinkedListNode<T>(aValue);

			myTail->myNext = newTail;
			newTail->myPrev = myTail;

			myTail = newTail;
		}

		++mySize;
	}

	template<typename T>
	inline void DoublyLinkedList<T>::InsertBefore(DoublyLinkedListNode<T>* aNode, const T& aValue)
	{
		if (aNode == nullptr)
			return;

		if (aNode == myHead)
		{
			InsertFirst(aValue);
		}
		else
		{
			DoublyLinkedListNode<T>* newNode = new DoublyLinkedListNode<T>(aValue);

			newNode->myNext = aNode;
			newNode->myPrev = aNode->myPrev;

			aNode->myPrev->myNext = newNode;
			aNode->myPrev = newNode;

			++mySize;
		}
	}
	template<typename T>
	inline void DoublyLinkedList<T>::InsertAfter(DoublyLinkedListNode<T>* aNode, const T& aValue)
	{
		if (aNode == nullptr)
			return;

		if (aNode == myTail)
		{
			InsertLast(aValue);
		}
		else
		{
			InsertBefore(aNode->myNext, aValue);
		}
	}

	template<typename T>
	inline void DoublyLinkedList<T>::Remove(DoublyLinkedListNode<T>* aNode)
	{
		if (aNode == nullptr) // no delete a nullptr
			return;

		if (aNode == myHead && aNode == myTail) // only one node in the list
		{
			myHead = nullptr;
			myTail = nullptr;
		}
		else if (aNode == myHead)
		{
			myHead->myNext->myPrev = nullptr; // next prev does not point to anything anymore
			myHead = aNode->myNext;
		}
		else if (aNode == myTail)
		{
			myTail->myPrev->myNext = nullptr; // prev next does not point to anything anymore
			myTail = aNode->myPrev;
		}
		else
		{
			aNode->myPrev->myNext = aNode->myNext;
			aNode->myNext->myPrev = aNode->myPrev;
		}

		delete aNode;
		--mySize;
	}

	template<typename T>
	inline DoublyLinkedListNode<T>* DoublyLinkedList<T>::FindFirst(const T& aValue)
	{
		if (myHead == nullptr)
			return nullptr;

		DoublyLinkedListNode<T>* currNode = myHead;
		while (currNode != nullptr)
		{
			if (currNode->GetValue() == aValue)
				return currNode;

			currNode = currNode->myNext;
		}

		return nullptr;
	}
	template<typename T>
	inline DoublyLinkedListNode<T>* DoublyLinkedList<T>::FindLast(const T& aValue)
	{
		if (myTail == nullptr)
			return nullptr;

		DoublyLinkedListNode<T>* currNode = myTail;
		while (currNode != nullptr)
		{
			if (currNode->GetValue() == aValue)
				return currNode;

			currNode = currNode->myPrev;
		}

		return nullptr;
	}

	template<typename T>
	inline bool DoublyLinkedList<T>::RemoveFirst(const T& aValue)
	{
		DoublyLinkedListNode<T>* foundNode = FindFirst(aValue);
		if (foundNode != nullptr)
		{
			Remove(foundNode);
			return true;
		}

		return false;
	}
	template<typename T>
	inline bool DoublyLinkedList<T>::RemoveLast(const T& aValue)
	{
		DoublyLinkedListNode<T>* foundNode = FindLast(aValue);
		if (foundNode != nullptr)
		{
			Remove(foundNode);
			return true;
		}

		return false;
	}
}
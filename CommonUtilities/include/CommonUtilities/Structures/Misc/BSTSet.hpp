#pragma once

#include <cassert>
#include <utility>
#include <cmath>

namespace CommonUtilities
{
	template<typename T>
	class BSTSet;

	template<typename T>
	class BSTSetNode
	{
	public:
		BSTSetNode(const BSTSetNode&) = delete;
		BSTSetNode& operator=(const BSTSetNode&) = delete;

		const T& GetData() const;
		const BSTSetNode* GetLeft() const;
		const BSTSetNode* GetRight() const;

	private:
		BSTSetNode(const T& aValue);
		~BSTSetNode();

		void ReplaceChildWith(BSTSetNode* aNodeParent, BSTSetNode* aNode);
		void SetLeftSubTree(BSTSetNode* aNodeParent, BSTSetNode* aNode);
		void SetRightSubTree(BSTSetNode* aNodeParent, BSTSetNode* aNode);

		T myValue;
		BSTSetNode* myLeft;
		BSTSetNode* myRight;

		friend class BSTSet<T>;
	};

	template<typename T>
	class BSTSet
	{
	public:
		BSTSet();
		~BSTSet();

		const BSTSetNode<T>* GetRoot() const;
		bool HasElement(const T& aValue) const;
		void Insert(const T& aValue);
		void Remove(const T& aValue);
		void DSWBalance();

		int GetDepth() const;

	private:
		using NodeType = BSTSetNode<T>;

		auto SearchImpl(const NodeType* aNode, const T& aValue) const -> const NodeType*;
		auto SearchImpl(NodeType* aNode, const T& aValue) -> NodeType*;

		auto InsertImpl(NodeType* aNode, const T& aValue) -> NodeType*;

		auto RemoveImpl(NodeType* aNode, const T& aValue) -> NodeType*;
		auto FindMinValueNode(NodeType* aNode) const -> NodeType*;

		int TreeToBackbone(NodeType* aRoot);
		void BackboneToTree(NodeType* aRoot, int aNodeCount);
		void Compact(NodeType* aRoot, int aCount);

		void RotateLeft(NodeType* aGrandParent, NodeType* aParent, NodeType* aChild);
		void RotateRight(NodeType* aGrandParent, NodeType* aParent, NodeType* aChild);

		int GetDepthImpl(const NodeType* aNode) const;

		NodeType* myRoot;
	};


	template<typename T>
	inline const T& BSTSetNode<T>::GetData() const
	{
		return myValue;
	}
	template<typename T>
	inline const BSTSetNode<T>* BSTSetNode<T>::GetLeft() const
	{
		return myLeft;
	}
	template<typename T>
	inline const BSTSetNode<T>* BSTSetNode<T>::GetRight() const
	{
		return myRight;
	}

	template<typename T>
	inline BSTSetNode<T>::BSTSetNode(const T& aValue)
		: myValue(aValue)
		, myLeft(nullptr)
		, myRight(nullptr)
	{

	}

	template<typename T>
	inline BSTSetNode<T>::~BSTSetNode()
	{
		if (myLeft != nullptr)
		{
			delete myLeft;
			myLeft = nullptr;
		}

		if (myRight != nullptr)
		{
			delete myRight;
			myRight = nullptr;
		}
	}

	template<typename T>
	inline void BSTSetNode<T>::ReplaceChildWith(BSTSetNode* aNodeParent, BSTSetNode* aNode)
	{
		if (myLeft == aNodeParent)
		{
			myLeft = aNode;
		}
		else if (myRight == aNodeParent)
		{
			myRight = aNode;
		}
		else
		{
			assert(false && "This should not be reachable!");
		}
	}

	template<typename T>
	inline void BSTSetNode<T>::SetLeftSubTree(BSTSetNode* aNodeParent, BSTSetNode* aNode)
	{
		if (aNodeParent != nullptr)
		{
			aNodeParent->ReplaceChildWith(aNode, nullptr); // detach child from parent
		}

		myLeft = aNode;
	}

	template<typename T>
	inline void BSTSetNode<T>::SetRightSubTree(BSTSetNode* aNodeParent, BSTSetNode* aNode)
	{
		if (aNodeParent != nullptr)
		{
			aNodeParent->ReplaceChildWith(aNode, nullptr);
		}

		myRight = aNode;
	}

	template<typename T>
	inline BSTSet<T>::BSTSet()
		: myRoot(nullptr)
	{

	}

	template<typename T>
	inline BSTSet<T>::~BSTSet()
	{
		if (myRoot != nullptr)
		{
			delete myRoot;
			myRoot = nullptr;
		}
	}

	template<typename T>
	inline const BSTSetNode<T>* BSTSet<T>::GetRoot() const
	{
		return myRoot;
	}

	template<typename T>
	inline bool BSTSet<T>::HasElement(const T& aValue) const
	{
		return SearchImpl(myRoot, aValue) != nullptr;
	}
	template<typename T>
	inline void BSTSet<T>::Insert(const T& aValue)
	{
		myRoot = InsertImpl(myRoot, aValue);
	}
	template<typename T>
	inline void BSTSet<T>::Remove(const T& aValue)
	{
		myRoot = RemoveImpl(myRoot, aValue);
	}

	template<typename T>
	inline void BSTSet<T>::DSWBalance()
	{
		if (myRoot == nullptr)
			return;

		NodeType* pseudoRoot = new NodeType(T{});

		pseudoRoot->myRight = myRoot;

		int nodeCount = TreeToBackbone(pseudoRoot);

		BackboneToTree(pseudoRoot, nodeCount);

		myRoot = pseudoRoot->myRight;

		pseudoRoot->myLeft = nullptr; // set to null to prevent deletion of children
		pseudoRoot->myRight = nullptr;

		delete pseudoRoot;
	}

	template<typename T>
	inline int BSTSet<T>::GetDepth() const
	{
		return GetDepthImpl(myRoot);
	}

	template<typename T>
	inline auto BSTSet<T>::SearchImpl(const NodeType* aNode, const T& aValue) const -> const NodeType*
	{
		if (aNode == nullptr)
		{
			return nullptr; // element not found
		}

		if (aValue < aNode->myValue)
		{
			return SearchImpl(aNode->myLeft, aValue);
		}
		else if (aNode->myValue < aValue)
		{
			return SearchImpl(aNode->myRight, aValue);
		}

		return aNode;
	}

	template<typename T>
	inline auto BSTSet<T>::SearchImpl(NodeType* aNode, const T& aValue) -> NodeType*
	{
		return const_cast<NodeType*>(std::as_const(*this).SearchImpl(aNode, aValue));
	}

	template<typename T>
	inline auto BSTSet<T>::InsertImpl(NodeType* aNode, const T& aValue) -> NodeType*
	{
		if (aNode == nullptr)
		{
			return new NodeType(aValue);
		}

		if (aValue < aNode->myValue)
		{
			aNode->myLeft = InsertImpl(aNode->myLeft, aValue);
		}
		else if (aNode->myValue < aValue)
		{
			aNode->myRight = InsertImpl(aNode->myRight, aValue);
		}
		else
		{
			//assert(false && "Element already exists in the set >:(");
		}

		return aNode;
	}

	template<typename T>
	inline auto BSTSet<T>::RemoveImpl(NodeType* aNode, const T& aValue) -> NodeType*
	{
		if (aNode == nullptr)
		{
			return nullptr;
		}

		if (aValue < aNode->myValue)
		{
			aNode->myLeft = RemoveImpl(aNode->myLeft, aValue);
		}
		else if (aNode->myValue < aValue)
		{
			aNode->myRight = RemoveImpl(aNode->myRight, aValue);
		}
		else // equal, remove this node
		{
			if (aNode->myLeft == nullptr)
			{
				NodeType* temp = aNode->myRight;
				aNode->myRight = nullptr; // set to null to prevent deletion of child
				delete aNode;
				return temp;
			}
			else if (aNode->myRight == nullptr)
			{
				NodeType* temp = aNode->myLeft;
				aNode->myLeft = nullptr;
				delete aNode;
				return temp;
			}
			else // has two children
			{
				NodeType* minValueNode = FindMinValueNode(aNode->myRight); // find smallest myValue in the myRight sub-tree

				aNode->myValue = std::move(minValueNode->myValue);
				aNode->myRight = RemoveImpl(aNode->myRight, aNode->myValue);
			}
		}

		return aNode;
	}

	template<typename T>
	inline auto BSTSet<T>::FindMinValueNode(NodeType* aNode) const -> NodeType*
	{
		assert(aNode != nullptr && "NodeType should never be null here!");

		if (aNode->myLeft != nullptr)
		{
			return FindMinValueNode(aNode->myLeft);
		}

		return aNode;
	}

	template<typename T>
	inline int BSTSet<T>::TreeToBackbone(NodeType* aRoot)
	{
		int nodeCount = 0;

		NodeType* tail = aRoot;
		NodeType* rest = tail->myRight;

		while (rest != nullptr)
		{
			if (rest->myLeft != nullptr)
			{
				NodeType* temp = rest->myLeft;
				RotateRight(tail, rest, rest->myLeft);
				rest = temp;
			}
			else
			{
				++nodeCount;

				tail = rest;
				rest = rest->myRight;
			}
		}

		return nodeCount;
	}

	template<typename T>
	inline void BSTSet<T>::BackboneToTree(NodeType* aRoot, int aNodeCount)
	{
		// 2 ^ (floor(log2(n + 1))) - 1
		int m = (int)std::powf(2.0f, std::floorf(std::log2f((float)aNodeCount + 1))) - 1;

		Compact(aRoot, aNodeCount - m);

		while (m > 1)
		{
			m /= 2;
			Compact(aRoot, m);
		}
	}

	template<typename T>
	inline void BSTSet<T>::Compact(NodeType* aRoot, int aCount)
	{
		NodeType* scanner = aRoot;
		for (int i = 0; i < aCount; ++i)
		{
			NodeType* child = scanner->myRight;
			RotateLeft(scanner, child, child->myRight);
			scanner = scanner->myRight;
		}
	}

	template<typename T>
	inline void BSTSet<T>::RotateLeft(NodeType* aGrandParent, NodeType* aParent, NodeType* aChild)
	{
		assert(aParent->myRight == aChild && "This should not be reachable!");

		if (aGrandParent != nullptr)
		{
			aGrandParent->ReplaceChildWith(aParent, aChild);
		}

		aParent->SetRightSubTree(aChild, aChild->myLeft);
		aChild->SetLeftSubTree(nullptr, aParent); // parent no longer has a grand parent
	}
	template<typename T>
	inline void BSTSet<T>::RotateRight(NodeType* aGrandParent, NodeType* aParent, NodeType* aChild)
	{
		assert(aParent->myLeft == aChild && "This should not be reachable!");

		if (aGrandParent != nullptr)
		{
			aGrandParent->ReplaceChildWith(aParent, aChild);
		}

		aParent->SetLeftSubTree(aChild, aChild->myRight);
		aChild->SetRightSubTree(nullptr, aParent); // parent no longer has a grand parent
	}

	template<typename T>
	inline int BSTSet<T>::GetDepthImpl(const NodeType* aNode) const
	{
		if (aNode == nullptr)
		{
			return 0;
		}

		int leftDepth = GetDepthImpl(aNode->myLeft);
		int rightDepth = GetDepthImpl(aNode->myRight);

		if (leftDepth > rightDepth)
		{
			return leftDepth + 1;
		}

		return rightDepth + 1;
	}
}
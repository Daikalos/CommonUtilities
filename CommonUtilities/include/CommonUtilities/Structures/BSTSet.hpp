#pragma once

#include <memory>
#include <utility>
#include <cassert>

template<typename T>
class BSTSet
{
public:
	BSTSet();
	~BSTSet() = default;

	bool HasElement(const T& aValue) const;

	void Insert(const T& aValue);

	void Remove(const T& aValue);

private:
	struct Node;

	using NodePtr = std::unique_ptr<Node>;

	struct Node
	{
		T value;
		NodePtr left;
		NodePtr right;
	};

	auto SearchImpl(const Node* aNode, const T& aValue) const -> const Node*;
	auto SearchImpl(Node* aNode, const T& aValue) -> Node*;

	void InsertImpl(Node& aNode, const T& aValue);

	void RemoveImpl(NodePtr& aNode, const T& aValue);
	void RemoveNode(NodePtr& aNode);

	T FindSuccessor(NodePtr& aNode);

	NodePtr myRoot;
};

template<typename T>
inline BSTSet<T>::BSTSet()
	: myRoot(nullptr)
{

}

template<typename T>
inline bool BSTSet<T>::HasElement(const T& aValue) const
{
	return SearchImpl(myRoot.get(), aValue) != nullptr;
}

template<typename T>
inline void BSTSet<T>::Insert(const T& aValue)
{
	if (myRoot == nullptr)
	{
		myRoot = std::make_unique<Node>(aValue);
	}
	else
	{
		InsertImpl(*myRoot, aValue);
	}
}

template<typename T>
inline void BSTSet<T>::Remove(const T& aValue)
{
	if (myRoot != nullptr)
	{
		RemoveImpl(myRoot, aValue);
	}
}

template<typename T>
inline auto BSTSet<T>::SearchImpl(const Node* aNode, const T& aValue) const -> const Node*
{
	if (aNode == nullptr)
	{
		return nullptr;
	}

	if (aValue < aNode->value)
	{
		return SearchImpl(aNode->left.get(), aValue);
	}
	else if (aNode->value < aValue)
	{
		return SearchImpl(aNode->right.get(), aValue);
	}

	return aNode;
}

template<typename T>
inline auto BSTSet<T>::SearchImpl(Node* aNode, const T& aValue) -> Node*
{
	return const_cast<Node*>(std::as_const(*this).SearchImpl(aNode, aValue));
}

template<typename T>
inline void BSTSet<T>::InsertImpl(Node& aNode, const T& aValue)
{
	if (aValue < aNode.value)
	{
		if (aNode.left == nullptr)
		{
			aNode.left = std::make_unique<Node>(aValue);
		}
		else
		{
			InsertImpl(*aNode.left, aValue);
		}
	}
	else if (aNode.value < aValue)
	{
		if (aNode.right == nullptr)
		{
			aNode.right = std::make_unique<Node>(aValue);
		}
		else
		{
			InsertImpl(*aNode.right, aValue);
		}
	}
	else
	{
		assert(false && "Element already exists in the set >:(");
	}
}

template<typename T>
inline void BSTSet<T>::RemoveImpl(NodePtr& aNode, const T& aValue)
{
	if (aNode == nullptr)
	{
		assert(false && "No node found to delete!");
	}
	
	if (aValue < aNode->value)
	{
		RemoveImpl(aNode->left, aValue);
	}
	else if (aNode->value < aValue)
	{
		RemoveImpl(aNode->right, aValue);
	}
	else // equal
	{
		RemoveNode(aNode);
	}
}

template<typename T>
inline void BSTSet<T>::RemoveNode(NodePtr& aNode)
{
	if (aNode->left == nullptr)
	{
		aNode = std::move(aNode->right);
	}
	else if (aNode->right == nullptr)
	{
		aNode = std::move(aNode->left);
	}
	else
	{
		aNode->value = FindSuccessor(aNode->right); // swap values with successor and remove it
	}
}

template<typename T>
inline T BSTSet<T>::FindSuccessor(NodePtr& aNode)
{
	if (aNode->left != nullptr)
	{
		return FindSuccessor(aNode->left);
	}

	T result = std::move(aNode->value);
	aNode = std::move(aNode->right);

	return result;
}

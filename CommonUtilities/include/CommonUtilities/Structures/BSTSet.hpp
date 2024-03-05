#pragma once

#include <memory>
#include <utility>
#include <cassert>

namespace CommonUtilities
{
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
		class Node;

		using NodePtr = std::unique_ptr<Node>;

		class Node
		{
		public:
			Node(const T& aValue);

			Node(const Node&) = delete;
			Node& operator=(const Node&) = delete;

			Node(Node&&) = delete;
			Node& operator=(Node&&) = delete;

			T		value;
			NodePtr left;
			NodePtr right;
		};

		auto SearchImpl(const NodePtr& aNode, const T& aValue) const -> const NodePtr&;
		auto SearchImpl(NodePtr& aNode, const T& aValue) -> NodePtr&;

		void InsertImpl(Node& aNode, const T& aValue);

		void RemoveNode(NodePtr& aNode);

		T FindSuccessor(NodePtr& aNode);

		NodePtr myRoot;
	};

	template<typename T>
	inline BSTSet<T>::Node::Node(const T& aValue) : value(aValue)
	{

	}

	template<typename T>
	inline BSTSet<T>::BSTSet()
		: myRoot(nullptr)
	{

	}

	template<typename T>
	inline bool BSTSet<T>::HasElement(const T& aValue) const
	{
		return SearchImpl(myRoot, aValue) != nullptr;
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
		NodePtr& nodeToDelete = SearchImpl(myRoot, aValue);
		if (nodeToDelete != nullptr)
		{
			RemoveNode(nodeToDelete);
		}
	}

	template<typename T>
	inline auto BSTSet<T>::SearchImpl(const NodePtr& aNode, const T& aValue) const -> const NodePtr&
	{
		if (aNode == nullptr)
		{
			return nullptr;
		}

		if (aValue < aNode->value)
		{
			return SearchImpl(aNode->left, aValue);
		}
		else if (aNode->value < aValue)
		{
			return SearchImpl(aNode->right, aValue);
		}

		return aNode;
	}

	template<typename T>
	inline auto BSTSet<T>::SearchImpl(NodePtr& aNode, const T& aValue) -> NodePtr&
	{
		return const_cast<NodePtr&>(std::as_const(*this).SearchImpl(aNode, aValue));
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
			//assert(false && "Element already exists in the set >:(");
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
		else // has two children
		{
			aNode->value = FindSuccessor(aNode->right); // swap values with successor and remove it
		}
	}

	template<typename T>
	inline T BSTSet<T>::FindSuccessor(NodePtr& aNode)
	{
		if (aNode->left != nullptr) // find smallest value in right sub-tree
		{
			return FindSuccessor(aNode->left);
		}

		T result = std::move(aNode->value);
		aNode = std::move(aNode->right);

		return result;
	}
}
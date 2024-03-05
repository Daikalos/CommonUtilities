#pragma once

#include <cassert>
#include <utility>

namespace CommonUtilities
{
	template<typename T>
	class BSTSet
	{
	public:
		BSTSet();
		~BSTSet();

		bool HasElement(const T& aValue) const;

		void Insert(const T& aValue);

		void Remove(const T& aValue);

	private:
		class Node
		{
		public:
			Node(const T& aValue);
			~Node();

			Node(const Node&) = delete;
			Node& operator=(const Node&) = delete;

			Node(Node&&) = delete;
			Node& operator=(Node&&) = delete;

			T		value;
			Node*	left;
			Node*	right;
		};

		auto SearchImpl(const Node* aNode, const T& aValue) const -> const Node*;
		auto SearchImpl(Node* aNode, const T& aValue) -> Node*;

		auto InsertImpl(Node* aNode, const T& aValue) -> Node*;

		auto RemoveImpl(Node* aNode, const T& aValue) -> Node*;
		auto FindMinValueNode(Node* aNode) const -> Node*;

		Node* myRoot;
	};

	template<typename T>
	inline BSTSet<T>::Node::Node(const T& aValue) 
		: value(aValue)
		, left(nullptr)
		, right(nullptr)
	{

	}

	template<typename T>
	inline BSTSet<T>::Node::~Node()
	{
		if (left != nullptr)
		{
			delete left;
			left = nullptr;
		}

		if (right != nullptr)
		{
			delete right;
			right = nullptr;
		}
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
	inline auto BSTSet<T>::SearchImpl(const Node* aNode, const T& aValue) const -> const Node*
	{
		if (aNode == nullptr)
		{
			return nullptr; // element not found
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
	inline auto BSTSet<T>::SearchImpl(Node* aNode, const T& aValue) -> Node*
	{
		return const_cast<Node*>(std::as_const(*this).SearchImpl(aNode, aValue));
	}

	template<typename T>
	inline auto BSTSet<T>::InsertImpl(Node* aNode, const T& aValue) -> Node*
	{
		if (aNode == nullptr)
		{
			return new Node(aValue);
		}

		if (aValue < aNode->value)
		{
			aNode->left = InsertImpl(aNode->left, aValue);
		}
		else if (aNode->value < aValue)
		{
			aNode->right = InsertImpl(aNode->right, aValue);
		}
		else
		{
			//assert(false && "Element already exists in the set >:(");
		}

		return aNode;
	}

	template<typename T>
	inline auto BSTSet<T>::RemoveImpl(Node* aNode, const T& aValue) -> Node*
	{
		if (aNode == nullptr)
		{
			return nullptr;
		}

		if (aValue < aNode->value)
		{
			aNode->left = RemoveImpl(aNode->left, aValue);
		}
		else if (aNode->value < aValue)
		{
			aNode->right = RemoveImpl(aNode->right, aValue);
		}
		else // equal, remove this node
		{
			if (aNode->left == nullptr)
			{
				Node* temp = aNode->right;
				aNode->right = nullptr; // set to null to prevent deletion of child
				delete aNode;
				return temp;
			}
			else if (aNode->right == nullptr)
			{
				Node* temp = aNode->left;
				aNode->left = nullptr;
				delete aNode;
				return temp;
			}
			else // has two children
			{
				Node* minValueNode = FindMinValueNode(aNode->right); // find smallest value in the right sub-tree

				aNode->value = std::move(minValueNode->value);
				aNode->right = RemoveImpl(aNode->right, aNode->value);
			}
		}

		return aNode;
	}

	template<typename T>
	inline auto BSTSet<T>::FindMinValueNode(Node* aNode) const -> Node*
	{
		assert(aNode != nullptr && "Node should never be null here!");

		if (aNode->left != nullptr)
		{
			return FindMinValueNode(aNode->left);
		}

		return aNode;
	}
}
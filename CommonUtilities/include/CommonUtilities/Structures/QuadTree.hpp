#pragma once

#include <vector>
#include <shared_mutex>
#include <concepts>

#include <CommonUtilities/Math/Rectangle.hpp>
#include <CommonUtilities/Math/Vector2.hpp>
#include <CommonUtilities/Config.h>

#include <CommonUtilities/Structures/FreeVector.hpp>

namespace CommonUtilities
{
	///	Quadtree based upon: https://stackoverflow.com/questions/41946007/efficient-and-well-explained-implementation-of-a-quadtree-for-2d-collision-det
	///
	template<std::equality_comparable T>
	class QuadTree
	{
	public:
		using ElementType	= T;
		using ValueType		= std::remove_const_t<T>;
		using SizeType		= int;

		static constexpr int ourChildCount = 4;

		struct Element
		{		
			RectFloat rect;	// rectangle encompassing the item
			ValueType item;
		};

		QuadTree(const RectFloat& aRootRect, int aMaxElements = 8, int aMaxDepth = 8);

		/// Inserts given element into the quadtree.
		/// 
		/// \param Rect: Rectangle encompassing item
		/// \param Args: Constructor parameters for item
		/// 
		/// \returns Index to element, can be used to directly access it when, e.g., erasing it from the tree
		/// 
		template<typename... Args> requires std::constructible_from<T, Args...>
		int Insert(const RectFloat& aRect, Args&&... someArgs);

		/// Attempts to erase element from tree.
		/// 
		/// \param Index: Index to element to erase
		/// 
		/// \returns True if successfully removed the element, otherwise false
		/// 
		bool Erase(SizeType aIndex);

		/// Updates the given element with new data.
		/// 
		/// \param Index: index to element
		/// \param Args: Data to update the current element
		/// 
		/// \returns True if successfully updated the element, otherwise false
		/// 
		template<typename... Args> requires std::constructible_from<T, Args...>
		bool Update(SizeType aIndex, Args&&... someArgs);

		/// Retrieves an element.
		/// 
		/// \param Index: index to element
		/// 
		NODISC auto Get(SizeType aIndex) -> ValueType&;

		/// Retrieves an element.
		/// 
		/// \param Index: index to element
		/// 
		NODISC auto Get(SizeType aIndex) const -> const ValueType&;

		/// Retrieves the rectangle encompassing item
		/// 
		/// \param Index: index to item.
		/// 
		NODISC auto GetRect(SizeType aIndex) const -> const RectFloat&;

		/// Queries the tree for elements.
		/// 
		/// \param Rect: Bounding rectangle where all the elements are contained
		/// 
		/// \returns List of entities contained within the bounding rectangle
		/// 
		NODISC auto Query(const RectFloat& aRect) const -> std::vector<SizeType>;

		/// Queries the tree for elements.
		/// 
		/// \param Point: Point to search for overlapping elements
		/// 
		/// \returns List of entities contained at the point
		/// 
		NODISC auto Query(const Vector2f& aPoint) const-> std::vector<SizeType>;

		/// Performs a lazy cleanup of the tree, should be called after items have been erased.
		/// 
		void Cleanup();

		/// Clears the tree
		/// 
		void Clear();

	private:
		struct Node
		{
			SizeType firstChild	 {-1};	// points to first sub-branch or first element index
			SizeType count		 {0};	// -1 for branch or it's a leaf and count means number of elements
		};

		struct NodeReg
		{
			RectFloat rect;
			SizeType index {0};
			SizeType depth {0};
		};

		struct ElementPtr
		{
			SizeType element {-1};	// points to item in elements, not sure if even needed, seems to always be aligned anyways
			SizeType next	 {-1};	// points to next elt ptr, or -1 means end of items
		};

		void NodeInsert(const NodeReg& aNode, SizeType aEltIndex);
		void LeafInsert(const NodeReg& aNode, SizeType aEltIndex);

		auto FindLeaves(const NodeReg& aNode, const RectFloat& aRect) const -> std::vector<NodeReg>;

		static bool IsLeaf(const Node& aNode);
		static bool IsBranch(const Node& aNode);

		FreeVector<Element>		myElements;		// all the elements
		FreeVector<ElementPtr>	myElementsPtr;	// all the element ptrs
		FreeVector<Node>		myNodes;

		RectFloat	myRootRect;

		SizeType	myFreeNode			{-1};
		SizeType	myMaxElements		{8}; // max elements before subdivision
		SizeType	myMaxDepth			{8};  // max depth before no more leaves will be created

		mutable std::vector<bool> myVisited;
		mutable std::shared_mutex myMutex;
	};

	template<std::equality_comparable T>
	inline QuadTree<T>::QuadTree(const RectFloat& aRootRect, int aMaxElements, int aMaxDepth)
		: myRootRect(aRootRect), myMaxElements(aMaxElements), myMaxDepth(aMaxDepth)
	{
		myNodes.emplace();
	}

	template<std::equality_comparable T>
	template<typename... Args> requires std::constructible_from<T, Args...>
	inline int QuadTree<T>::Insert(const RectFloat& aRect, Args&&... someArgs)
	{
		std::unique_lock lock(myMutex);

		if (!myRootRect.Overlaps(aRect)) // dont attempt to add if outside boundary
			return -1;

		const auto aIndex = static_cast<SizeType>(myElements.emplace(aRect, std::forward<Args>(someArgs)...));
		NodeInsert({ myRootRect, 0, 0 }, aIndex);

		return aIndex;
	}

	template<std::equality_comparable T>
	inline bool QuadTree<T>::Erase(SizeType aIndex)
	{
		std::unique_lock lock(myMutex);

		const RectFloat& rect = myElements[aIndex].rect;
		const auto& leaves = FindLeaves({ myRootRect, 0, 0 }, rect);

		if (leaves.empty())
			return false;

		for (const auto& leaf : leaves)
		{
			Node& node = myNodes[leaf.index];

			auto ptrIndex = node.firstChild;
			auto prvIdx = -1;

			while (ptrIndex != -1 && myElementsPtr[ptrIndex].element != aIndex)
			{
				prvIdx = ptrIndex;
				ptrIndex = myElementsPtr[ptrIndex].next;
			}

			if (ptrIndex != -1)
			{
				const auto nextIndex = myElementsPtr[ptrIndex].next;

				if (prvIdx == -1)
				{
					node.firstChild = nextIndex;
				}
				else
				{
					myElementsPtr[prvIdx].next = nextIndex;
				}

				myElementsPtr.erase(ptrIndex);

				--node.count;

				assert(node.count >= 0);
			}
		}

		myElements.erase(aEltIndex);

		return true;
	}

	template<std::equality_comparable T>
	template<typename... Args> requires std::constructible_from<T, Args...>
	inline bool QuadTree<T>::Update(SizeType aIndex, Args&&... someArgs)
	{
		std::unique_lock lock(myMutex);

		if (aIndex >= myElements.size() || !myElements.valid(aIndex))
			return false;

		myElements[aIndex].item = T{std::forward<Args>(someArgs)...};

		return true;
	}

	template<std::equality_comparable T>
	auto QuadTree<T>::Get(SizeType aIndex) -> ValueType&
	{
		return myElements[aIndex].item;
	}

	template<std::equality_comparable T>
	auto QuadTree<T>::Get(SizeType aIndex) const -> const ValueType&
	{
		return myElements[aIndex].item;
	}

	template<std::equality_comparable T>
	inline auto QuadTree<T>::GetRect(SizeType aIndex) const -> const RectFloat&
	{
		return myElements[aIndex].rect;
	}

	template<std::equality_comparable T>
	inline auto QuadTree<T>::Query(const RectFloat& aRect) const -> std::vector<SizeType>
	{
		std::shared_lock lock(myMutex);

		std::vector<SizeType> result;

		myVisited.resize(myElements.size());

		for (const NodeReg& leaf : FindLeaves({ myRootRect, 0, 0 }, aRect))
		{
			const auto nd_index = leaf.index;
			
			for (auto child = myNodes[nd_index].firstChild; child != -1;)
			{
				const auto eltIndex	= myElementsPtr[child].element;
				const auto& elt		= myElements[eltIndex];

				if (!myVisited[eltIndex] && elt.rect.Overlaps(aRect))
				{
					result.emplace_back(eltIndex);
					myVisited[eltIndex] = true;
				}

				child = myElementsPtr[child].next;
			}
		}

		for (const auto eltIndex : result)
		{
			myVisited[eltIndex] = false;
		}

		return result;
	}

	template<std::equality_comparable T>
	inline auto QuadTree<T>::Query(const Vector2f& aPoint) const -> std::vector<SizeType>
	{
		return Query(RectFloat(aPoint.x, aPoint.y, 0.f, 0.f));
	}

	template<std::equality_comparable T>
	inline void QuadTree<T>::Cleanup()
	{
		std::unique_lock lock(myMutex);

		assert(!myNodes.empty());

		std::vector<int> toProcess;
		if (myNodes[0].count == -1)
		{
			toProcess.emplace_back(0); // push root
		}

		while (!toProcess.empty())
		{
			Node& node = myNodes[toProcess.back()];
			toProcess.pop_back();

			int num_empty = 0;
			for (int i = 0; i < ourChildCount; ++i)
			{
				const int childIndex	= node.firstChild + i;
				const Node& child		= myNodes[childIndex];

				if (child.count == -1)
				{
					toProcess.emplace_back(childIndex);
				}
				else if (child.count == 0)
				{
					++num_empty;
				}
			}

			if (num_empty == ourChildCount)
			{
				myNodes.erase(node.firstChild + 3);
				myNodes.erase(node.firstChild + 2);
				myNodes.erase(node.firstChild + 1);
				myNodes.erase(node.firstChild + 0);

				node = {}; // reset
			}
		}
	}

	template<std::equality_comparable T>
	inline void QuadTree<T>::Clear()
	{
		myElements.clear();
		myElementsPtr.clear();
		myNodes.clear();

		myFreeNode = {-1};
	}

	template<std::equality_comparable T>
	inline void QuadTree<T>::NodeInsert(const NodeReg& aNode, SizeType aEltIndex)
	{
		const RectFloat& rect = myElements[aEltIndex].rect;
		for (const auto& leaf : FindLeaves(aNode, rect))
		{
			LeafInsert(leaf, aEltIndex);
		}
	}

	template<std::equality_comparable T>
	inline void QuadTree<T>::LeafInsert(const NodeReg& aNode, SizeType aEltIndex)
	{
		Node& node = myNodes[aNode.index];
		node.firstChild = myElementsPtr.emplace(aEltIndex, node.firstChild);

		if (node.count == myMaxElements && aNode.depth < myMaxDepth)
		{
			std::vector<SizeType> elements;
			while (node.firstChild != -1)
			{
				const auto index = node.firstChild;

				const auto next = myElementsPtr[index].next;
				const auto elt = myElementsPtr[index].element;

				node.firstChild = next;
				myElementsPtr.erase(index);

				elements.emplace_back(elt);
			}

			const auto fc = myNodes.emplace();
			myNodes.emplace();
			myNodes.emplace();
			myNodes.emplace();

			node.firstChild = fc;
			node.count = -1; // set as branch

			for (const auto elt : elements)
			{
				NodeInsert(aNode, elt);
			}
		}
		else
			++node.count;
	}

	template<std::equality_comparable T>
	inline auto QuadTree<T>::FindLeaves(const NodeReg& aNode, const RectFloat& aRect) const -> std::vector<NodeReg>
	{
		std::vector<NodeReg> leaves;
		std::vector<NodeReg> toProcess;

		toProcess.emplace_back(aNode);

		while (!toProcess.empty())
		{
			const NodeReg nd = toProcess.back();
			toProcess.pop_back();

			if (IsLeaf(myNodes[nd.index]))
			{
				leaves.emplace_back(nd);
			}
			else
			{
				const auto fc	= myNodes[nd.index].firstChild;
				const auto hx	= nd.rect.width / 2.0f;
				const auto hy	= nd.rect.height / 2.0f;
				const auto l	= nd.rect.left - hx;
				const auto t	= nd.rect.top - hy;
				const auto r	= nd.rect.left + hx;
				const auto b	= nd.rect.top + hy;

				if (aRect.top <= nd.rect.top)
				{
					if (aRect.left <= nd.rect.left)
						toProcess.emplace_back(RectFloat(l, t, hx, hy), fc + 0, nd.depth + 1);
					if (aRect.Right() > nd.rect.left)
						toProcess.emplace_back(RectFloat(r, t, hx, hy), fc + 1, nd.depth + 1);
				}
				if (aRect.Bottom() > nd.rect.top)
				{
					if (aRect.left <= nd.rect.left)
						toProcess.emplace_back(RectFloat(l, b, hx, hy), fc + 2, nd.depth + 1);
					if (aRect.Right() > nd.rect.left)
						toProcess.emplace_back(RectFloat(r, b, hx, hy), fc + 3, nd.depth + 1);
				}
			}
		}

		return leaves;
	}

	template<std::equality_comparable T>
	inline bool QuadTree<T>::IsLeaf(const Node& aNode)
	{
		return aNode.count != -1;
	}
	template<std::equality_comparable T>
	inline bool QuadTree<T>::IsBranch(const Node& aNode)
	{
		return aNode.count == -1;
	}
}
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
	template<std::equality_comparable T = int>
	class QuadTree
	{
	public:
		using ElementType	= T;
		using ValueType		= std::remove_const_t<T>;
		using SizeType		= int;

		static constexpr int ourChildCount = 4;

		struct Element
		{		
			RectFloat rect;			// rectangle encompassing the item
			ValueType item;
		};

		QuadTree(const RectFloat& aRootRect, int aMaxElements = 8, int aMaxDepth = 8);

		/// Inserts given element into the quadtree.
		/// 
		/// \param Element: Element to insert containing the item and bounding rectangle.
		/// 
		/// \returns Index to element, can be used to directly access it when, e.g., erasing it from the tree.
		/// 
		template<typename... Args> requires std::constructible_from<T, Args...>
		int Insert(const RectFloat& rect, Args&&... args);

		/// Attempts to erase element from tree.
		/// 
		/// \param EltIdx: Index to element to erase.
		/// 
		/// \returns True if successfully removed the element, otherwise false.
		/// 
		bool Erase(SizeType elt_idx);

		/// Updates the given element with new data.
		/// 
		/// \param Index: index to element.
		/// \param Args: Data to update the current element.
		/// 
		/// \returns True if successfully updated the element, otherwise false.
		/// 
		template<typename... Args> requires std::constructible_from<T, Args...>
		bool Update(SizeType idx, Args&&... args);

		/// Retrieves an element.
		/// 
		/// \param Index: index to element.
		/// 
		NODISC auto Get(SizeType idx) -> ValueType&;

		/// Retrieves an element.
		/// 
		/// \param Index: index to element.
		/// 
		NODISC auto Get(SizeType idx) const -> const ValueType&;

		/// Retrieves the rectangle encompassing item.
		/// 
		/// \param Index: index to item.
		/// 
		NODISC auto GetRect(SizeType idx) const -> const RectFloat&;

		/// Queries the tree for elements.
		/// 
		/// \param Rect: Bounding rectangle where all the elements are contained.
		/// 
		/// \returns List of entities contained within the bounding rectangle.
		/// 
		NODISC auto Query(const RectFloat& rect) const -> std::vector<SizeType>;

		/// Queries the tree for elements.
		/// 
		/// \param Point: Point to search for overlapping elements.
		/// 
		/// \returns List of entities contained at the point.
		/// 
		NODISC auto Query(const Vector2f& point) const-> std::vector<SizeType>;

		/// Performs a lazy cleanup of the tree; can only be called if erase has been used.
		/// 
		void Cleanup();

		/// Clears the tree
		/// 
		void Clear();

	private:
		struct Node
		{
			SizeType firstChild {-1};	// points to first sub-branch or first element index
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

		void NodeInsert(const NodeReg& node, SizeType elt_idx);
		void LeafInsert(const NodeReg& node, SizeType elt_idx);

		auto FindLeaves(const NodeReg& node, const RectFloat& rect) const -> std::vector<NodeReg>;

		static bool IsLeaf(const Node& node);
		static bool IsBranch(const Node& node);

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
	inline int QuadTree<T>::Insert(const RectFloat& rect, Args&&... args)
	{
		std::unique_lock lock(myMutex);

		if (!myRootRect.Overlaps(rect)) // dont attempt to add if outside boundary
			return -1;

		const auto idx = static_cast<SizeType>(myElements.emplace(rect, std::forward<Args>(args)...));
		NodeInsert({ myRootRect, 0, 0 }, idx);

		return idx;
	}

	template<std::equality_comparable T>
	inline bool QuadTree<T>::Erase(SizeType elt_idx)
	{
		std::unique_lock lock(myMutex);

		const RectFloat& rect = myElements[elt_idx].rect;
		const auto& leaves = FindLeaves({ myRootRect, 0, 0 }, rect);

		if (leaves.empty())
			return false;

		for (const auto& leaf : leaves)
		{
			Node& node = myNodes[leaf.index];

			auto ptr_idx = node.firstChild;
			auto prv_idx = -1;

			while (ptr_idx != -1 && myElementsPtr[ptr_idx].element != elt_idx)
			{
				prv_idx = ptr_idx;
				ptr_idx = myElementsPtr[ptr_idx].next;
			}

			if (ptr_idx != -1)
			{
				const auto next_index = myElementsPtr[ptr_idx].next;

				if (prv_idx == -1)
				{
					node.firstChild = next_index;
				}
				else
				{
					myElementsPtr[prv_idx].next = next_index;
				}

				myElementsPtr.erase(ptr_idx);

				--node.count;

				assert(node.count >= 0);
			}
		}

		myElements.erase(elt_idx);

		return true;
	}

	template<std::equality_comparable T>
	template<typename... Args> requires std::constructible_from<T, Args...>
	inline bool QuadTree<T>::Update(SizeType idx, Args&&... args)
	{
		std::unique_lock lock(myMutex);

		if (idx >= myElements.size() || !myElements.valid(idx))
			return false;

		myElements[idx].item = T{std::forward<Args>(args)...};

		return true;
	}

	template<std::equality_comparable T>
	auto QuadTree<T>::Get(SizeType idx) -> ValueType&
	{
		return myElements[idx].item;
	}

	template<std::equality_comparable T>
	auto QuadTree<T>::Get(SizeType idx) const -> const ValueType&
	{
		return myElements[idx].item;
	}

	template<std::equality_comparable T>
	inline auto QuadTree<T>::GetRect(SizeType idx) const -> const RectFloat&
	{
		return myElements[idx].rect;
	}

	template<std::equality_comparable T>
	inline auto QuadTree<T>::Query(const RectFloat& rect) const -> std::vector<SizeType>
	{
		std::shared_lock lock(myMutex);

		std::vector<SizeType> result;

		myVisited.resize(myElements.size());

		for (const NodeReg& leaf : FindLeaves({ myRootRect, 0, 0 }, rect))
		{
			const auto nd_index = leaf.index;
			
			for (auto child = myNodes[nd_index].firstChild; child != -1;)
			{
				const auto elt_idx	= myElementsPtr[child].element;
				const auto& elt		= myElements[elt_idx];

				if (!myVisited[elt_idx] && elt.rect.Overlaps(rect))
				{
					result.emplace_back(elt_idx);
					myVisited[elt_idx] = true;
				}

				child = myElementsPtr[child].next;
			}
		}

		for (const auto elt_idx : result)
		{
			myVisited[elt_idx] = false;
		}

		return result;
	}

	template<std::equality_comparable T>
	inline auto QuadTree<T>::Query(const Vector2f& point) const -> std::vector<SizeType>
	{
		return Query(RectFloat(point.x, point.y, 0.f, 0.f));
	}

	template<std::equality_comparable T>
	inline void QuadTree<T>::Cleanup()
	{
		std::unique_lock lock(myMutex);

		assert(!myNodes.empty());

		std::vector<int> to_process;
		if (myNodes[0].count == -1)
			to_process.emplace_back(0); // push root

		while (!to_process.empty())
		{
			Node& node = myNodes[to_process.back()];
			to_process.pop_back();

			int num_empty = 0;
			for (int i = 0; i < ourChildCount; ++i)
			{
				const int child_index	= node.firstChild + i;
				const Node& child		= myNodes[child_index];

				if (child.count == -1)
					to_process.emplace_back(child_index);
				else if (child.count == 0)
					++num_empty;
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
	inline void QuadTree<T>::NodeInsert(const NodeReg& nr, SizeType elt_idx)
	{
		const RectFloat& rect = myElements[elt_idx].rect;
		for (const auto& leaf : FindLeaves(nr, rect))
		{
			LeafInsert(leaf, elt_idx);
		}
	}

	template<std::equality_comparable T>
	inline void QuadTree<T>::LeafInsert(const NodeReg& nr, SizeType elt_idx)
	{
		Node& node = myNodes[nr.index];
		node.firstChild = myElementsPtr.emplace(elt_idx, node.firstChild);

		if (node.count == myMaxElements && nr.depth < myMaxDepth)
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
				NodeInsert(nr, elt);
			}
		}
		else
			++node.count;
	}

	template<std::equality_comparable T>
	inline auto QuadTree<T>::FindLeaves(const NodeReg& nr, const RectFloat& rect) const -> std::vector<NodeReg>
	{
		std::vector<NodeReg> leaves;
		std::vector<NodeReg> to_process;

		to_process.emplace_back(nr);

		while (!to_process.empty())
		{
			const NodeReg nd = to_process.back();
			to_process.pop_back();

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

				if (rect.top <= nd.rect.top)
				{
					if (rect.left <= nd.rect.left)
						to_process.emplace_back(RectFloat(l, t, hx, hy), fc + 0, nd.depth + 1);
					if (rect.Right() > nd.rect.left)
						to_process.emplace_back(RectFloat(r, t, hx, hy), fc + 1, nd.depth + 1);
				}
				if (rect.Bottom() > nd.rect.top)
				{
					if (rect.left <= nd.rect.left)
						to_process.emplace_back(RectFloat(l, b, hx, hy), fc + 2, nd.depth + 1);
					if (rect.Right() > nd.rect.left)
						to_process.emplace_back(RectFloat(r, b, hx, hy), fc + 3, nd.depth + 1);
				}
			}
		}

		return leaves;
	}

	template<std::equality_comparable T>
	inline bool QuadTree<T>::IsLeaf(const Node& node)
	{
		return node.count != -1;
	}

	template<std::equality_comparable T>
	inline bool QuadTree<T>::IsBranch(const Node& node)
	{
		return node.count == -1;
	}
}
#pragma once

#include <vector>
#include <shared_mutex>
#include <concepts>

#include <CommonUtilities/Math/AABB.hpp>
#include <CommonUtilities/Math/Vector3.hpp>
#include <CommonUtilities/Math/Frustum.hpp>
#include <CommonUtilities/Math/Intersection.hpp>
#include <CommonUtilities/Math/Sphere.hpp>

#include <CommonUtilities/Structures/FreeVector.hpp>

template<typename MutexType>
class MutexHolder
{
public:
	typedef MutexType mutex_type;

	MutexHolder() = default;
	MutexHolder(const MutexHolder&) : myMutex() {}

	MutexHolder& operator=(const MutexHolder&) { return *this; }
	MutexHolder& operator=(MutexHolder&&) { return *this; }

	operator std::shared_mutex&() { return myMutex; }
	operator std::shared_mutex&() const { return myMutex; }

private:
	mutable mutex_type myMutex;
};

namespace CommonUtilities
{ 
	///	Octree based upon: https://stackoverflow.com/questions/41946007/efficient-and-well-explained-implementation-of-a-quadtree-for-2d-collision-det
	///
	template<std::equality_comparable T>
	class Octree
	{
		typedef std::shared_mutex MutexType;

	public:
		using ElementType	= T;
		using ValueType		= std::remove_const_t<T>;
		using SizeType		= int;

		static constexpr SizeType ourChildCount = 8;

		struct Element
		{		
			cu::AABBf	aabb;	// aabb encompassing the item
			ValueType	item;
		};

		Octree();

		Octree(const cu::AABBf& aRootAABB, int aMaxElements = 16, int aMaxDepth = 16);

		Octree(const Octree&) = default;
		Octree(Octree&&) = default;

		Octree& operator=(const Octree&) = default;
		Octree& operator=(Octree&&) = default;

		int ElementCount() const;

		const cu::AABBf& GetRootAABB() const;

		void SetRootAABB(const cu::AABBf& aRootAABB);

		/// Inserts given element into the quadtree.
		/// 
		/// \param AABB: Rectangle encompassing item
		/// \param Args: Constructor parameters for item
		/// 
		/// \returns Index to element, can be used to directly access it when, e.g., erasing it from the tree
		/// 
		template<typename... Args> requires std::constructible_from<T, Args...>
		auto Insert(const cu::AABBf& aAABB, Args&&... someArgs) -> SizeType;

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

		/// Retrieves the aabb encompassing item
		/// 
		/// \param Index: index to item.
		/// 
		NODISC auto GetAABB(SizeType aIndex) const -> const cu::AABBf&;

		/// Queries the tree for elements.
		/// 
		/// \param Frustum: Frustum to search for overlapping elements
		/// 
		/// \returns List of entities intersecting the frustum
		/// 
		NODISC void Query(const cu::Frustumf& aFrustum, std::vector<SizeType>& outResult) const;

		/// Queries the tree for elements.
		/// 
		/// \param Frustum: Frustum to search for overlapping elements
		/// 
		/// \returns List of entities intersecting the frustum
		/// 
		NODISC void QueryNoDepth(const cu::Frustumf& aFrustum, std::vector<SizeType>& outResult) const;

		/// Queries the tree for elements.
		/// 
		/// \param Frustum: Frustum to search for overlapping elements
		/// 
		/// \returns List of entities intersecting the frustum
		/// 
		NODISC void Query(const cu::Vector3f& aStartPos, const cu::Vector3f& aEndPos, std::vector<SizeType>& outResult) const;

		/// Queries the tree for elements.
		/// 
		/// \param AABB: Bounding box to search for overlapping elements
		/// 
		/// \returns List of entities intersecting the aabb
		/// 
		NODISC void Query(const cu::AABBf& aAABB, std::vector<SizeType>& outResult) const;

		/// Queries the tree for elements.
		/// 
		/// \param Point: Point to search for overlapping elements
		/// 
		/// \returns List of entities intersecting the point
		/// 
		NODISC void Query(const cu::Spheref& aSphere, std::vector<SizeType>& outResult) const;

		/// Queries the tree for elements.
		/// 
		/// \param Point: Point to search for overlapping elements
		/// 
		/// \returns List of entities intersecting the point
		/// 
		NODISC void Query(const cu::Vector3f& aPoint, std::vector<SizeType>& outResult) const;

		/// Performs a lazy cleanup of the tree, should be called after items have been erased.
		/// 
		void Cleanup();

		/// Clears the tree
		/// 
		void Clear();

		std::vector<cu::AABBf> GetBranchAABBs() const;

	private:
		struct Node
		{
			SizeType firstChild	 {-1};	// points to first sub-branch or first element ptr index
			SizeType count		 {0};	// -1 for branch or it's a leaf and count means number of elements
		};

		struct ElementPtr
		{
			SizeType element {-1};	// points to item in elements, not sure if even needed, seems to always be aligned anyways
			SizeType next	 {-1};	// points to next elt ptr, or -1 means end of items
		};

		struct NodeReg
		{
			cu::AABBf	aabb;
			SizeType	index {0};
			SizeType	depth {0};
		};

		struct NodeQuery
		{
			SizeType	index {0};
			bool		insideQuery {false};
		};

		struct NodeRegQuery
		{
			cu::AABBf	aabb;
			SizeType	index {0};
		};

		void NodeInsert(const NodeReg& aNode, SizeType aEltIndex);
		void LeafInsert(const NodeReg& aNode, SizeType aEltIndex);

		auto FindLeaves(const NodeReg& aNode, const cu::AABBf& aAABB) const -> std::vector<NodeReg>;

		auto QFindLeaves(const NodeRegQuery& aNode, const cu::Vector3f& aStartPos, const cu::Vector3f& aEndPos) const -> std::vector<NodeQuery>;
		auto QFindLeaves(const NodeRegQuery& aNode, const cu::Frustumf& aFrustum) const -> std::vector<NodeQuery>;
		auto QFindLeavesNoDepth(const NodeRegQuery& aNode, const cu::Frustumf& aFrustum) const -> std::vector<NodeQuery>;
		auto QFindLeaves(const NodeRegQuery& aNode, const cu::AABBf& aAABB) const -> std::vector<NodeQuery>;
		auto QFindLeaves(const NodeRegQuery& aNode, const cu::Spheref& aSphere) const -> std::vector<NodeQuery>;

		void GetNodeLeaves(SizeType aNodeIndex, std::vector<NodeQuery>& outLeaves, std::vector<SizeType>& aProcessNodes, bool aInsideQuery = false) const;

		static bool IsLeaf(const Node& aNode);
		static bool IsBranch(const Node& aNode);

		cu::FreeVector<Element>		myElements;		// all the elements
		cu::FreeVector<ElementPtr>	myElementsPtr;	// all the element ptrs
		cu::FreeVector<Node>		myNodes;

		cu::AABBf	myRootAABB;

		SizeType	myMaxElements	{16}; // max elements before subdivision
		SizeType	myMaxDepth		{8}; // max depth before no more leaves will be created

		mutable std::vector<std::uint8_t> myVisited;
		mutable MutexHolder<MutexType> myMutex = {};
	};

	template <std::equality_comparable T>
	inline Octree<T>::Octree() : Octree({Vector3f(-4096.0f), Vector3f(4096.0f) })
	{
	}

	template<std::equality_comparable T>
	inline Octree<T>::Octree(const cu::AABBf& aRootAABB, int aMaxElements, int aMaxDepth)
		: myRootAABB(aRootAABB), myMaxElements(aMaxElements), myMaxDepth(aMaxDepth)
	{
		myNodes.emplace();
	}

	template<std::equality_comparable T>
	inline int Octree<T>::ElementCount() const
	{
		return (int)myElements.count();
	}

	template<std::equality_comparable T>
	inline const cu::AABBf& Octree<T>::GetRootAABB() const
	{
		std::shared_lock<MutexType> lock(myMutex);
		return myRootAABB;
	}

	template<std::equality_comparable T>
	inline void Octree<T>::SetRootAABB(const cu::AABBf& aRootAABB)
	{
		std::scoped_lock<MutexType> lock(myMutex);

		if (myRootAABB == aRootAABB)
			return;

		myRootAABB = aRootAABB;

		if (myElements.empty())
			return;

		std::vector<NodeQuery> leaves;
		std::vector<SizeType> toProcess;

		leaves.reserve(ourChildCount * myMaxDepth / 2);
		toProcess.reserve(ourChildCount * myMaxDepth / 2);

		GetNodeLeaves(0, leaves, toProcess);

		assert(!leaves.empty() && "You have elements but no leaves?");

		myVisited.resize(myElements.size());

		std::vector<SizeType> elements;
		elements.reserve(myMaxElements * myMaxDepth * ourChildCount / 2);

		for (const auto& leaf : leaves)
		{
			const auto nodeIndex = leaf.index;

			for (auto child = myNodes[nodeIndex].firstChild; child != -1;)
			{
				const auto eltIndex	= myElementsPtr[child].element;

				if (!myVisited[eltIndex])
				{
					elements.emplace_back(eltIndex);
					myVisited[eltIndex] = true;
				}

				child = myElementsPtr[child].next;
			}
		}

		for (const auto eltIndex : elements)
		{
			myVisited[eltIndex] = false;
		}

		myElementsPtr.clear();
		myNodes.clear();

		myNodes.emplace();

		for (auto elt : elements)
		{
			NodeInsert({ myRootAABB, 0, 0 }, elt);
		}
	}

	template<std::equality_comparable T>
	template<typename... Args> requires std::constructible_from<T, Args...>
	inline auto Octree<T>::Insert(const cu::AABBf& aAABB, Args&&... someArgs) -> SizeType
	{
		std::scoped_lock<MutexType> lock(myMutex);

		if (!myRootAABB.Overlaps(aAABB)) // dont attempt to add if outside boundary
			return -1;

		const auto aIndex = (SizeType)myElements.emplace(aAABB, std::forward<Args>(someArgs)...);
		NodeInsert({ myRootAABB, 0, 0 }, aIndex);

		return aIndex;
	}

	template<std::equality_comparable T>
	inline bool Octree<T>::Erase(SizeType aIndex)
	{
		std::scoped_lock<MutexType> lock(myMutex);

		const cu::AABBf& aabb	= myElements[aIndex].aabb;
		const auto leaves		= QFindLeaves({ myRootAABB, 0 }, aabb);

		if (leaves.empty())
			return false;

		for (const auto& leaf : leaves)
		{
			Node& node = myNodes[leaf.index];

			auto ptrIndex	= node.firstChild;
			auto prvIdx		= -1;

			while (ptrIndex != -1 && myElementsPtr[ptrIndex].element != aIndex)
			{
				prvIdx		= ptrIndex;
				ptrIndex	= myElementsPtr[ptrIndex].next;
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

				assert(node.count >= 0 && "Node cannot have a negative count of elements");
			}
		}

		myElements.erase(aIndex);

		return true;
	}

	template<std::equality_comparable T>
	template<typename... Args> requires std::constructible_from<T, Args...>
	inline bool Octree<T>::Update(SizeType aIndex, Args&&... someArgs)
	{
		std::shared_lock<MutexType> lock(myMutex);

		if (aIndex >= myElements.size() || !myElements.valid(aIndex))
			return false;

		myElements[aIndex].item = T{std::forward<Args>(someArgs)...};

		return true;
	}

	template<std::equality_comparable T>
	auto Octree<T>::Get(SizeType aIndex) -> ValueType&
	{
		std::shared_lock<MutexType> lock(myMutex);
		return myElements[aIndex].item;
	}

	template<std::equality_comparable T>
	auto Octree<T>::Get(SizeType aIndex) const -> const ValueType&
	{
		std::shared_lock<MutexType> lock(myMutex);
		return myElements[aIndex].item;
	}

	template<std::equality_comparable T>
	inline auto Octree<T>::GetAABB(SizeType aIndex) const -> const cu::AABBf&
	{
		std::shared_lock<MutexType> lock(myMutex);
		return myElements[aIndex].aabb;
	}

	template<std::equality_comparable T>
	inline void Octree<T>::Query(const cu::Frustumf& aFrustum, std::vector<SizeType>& outResult) const
	{
		std::scoped_lock<MutexType> lock(myMutex);

		outResult.clear();

		if (myNodes.empty())
			return;

		myVisited.resize(myElements.size());

		for (NodeQuery leaf : QFindLeaves({ myRootAABB, 0 }, aFrustum))
		{
			const SizeType nodeIndex = leaf.index;

			if (leaf.insideQuery)
			{
				for (SizeType child = myNodes[nodeIndex].firstChild; child != -1;)
				{
					const SizeType eltIndex = myElementsPtr[child].element;

					if (!myVisited[eltIndex])
					{
						outResult.emplace_back(eltIndex);
						myVisited[eltIndex] = true;
					}

					child = myElementsPtr[child].next;
				}
			}
			else
			{ 
				for (SizeType child = myNodes[nodeIndex].firstChild; child != -1;)
				{
					const SizeType eltIndex = myElementsPtr[child].element;
					const Element& elt		= myElements[eltIndex];

					if (!myVisited[eltIndex] && aFrustum.IsInside(elt.aabb))
					{
						outResult.emplace_back(eltIndex);
						myVisited[eltIndex] = true;
					}

					child = myElementsPtr[child].next;
				}
			}
		}

		for (const SizeType eltIndex : outResult)
		{
			myVisited[eltIndex] = false;
		}
	}

	template<std::equality_comparable T>
	inline void Octree<T>::QueryNoDepth(const cu::Frustumf& aFrustum, std::vector<SizeType>& outResult) const
	{
		std::scoped_lock<MutexType> lock(myMutex);

		outResult.clear();

		if (myNodes.empty())
			return;

		myVisited.resize(myElements.size());

		for (NodeQuery leaf : QFindLeavesNoDepth({ myRootAABB, 0 }, aFrustum))
		{
			const SizeType nodeIndex = leaf.index;

			if (leaf.insideQuery)
			{
				for (SizeType child = myNodes[nodeIndex].firstChild; child != -1;)
				{
					const SizeType eltIndex = myElementsPtr[child].element;

					if (!myVisited[eltIndex])
					{
						outResult.emplace_back(eltIndex);
						myVisited[eltIndex] = true;
					}

					child = myElementsPtr[child].next;
				}
			}
			else
			{
				for (SizeType child = myNodes[nodeIndex].firstChild; child != -1;)
				{
					const SizeType eltIndex = myElementsPtr[child].element;
					const Element& elt		= myElements[eltIndex];

					if (!myVisited[eltIndex] && aFrustum.IsInsideNoDepth(elt.aabb))
					{
						outResult.emplace_back(eltIndex);
						myVisited[eltIndex] = true;
					}

					child = myElementsPtr[child].next;
				}
			}
		}

		for (const SizeType eltIndex : outResult)
		{
			myVisited[eltIndex] = false;
		}
	}

	template<std::equality_comparable T>
	inline void Octree<T>::Query(const cu::Vector3f& aStartPos, const cu::Vector3f& aEndPos, std::vector<SizeType>& outResult) const
	{
		std::scoped_lock<MutexType> lock(myMutex);

		outResult.clear();

		if (myNodes.empty())
			return;

		myVisited.resize(myElements.size());

		for (NodeQuery leaf : QFindLeaves({ myRootAABB, 0 }, aStartPos, aEndPos))
		{
			const SizeType nodeIndex = leaf.index;
			
			if (leaf.insideQuery)
			{
				for (SizeType child = myNodes[nodeIndex].firstChild; child != -1;)
				{
					const SizeType eltIndex	= myElementsPtr[child].element;

					if (!myVisited[eltIndex])
					{
						outResult.emplace_back(eltIndex);
						myVisited[eltIndex] = true;
					}

					child = myElementsPtr[child].next;
				}
			}
			else
			{
				for (SizeType child = myNodes[nodeIndex].firstChild; child != -1;)
				{
					const SizeType eltIndex	= myElementsPtr[child].element;
					const Element& elt		= myElements[eltIndex];

					if (!myVisited[eltIndex] && cu::IntersectionAABBSegment(elt.aabb, aStartPos, aEndPos))
					{
						outResult.emplace_back(eltIndex);
						myVisited[eltIndex] = true;
					}

					child = myElementsPtr[child].next;
				}
			}
		}

		for (const SizeType eltIndex : outResult)
		{
			myVisited[eltIndex] = false;
		}
	}

	template<std::equality_comparable T>
	inline void Octree<T>::Query(const cu::AABBf& aAABB, std::vector<SizeType>& outResult) const
	{
		std::scoped_lock<MutexType> lock(myMutex);

		outResult.clear();

		if (myNodes.empty())
			return;

		myVisited.resize(myElements.size());

		for (NodeQuery leaf : QFindLeaves({ myRootAABB, 0 }, aAABB))
		{
			const SizeType nodeIndex = leaf.index;
			
			if (leaf.insideQuery)
			{
				for (SizeType child = myNodes[nodeIndex].firstChild; child != -1;)
				{
					const SizeType eltIndex	= myElementsPtr[child].element;

					if (!myVisited[eltIndex])
					{
						outResult.emplace_back(eltIndex);
						myVisited[eltIndex] = true;
					}

					child = myElementsPtr[child].next;
				}
			}
			else
			{
				for (SizeType child = myNodes[nodeIndex].firstChild; child != -1;)
				{
					const SizeType eltIndex	= myElementsPtr[child].element;
					const Element& elt		= myElements[eltIndex];

					if (!myVisited[eltIndex] && elt.aabb.Overlaps(aAABB))
					{
						outResult.emplace_back(eltIndex);
						myVisited[eltIndex] = true;
					}

					child = myElementsPtr[child].next;
				}
			}
		}

		for (const SizeType eltIndex : outResult)
		{
			myVisited[eltIndex] = false;
		}
	}

	template<std::equality_comparable T>
	inline void Octree<T>::Query(const cu::Spheref& aSphere, std::vector<SizeType>& outResult) const
	{
		std::scoped_lock<MutexType> lock(myMutex);

		outResult.clear();

		if (myNodes.empty())
			return;

		myVisited.resize(myElements.size());

		for (NodeQuery leaf : QFindLeaves({ myRootAABB, 0 }, aSphere))
		{
			const SizeType nodeIndex = leaf.index;
			
			if (leaf.insideQuery)
			{
				for (SizeType child = myNodes[nodeIndex].firstChild; child != -1;)
				{
					const SizeType eltIndex	= myElementsPtr[child].element;

					if (!myVisited[eltIndex])
					{
						outResult.emplace_back(eltIndex);
						myVisited[eltIndex] = true;
					}

					child = myElementsPtr[child].next;
				}
			}
			else
			{
				for (SizeType child = myNodes[nodeIndex].firstChild; child != -1;)
				{
					const SizeType eltIndex	= myElementsPtr[child].element;
					const Element& elt		= myElements[eltIndex];

					if (!myVisited[eltIndex] && aSphere.Overlaps(elt.aabb))
					{
						outResult.emplace_back(eltIndex);
						myVisited[eltIndex] = true;
					}

					child = myElementsPtr[child].next;
				}
			}
		}

		for (const SizeType eltIndex : outResult)
		{
			myVisited[eltIndex] = false;
		}
	}

	template<std::equality_comparable T>
	inline void Octree<T>::Query(const cu::Vector3f& aPoint, std::vector<SizeType>& outResult) const
	{
		Query(cu::AABBf(aPoint, aPoint), outResult);
	}

	template<std::equality_comparable T>
	inline void Octree<T>::Cleanup()
	{
		std::scoped_lock<MutexType> lock(myMutex);

		if (myNodes.empty())
			return;

		std::vector<SizeType> toProcess;
		toProcess.reserve(ourChildCount * myMaxDepth / 2);

		if (IsBranch(myNodes[0]))
		{
			toProcess.emplace_back(0); // push root
		}

		while (!toProcess.empty())
		{
			Node& node = myNodes[toProcess.back()];
			toProcess.pop_back();

			int numEmpty = 0;
			for (int i = 0; i < ourChildCount; ++i)
			{
				const int childIndex	= node.firstChild + i;
				const Node& child		= myNodes[childIndex];

				if (IsBranch(child))
				{
					toProcess.emplace_back(childIndex);
				}
				else if (child.count == 0)
				{
					++numEmpty;
				}
			}

			if (numEmpty == ourChildCount)
			{
				for (int i = ourChildCount - 1; i >= 0; --i)
				{
					myNodes.erase(node.firstChild + i);
				}

				node = {}; // reset
			}
		}
	}

	template<std::equality_comparable T>
	inline void Octree<T>::Clear()
	{
		std::scoped_lock<MutexType> lock(myMutex);

		myElements.clear();
		myElementsPtr.clear();
		myNodes.clear();
	}

	template<std::equality_comparable T>
	inline std::vector<cu::AABBf> Octree<T>::GetBranchAABBs() const
	{
		std::shared_lock<MutexType> lock(myMutex);

		std::vector<cu::AABBf> result;

		if (myNodes.empty())
			return result;

		std::vector<NodeRegQuery> toProcess;
		toProcess.reserve(ourChildCount * myMaxDepth / 2);

		toProcess.emplace_back(myRootAABB, 0);

		while (!toProcess.empty())
		{
			const auto nd = toProcess.back();
			toProcess.pop_back();

			result.emplace_back(nd.aabb);

			if (IsBranch(myNodes[nd.index]))
			{
				const auto fc = myNodes[nd.index].firstChild;

				const auto c	= nd.aabb.GetCenter();
				const auto hs	= nd.aabb.GetExtends();
				const auto l	= nd.aabb.GetMin().x;
				const auto b	= nd.aabb.GetMin().y;
				const auto h	= nd.aabb.GetMin().z;
				const auto r	= l + hs.x;
				const auto t	= b + hs.y;
				const auto f	= h + hs.z;

				auto b1 = cu::AABBf(r, t, f, r + hs.x, t + hs.y, f + hs.z);
				auto b2 = cu::AABBf(l, t, f, l + hs.x, t + hs.y, f + hs.z);
				auto b3 = cu::AABBf(r, b, f, r + hs.x, b + hs.y, f + hs.z);
				auto b4 = cu::AABBf(l, b, f, l + hs.x, b + hs.y, f + hs.z);
				auto b5 = cu::AABBf(r, t, h, r + hs.x, t + hs.y, h + hs.z);
				auto b6 = cu::AABBf(l, t, h, l + hs.x, t + hs.y, h + hs.z);
				auto b7 = cu::AABBf(r, b, h, r + hs.x, b + hs.y, h + hs.z);
				auto b8 = cu::AABBf(l, b, h, l + hs.x, b + hs.y, h + hs.z);

				toProcess.emplace_back(b1, fc + 0);
				toProcess.emplace_back(b2, fc + 1);
				toProcess.emplace_back(b3, fc + 2);
				toProcess.emplace_back(b4, fc + 3);
				toProcess.emplace_back(b5, fc + 4);
				toProcess.emplace_back(b6, fc + 5);
				toProcess.emplace_back(b7, fc + 6);
				toProcess.emplace_back(b8, fc + 7);
			}
		}

		return result;
	}

	template<std::equality_comparable T>
	inline void Octree<T>::NodeInsert(const NodeReg& aNode, SizeType aEltIndex)
	{
		const cu::AABBf& aabb	= myElements[aEltIndex].aabb;
		auto leaves				= FindLeaves(aNode, aabb);

		for (const auto& leaf : leaves)
		{
			LeafInsert(leaf, aEltIndex);
		}
	}

	template<std::equality_comparable T>
	inline void Octree<T>::LeafInsert(const NodeReg& aNode, SizeType aEltIndex)
	{
		Node* node = &myNodes[aNode.index];

		node->firstChild = (SizeType)myElementsPtr.emplace(aEltIndex, node->firstChild);

		if (node->count == myMaxElements && aNode.depth < myMaxDepth && aNode.aabb.Contains(myElements[aEltIndex].aabb)) // nodes that are smaller than the elements aabb cannot get divided
		{
			std::vector<SizeType> elements;
			elements.reserve(myMaxElements);

			while (node->firstChild != -1)
			{
				const auto index = node->firstChild;

				const auto next = myElementsPtr[index].next;
				const auto elt = myElementsPtr[index].element;

				node->firstChild = next;
				myElementsPtr.erase(index);

				elements.emplace_back(elt);
			}

			const auto fc = myNodes.emplace();
			for (int i = 0; i < ourChildCount - 1; ++i)
			{
				myNodes.emplace();
			}

			node = &myNodes[aNode.index];

			node->firstChild	= (SizeType)fc;
			node->count			= -1; // set as branch

			for (const auto elt : elements)
			{
				NodeInsert(aNode, elt);
			}
		}
		else
			++node->count;
	}

	template<std::equality_comparable T>
	inline auto Octree<T>::FindLeaves(const NodeReg& aNode, const cu::AABBf& aAABB) const -> std::vector<NodeReg>
	{
		std::vector<NodeReg>	leaves;
		std::vector<NodeReg>	toProcess;
		std::vector<SizeType>	toProcessContained;

		leaves.reserve(ourChildCount / 2);
		toProcess.reserve(ourChildCount * myMaxDepth / 2);
		toProcessContained.reserve(ourChildCount * myMaxDepth / 4);

		toProcess.emplace_back(aNode);

		while (!toProcess.empty())
		{
			const auto nd = toProcess.back();
			toProcess.pop_back();

			if (IsLeaf(myNodes[nd.index]))
			{
				leaves.emplace_back(nd);
			}
			else
			{
				const auto fc	= myNodes[nd.index].firstChild;

				const auto c	= nd.aabb.GetCenter();
				const auto hs	= nd.aabb.GetExtends();
				const auto l	= nd.aabb.GetMin().x;
				const auto b	= nd.aabb.GetMin().y;
				const auto h	= nd.aabb.GetMin().z;
				const auto r	= l + hs.x;
				const auto t	= b + hs.y;
				const auto f	= h + hs.z;

				if (aAABB.GetMax().z >= c.z)
				{
					if (aAABB.GetMax().y >= c.y)
					{
						if (aAABB.GetMax().x >= c.x)
						{
							toProcess.emplace_back(cu::AABBf(r, t, f, r + hs.x, t + hs.y, f + hs.z), fc + 0, nd.depth + 1);
						}
						if (aAABB.GetMin().x < c.x)
						{
							toProcess.emplace_back(cu::AABBf(l, t, f, l + hs.x, t + hs.y, f + hs.z), fc + 1, nd.depth + 1);
						}
					}
					if (aAABB.GetMin().y < c.y)
					{
						if (aAABB.GetMax().x >= c.x)
						{
							toProcess.emplace_back(cu::AABBf(r, b, f, r + hs.x, b + hs.y, f + hs.z), fc + 2, nd.depth + 1);
						}
						if (aAABB.GetMin().x < c.x)
						{
							toProcess.emplace_back(cu::AABBf(l, b, f, l + hs.x, b + hs.y, f + hs.z), fc + 3, nd.depth + 1);
						}
					}
				}
				if (aAABB.GetMin().z < c.z)
				{
					if (aAABB.GetMax().y >= c.y)
					{
						if (aAABB.GetMax().x >= c.x)
						{
							toProcess.emplace_back(cu::AABBf(r, t, h, r + hs.x, t + hs.y, h + hs.z), fc + 4, nd.depth + 1);
						}
						if (aAABB.GetMin().x < c.x)
						{
							toProcess.emplace_back(cu::AABBf(l, t, h, l + hs.x, t + hs.y, h + hs.z), fc + 5, nd.depth + 1);
						}
					}
					if (aAABB.GetMin().y < c.y)
					{
						if (aAABB.GetMax().x >= c.x)
						{
							toProcess.emplace_back(cu::AABBf(r, b, h, r + hs.x, b + hs.y, h + hs.z), fc + 6, nd.depth + 1);
						}
						if (aAABB.GetMin().x < c.x)
						{
							toProcess.emplace_back(cu::AABBf(l, b, h, l + hs.x, b + hs.y, h + hs.z), fc + 7, nd.depth + 1);
						}
					}
				}
			}
		}

		return leaves;
	}

	template<std::equality_comparable T>
	inline auto Octree<T>::QFindLeaves(const NodeRegQuery& aNode, const cu::Vector3f& aStartPos, const cu::Vector3f& aEndPos) const -> std::vector<NodeQuery>
	{
		std::vector<NodeQuery> leaves;
		std::vector<NodeRegQuery> toProcess;

		leaves.reserve(ourChildCount / 2);
		toProcess.reserve(ourChildCount * myMaxDepth / 2);

		toProcess.emplace_back(aNode);

		while (!toProcess.empty())
		{
			const auto nd = toProcess.back();
			toProcess.pop_back();

			if (IsLeaf(myNodes[nd.index]))
			{
				leaves.emplace_back(nd.index);
			}
			else
			{
				const auto fc	= myNodes[nd.index].firstChild;

				const auto c	= nd.aabb.GetCenter();
				const auto hs	= nd.aabb.GetExtends();
				const auto l	= nd.aabb.GetMin().x;
				const auto b	= nd.aabb.GetMin().y;
				const auto h	= nd.aabb.GetMin().z;
				const auto r	= l + hs.x;
				const auto t	= b + hs.y;
				const auto f	= h + hs.z;

				const auto b1 = cu::AABBf(r, t, f, r + hs.x, t + hs.y, f + hs.z);
				const auto b2 = cu::AABBf(l, t, f, l + hs.x, t + hs.y, f + hs.z);
				const auto b3 = cu::AABBf(r, b, f, r + hs.x, b + hs.y, f + hs.z);
				const auto b4 = cu::AABBf(l, b, f, l + hs.x, b + hs.y, f + hs.z);
				const auto b5 = cu::AABBf(r, t, h, r + hs.x, t + hs.y, h + hs.z);
				const auto b6 = cu::AABBf(l, t, h, l + hs.x, t + hs.y, h + hs.z);
				const auto b7 = cu::AABBf(r, b, h, r + hs.x, b + hs.y, h + hs.z);
				const auto b8 = cu::AABBf(l, b, h, l + hs.x, b + hs.y, h + hs.z);

				if (cu::IntersectionAABBSegment(b1, aStartPos, aEndPos)) toProcess.emplace_back(b1, fc + 0);
				if (cu::IntersectionAABBSegment(b2, aStartPos, aEndPos)) toProcess.emplace_back(b2, fc + 1);
				if (cu::IntersectionAABBSegment(b3, aStartPos, aEndPos)) toProcess.emplace_back(b3, fc + 2);
				if (cu::IntersectionAABBSegment(b4, aStartPos, aEndPos)) toProcess.emplace_back(b4, fc + 3);
				if (cu::IntersectionAABBSegment(b5, aStartPos, aEndPos)) toProcess.emplace_back(b5, fc + 4);
				if (cu::IntersectionAABBSegment(b6, aStartPos, aEndPos)) toProcess.emplace_back(b6, fc + 5);
				if (cu::IntersectionAABBSegment(b7, aStartPos, aEndPos)) toProcess.emplace_back(b7, fc + 6);
				if (cu::IntersectionAABBSegment(b8, aStartPos, aEndPos)) toProcess.emplace_back(b8, fc + 7);
			}
		}

		return leaves;
	}

	template<std::equality_comparable T>
	inline auto Octree<T>::QFindLeaves(const NodeRegQuery& aNode, const cu::Frustumf& aFrustum) const -> std::vector<NodeQuery>
	{
		std::vector<NodeQuery>		leaves;
		std::vector<NodeRegQuery>	toProcess;
		std::vector<SizeType>		toProcessContained;

		leaves.reserve(ourChildCount / 2);
		toProcess.reserve(ourChildCount * myMaxDepth / 2);
		toProcessContained.reserve(ourChildCount * myMaxDepth / 4);

		toProcess.emplace_back(aNode);

		while (!toProcess.empty())
		{
			const auto nd = toProcess.back();
			toProcess.pop_back();

			if (IsLeaf(myNodes[nd.index]))
			{
				leaves.emplace_back(nd.index);
			}
			else 
			{
				if (aFrustum.Contains(nd.aabb))
				{
					GetNodeLeaves(nd.index, leaves, toProcessContained, true);
				}
				else
				{ 
					const auto fc = myNodes[nd.index].firstChild;

					const auto c	= nd.aabb.GetCenter();
					const auto hs	= nd.aabb.GetExtends();
					const auto l	= nd.aabb.GetMin().x;
					const auto b	= nd.aabb.GetMin().y;
					const auto h	= nd.aabb.GetMin().z;
					const auto r	= l + hs.x;
					const auto t	= b + hs.y;
					const auto f	= h + hs.z;

					const auto b1 = cu::AABBf(r, t, f, r + hs.x, t + hs.y, f + hs.z);
					const auto b2 = cu::AABBf(l, t, f, l + hs.x, t + hs.y, f + hs.z);
					const auto b3 = cu::AABBf(r, b, f, r + hs.x, b + hs.y, f + hs.z);
					const auto b4 = cu::AABBf(l, b, f, l + hs.x, b + hs.y, f + hs.z);
					const auto b5 = cu::AABBf(r, t, h, r + hs.x, t + hs.y, h + hs.z);
					const auto b6 = cu::AABBf(l, t, h, l + hs.x, t + hs.y, h + hs.z);
					const auto b7 = cu::AABBf(r, b, h, r + hs.x, b + hs.y, h + hs.z);
					const auto b8 = cu::AABBf(l, b, h, l + hs.x, b + hs.y, h + hs.z);

					if (aFrustum.IsInside(b1)) toProcess.emplace_back(b1, fc + 0);
					if (aFrustum.IsInside(b2)) toProcess.emplace_back(b2, fc + 1);
					if (aFrustum.IsInside(b3)) toProcess.emplace_back(b3, fc + 2);
					if (aFrustum.IsInside(b4)) toProcess.emplace_back(b4, fc + 3);
					if (aFrustum.IsInside(b5)) toProcess.emplace_back(b5, fc + 4);
					if (aFrustum.IsInside(b6)) toProcess.emplace_back(b6, fc + 5);
					if (aFrustum.IsInside(b7)) toProcess.emplace_back(b7, fc + 6);
					if (aFrustum.IsInside(b8)) toProcess.emplace_back(b8, fc + 7);
				}
			}
		}

		return leaves;
	}
	template<std::equality_comparable T>
	inline auto Octree<T>::QFindLeavesNoDepth(const NodeRegQuery& aNode, const cu::Frustumf& aFrustum) const -> std::vector<NodeQuery>
	{
		std::vector<NodeQuery>		leaves;
		std::vector<NodeRegQuery>	toProcess;
		std::vector<SizeType>		toProcessContained;

		leaves.reserve(ourChildCount / 2);
		toProcess.reserve(ourChildCount * myMaxDepth / 2);
		toProcessContained.reserve(ourChildCount * myMaxDepth / 4);

		toProcess.emplace_back(aNode);

		while (!toProcess.empty())
		{
			const auto nd = toProcess.back();
			toProcess.pop_back();

			if (IsLeaf(myNodes[nd.index]))
			{
				leaves.emplace_back(nd.index);
			}
			else // TODO: optimize further if this becomes a bottleneck
			{
				if (aFrustum.Contains(nd.aabb))
				{
					GetNodeLeaves(nd.index, leaves, toProcessContained, true);
				}
				else
				{
					const auto fc = myNodes[nd.index].firstChild;

					const auto c	= nd.aabb.GetCenter();
					const auto hs	= nd.aabb.GetExtends();
					const auto l	= nd.aabb.GetMin().x;
					const auto b	= nd.aabb.GetMin().y;
					const auto h	= nd.aabb.GetMin().z;
					const auto r	= l + hs.x;
					const auto t	= b + hs.y;
					const auto f	= h + hs.z;

					const auto b1 = cu::AABBf(r, t, f, r + hs.x, t + hs.y, f + hs.z);
					const auto b2 = cu::AABBf(l, t, f, l + hs.x, t + hs.y, f + hs.z);
					const auto b3 = cu::AABBf(r, b, f, r + hs.x, b + hs.y, f + hs.z);
					const auto b4 = cu::AABBf(l, b, f, l + hs.x, b + hs.y, f + hs.z);
					const auto b5 = cu::AABBf(r, t, h, r + hs.x, t + hs.y, h + hs.z);
					const auto b6 = cu::AABBf(l, t, h, l + hs.x, t + hs.y, h + hs.z);
					const auto b7 = cu::AABBf(r, b, h, r + hs.x, b + hs.y, h + hs.z);
					const auto b8 = cu::AABBf(l, b, h, l + hs.x, b + hs.y, h + hs.z);

					if (aFrustum.IsInsideNoDepth(b1)) toProcess.emplace_back(b1, fc + 0);
					if (aFrustum.IsInsideNoDepth(b2)) toProcess.emplace_back(b2, fc + 1);
					if (aFrustum.IsInsideNoDepth(b3)) toProcess.emplace_back(b3, fc + 2);
					if (aFrustum.IsInsideNoDepth(b4)) toProcess.emplace_back(b4, fc + 3);
					if (aFrustum.IsInsideNoDepth(b5)) toProcess.emplace_back(b5, fc + 4);
					if (aFrustum.IsInsideNoDepth(b6)) toProcess.emplace_back(b6, fc + 5);
					if (aFrustum.IsInsideNoDepth(b7)) toProcess.emplace_back(b7, fc + 6);
					if (aFrustum.IsInsideNoDepth(b8)) toProcess.emplace_back(b8, fc + 7);
				}
			}
		}

		return leaves;
	}
	template<std::equality_comparable T>
	inline auto Octree<T>::QFindLeaves(const NodeRegQuery& aNode, const cu::AABBf& aAABB) const -> std::vector<NodeQuery>
	{
		std::vector<NodeQuery>		leaves;
		std::vector<NodeRegQuery>	toProcess;
		std::vector<SizeType>		toProcessContained;

		leaves.reserve(ourChildCount / 2);
		toProcess.reserve(ourChildCount * myMaxDepth / 2);
		toProcessContained.reserve(ourChildCount * myMaxDepth / 4);

		toProcess.emplace_back(aNode);

		while (!toProcess.empty())
		{
			const auto nd = toProcess.back();
			toProcess.pop_back();

			if (IsLeaf(myNodes[nd.index]))
			{
				leaves.emplace_back(nd.index);
			}
			else
			{
				if (aAABB.Contains(nd.aabb))
				{
					GetNodeLeaves(nd.index, leaves, toProcessContained, true);
				}
				else
				{
					const auto fc	= myNodes[nd.index].firstChild;

					const auto c	= nd.aabb.GetCenter();
					const auto hs	= nd.aabb.GetExtends();
					const auto l	= nd.aabb.GetMin().x;
					const auto b	= nd.aabb.GetMin().y;
					const auto h	= nd.aabb.GetMin().z;
					const auto r	= l + hs.x;
					const auto t	= b + hs.y;
					const auto f	= h + hs.z;

					if (aAABB.GetMax().z >= c.z)
					{
						if (aAABB.GetMax().y >= c.y)
						{
							if (aAABB.GetMax().x >= c.x)
							{
								toProcess.emplace_back(cu::AABBf(r, t, f, r + hs.x, t + hs.y, f + hs.z), fc + 0);
							}
							if (aAABB.GetMin().x < c.x)
							{
								toProcess.emplace_back(cu::AABBf(l, t, f, l + hs.x, t + hs.y, f + hs.z), fc + 1);
							}
						}
						if (aAABB.GetMin().y < c.y)
						{
							if (aAABB.GetMax().x >= c.x)
							{
								toProcess.emplace_back(cu::AABBf(r, b, f, r + hs.x, b + hs.y, f + hs.z), fc + 2);
							}
							if (aAABB.GetMin().x < c.x)
							{
								toProcess.emplace_back(cu::AABBf(l, b, f, l + hs.x, b + hs.y, f + hs.z), fc + 3);
							}
						}
					}
					if (aAABB.GetMin().z < c.z)
					{
						if (aAABB.GetMax().y >= c.y)
						{
							if (aAABB.GetMax().x >= c.x)
							{
								toProcess.emplace_back(cu::AABBf(r, t, h, r + hs.x, t + hs.y, h + hs.z), fc + 4);
							}
							if (aAABB.GetMin().x < c.x)
							{
								toProcess.emplace_back(cu::AABBf(l, t, h, l + hs.x, t + hs.y, h + hs.z), fc + 5);
							}
						}
						if (aAABB.GetMin().y < c.y)
						{
							if (aAABB.GetMax().x >= c.x)
							{
								toProcess.emplace_back(cu::AABBf(r, b, h, r + hs.x, b + hs.y, h + hs.z), fc + 6);
							}
							if (aAABB.GetMin().x < c.x)
							{
								toProcess.emplace_back(cu::AABBf(l, b, h, l + hs.x, b + hs.y, h + hs.z), fc + 7);
							}
						}
					}
				}
			}
		}

		return leaves;
	}

	template<std::equality_comparable T>
	inline auto Octree<T>::QFindLeaves(const NodeRegQuery& aNode, const cu::Spheref& aSphere) const -> std::vector<NodeQuery>
	{
		std::vector<NodeQuery> leaves;
		std::vector<NodeRegQuery> toProcess;

		leaves.reserve(ourChildCount / 2);
		toProcess.reserve(ourChildCount * myMaxDepth / 2);

		toProcess.emplace_back(aNode);

		while (!toProcess.empty())
		{
			const auto nd = toProcess.back();
			toProcess.pop_back();

			if (IsLeaf(myNodes[nd.index]))
			{
				leaves.emplace_back(nd.index);
			}
			else
			{
				const auto fc = myNodes[nd.index].firstChild;

				const auto c	= nd.aabb.GetCenter();
				const auto hs	= nd.aabb.GetExtends();
				const auto l	= nd.aabb.GetMin().x;
				const auto b	= nd.aabb.GetMin().y;
				const auto h	= nd.aabb.GetMin().z;
				const auto r	= l + hs.x;
				const auto t	= b + hs.y;
				const auto f	= h + hs.z;

				const auto b1 = cu::AABBf(r, t, f, r + hs.x, t + hs.y, f + hs.z);
				const auto b2 = cu::AABBf(l, t, f, l + hs.x, t + hs.y, f + hs.z);
				const auto b3 = cu::AABBf(r, b, f, r + hs.x, b + hs.y, f + hs.z);
				const auto b4 = cu::AABBf(l, b, f, l + hs.x, b + hs.y, f + hs.z);
				const auto b5 = cu::AABBf(r, t, h, r + hs.x, t + hs.y, h + hs.z);
				const auto b6 = cu::AABBf(l, t, h, l + hs.x, t + hs.y, h + hs.z);
				const auto b7 = cu::AABBf(r, b, h, r + hs.x, b + hs.y, h + hs.z);
				const auto b8 = cu::AABBf(l, b, h, l + hs.x, b + hs.y, h + hs.z);

				if (aSphere.Overlaps(b1)) toProcess.emplace_back(b1, fc + 0);
				if (aSphere.Overlaps(b2)) toProcess.emplace_back(b2, fc + 1);
				if (aSphere.Overlaps(b3)) toProcess.emplace_back(b3, fc + 2);
				if (aSphere.Overlaps(b4)) toProcess.emplace_back(b4, fc + 3);
				if (aSphere.Overlaps(b5)) toProcess.emplace_back(b5, fc + 4);
				if (aSphere.Overlaps(b6)) toProcess.emplace_back(b6, fc + 5);
				if (aSphere.Overlaps(b7)) toProcess.emplace_back(b7, fc + 6);
				if (aSphere.Overlaps(b8)) toProcess.emplace_back(b8, fc + 7);
			}
		}

		return leaves;
	}

	template<std::equality_comparable T>
	inline void Octree<T>::GetNodeLeaves(SizeType aNodeIndex, std::vector<NodeQuery>& outLeaves, std::vector<SizeType>& aProcessNodes, bool aInsideQuery) const
	{
		aProcessNodes.clear();

		aProcessNodes.emplace_back(aNodeIndex);

		while (!aProcessNodes.empty())
		{
			const SizeType index = aProcessNodes.back();
			aProcessNodes.pop_back();

			if (IsLeaf(myNodes[index]))
			{
				outLeaves.emplace_back(index, aInsideQuery);
			}
			else
			{
				const SizeType fc = myNodes[index].firstChild;

				aProcessNodes.emplace_back(fc + 0);
				aProcessNodes.emplace_back(fc + 1);
				aProcessNodes.emplace_back(fc + 2);
				aProcessNodes.emplace_back(fc + 3);
				aProcessNodes.emplace_back(fc + 4);
				aProcessNodes.emplace_back(fc + 5);
				aProcessNodes.emplace_back(fc + 6);
				aProcessNodes.emplace_back(fc + 7);
			}
		}
	}

	template<std::equality_comparable T>
	inline bool Octree<T>::IsLeaf(const Node& aNode)
	{
		return aNode.count != -1;
	}
	template<std::equality_comparable T>
	inline bool Octree<T>::IsBranch(const Node& aNode)
	{
		return aNode.count == -1;
	}
}
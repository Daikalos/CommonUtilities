#pragma once

#include <vector>
#include <unordered_set>
#include <cmath>

#include "PriorityQueue.hpp"

namespace CommonUtilities
{
	static const int MapWidth = 20;
	static const int MapHeight = 20;
	static const int TileCount = MapWidth * MapHeight;

	enum class Tile
	{
		Impassable,
		Passable
	};

	std::vector<int> Dijkstra(const std::vector<Tile>& aMap, int aStartIndex, int aEndIndex);

	inline std::vector<int> Dijkstra(const std::vector<Tile>& aMap, int aStartIndex, int aEndIndex) // TODO: move to cpp later
	{
		struct Node
		{
			int index;
			int cost;
			int parent;

			bool operator<(const Node& aOther) const
			{
				return cost < aOther.cost;
			}
		};

		struct Point
		{
			int x;
			int y;
		};

		const auto PosToIndex = [](int aX, int aY) { return aX + aY * MapWidth; };
		const auto IndexToPos = [](int aIndex) { return Point(aIndex % MapWidth, aIndex / MapWidth); };

		PriorityQueue<Node, pq::HeapType::Min> open;
		std::unordered_set<int> visited;

		std::vector<int> parents;
		parents.resize(aMap.size());

		open.Emplace(aStartIndex, 0, INT_MAX);

		const auto CheckNeighbour = 
			[&aMap, &open](int aNeighbourIndex, int aCurrentIndex, int aCurrentCost)
			{					
				if (aNeighbourIndex < 0 || aNeighbourIndex >= (int)aMap.size()) // check if outside bounds
					return;

				Tile neighbour = aMap[aNeighbourIndex];

				if (neighbour == Tile::Passable)
				{
					open.Emplace(aNeighbourIndex, aCurrentCost + 1, aCurrentIndex); // just add 1 because we're in a grid
				}
			};

		while (!open.IsEmpty())
		{
			Node current = open.Top();
			open.Pop();

			auto pair = visited.emplace(current.index);

			if (!pair.second) // check if already visited
				continue;

			parents[current.index] = current.parent;

			if (current.index == aEndIndex) // we found a path
			{
				std::vector<int> path;

				int currentIndex = aEndIndex;

				while (currentIndex != aStartIndex)
				{
					path.emplace_back(currentIndex);
					currentIndex = parents[currentIndex];
				}

				path.emplace_back(aStartIndex);
				std::reverse(path.begin(), path.end());

				return path;
			}

			const Point currentPos = IndexToPos(current.index);

			for (int y = -1; y <= 1; y += 2)
			{
				Point neighbourPos = Point{ currentPos.x, currentPos.y + y };
				int neighbourIndex = PosToIndex(neighbourPos.x, neighbourPos.y);

				CheckNeighbour(neighbourIndex, current.index, current.cost);
			}

			for (int x = -1; x <= 1; x += 2)
			{
				Point neighbourPos = Point{ currentPos.x + x, currentPos.y };
				int neighbourIndex = PosToIndex(neighbourPos.x, neighbourPos.y);

				CheckNeighbour(neighbourIndex, current.index, current.cost);
			}
		}

		return std::vector<int>(); // no path found
	}
}
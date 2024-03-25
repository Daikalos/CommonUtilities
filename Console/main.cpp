#include <iostream>
#include <string>

#include <CommonUtilities/Algorithms/Dijkstra.hpp>
#include <CommonUtilities/Utility/Random.hpp>

int main()
{
	std::vector<cu::Tile> map;


	for (int i = 0; i < 50; ++i)
	{
		map.emplace_back(cu::Tile::Passable);
	}

	//map[31] = cu::Tile::Impassable;

	std::vector<int> path = cu::Dijkstra(map, 1, 8);

	

	return 0;
}
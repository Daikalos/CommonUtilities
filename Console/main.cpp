#include <iostream>
#include <string>

#include <CommonUtilities/Structures/PriorityQueue.hpp>
#include <CommonUtilities/Utility/Random.hpp>

int main()
{
	for (int i = 0; i < 1000000; ++i)
	{
		CommonUtilities::PriorityQueue<int> heap({ 5, 2, 3, 4, 0, 10, -5 });

		int size = heap.Size();
		for (int j = 0; j < size; ++j)
		{
			std::cout << heap.Top() << '\n';
			heap.Pop();
		}

		heap.Push(5);
	}

	return 0;
}
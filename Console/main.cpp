#include <iostream>
#include <string>

#include <CommonUtilities/Structures/Heap.hpp>
#include <CommonUtilities/Utility/Random.hpp>

int main()
{
	for (int i = 0; i < 1000000; ++i)
	{
		CommonUtilities::Heap<int> heap;

		for (int j = 0; j < 1000; ++j)
		{
			heap.Enqueue(cu::rn::Random(-10000, 10000));
		}

		int size = heap.GetSize();
		for (int j = 0; j < size; ++j)
		{
			std::cout << heap.Dequeue() << '\n';
		}

		heap.Enqueue(5);
	}

	return 0;
}
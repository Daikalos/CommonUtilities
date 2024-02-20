#include <CommonUtilities/Structures/DoublyLinkedList.hpp>
#include <CommonUtilities/Utility/Random.hpp>

#include <iostream>

int main()
{
	int listCount = cu::rn::Random(512, 10024);
	for (int i = 0; i < listCount; ++i)
	{
		CommonUtilities::DoublyLinkedList<int> linkedList;

		int listSize = cu::rn::Random(512, 1024);
		for (int i = 0; i < listSize; ++i)
		{
			int test = cu::rn::Random(0, 4);
			if (test == 0)
			{
				linkedList.InsertFirst(cu::rn::Random(0, 512));
			}
			else if (test == 1)
			{
				linkedList.InsertLast(cu::rn::Random(0, 512));
			}
			else if (test == 2)
			{
				linkedList.RemoveFirst(cu::rn::Random(0, 512));
			}
			else
			{
				linkedList.RemoveLast(cu::rn::Random(0, 512));
			}
		}

		int a = linkedList.GetSize();
		int b = 0;
	}

	std::cin.get();

	return 0;
}
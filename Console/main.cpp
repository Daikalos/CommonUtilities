#include <CommonUtilities/Structures/BSTSet.hpp>
#include <CommonUtilities/Utility/Random.hpp>

#include <string>

int main()
{
	for (int i = 0; i < 1000000; ++i)
	{
		CommonUtilities::BSTSet<int> set;

		set.Insert(6);
		set.Insert(4);
		set.Insert(-2);
		set.Insert(8);
		set.Insert(10);
		set.Insert(-3);
		set.Insert(3);
		set.Insert(-7);
		set.Insert(0);
		set.Insert(15);

		set.DSWBalance();

	}

	return 0;
}
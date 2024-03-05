#include <CommonUtilities/Structures/BSTSet.hpp>

#include <CommonUtilities/Utility/Random.hpp>

int main()
{
	BSTSet<int> set;
	set.Insert(5);
	set.Insert(-3);
	set.Insert(2);
	set.Insert(7);
	set.Insert(-8);
	set.Insert(10);
	set.Remove(-8);
	
	bool exists = set.HasElement(-8);

	return 0;
}
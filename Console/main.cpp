#include <CommonUtilities/Structures/BSTSet.hpp>

int main()
{
	CommonUtilities::BSTSet<int> set;
	set.Insert(5);
	set.Insert(-3);
	set.Insert(2);
	set.Insert(7);
	set.Insert(-8);
	set.Insert(10);
	set.Remove(-8);
	set.Remove(-3);
	set.Remove(-2);
	set.Remove(-8);
	
	bool exists = set.HasElement(5);

	return 0;
}
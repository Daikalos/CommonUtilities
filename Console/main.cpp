#include <CommonUtilities/Structures/BSTSet.hpp>

#include <string>

int main()
{
	for (int i = 0; i < 100000000; ++i)
	{
		CommonUtilities::BSTSet<std::string> set;
		set.Insert("hello");
		set.Insert("wow");
		set.Insert("yesss");
		set.Insert("lololol");
		set.Remove("hello");
		set.Remove("lololol");

		bool exists = set.HasElement("lololol");
	}

	return 0;
}
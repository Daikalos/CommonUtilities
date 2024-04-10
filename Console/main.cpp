#include <iostream>
#include <string>

#include <CommonUtilities/Structures/Misc/HashMap.hpp>

int main()
{
	CommonUtilities::HashMap<std::string, int> map(-1);
	bool huh = map.Insert("wot", 5);
	return 0;
}
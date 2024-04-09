#include <iostream>
#include <string>

#include <CommonUtilities/Structures/Misc/HashMap.hpp>

int main()
{
	CommonUtilities::HashMap<std::string, int> map(-100);
	map.Insert("wot", 5);
	return 0;
}
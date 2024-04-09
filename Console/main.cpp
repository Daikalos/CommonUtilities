#include <iostream>
#include <string>

#include <CommonUtilities/Structures/Misc/HashMap.hpp>

int main()
{
	CommonUtilities::HashMap<std::string, int> map(8);
	map.Insert("hello", 5);
	map.Insert("test", 12);
	map.Insert("yes", 14);
	map.Insert("wow", 18);
	map.Insert("hey", 54);
	map.Insert("jesus", -43);
	map.Insert("what", 3);
	map.Insert("true", 32);
	map.Insert("jesus", 50);
	int val = *map.Get("hello");
	int val2 = *map.Get("test");
	int val3 = *map.Get("yes");
	int val4 = *map.Get("wow");
	int val5 = *map.Get("hey");
	int val6 = *map.Get("jesus");
	int val7 = *map.Get("what");
	int val8 = *map.Get("true");
	return 0;
}
#include <iostream>
#include <string>

#include <CommonUtilities/Structures/Blackboard.hpp>

int main()
{
	cu::Blackboard<> blackboard;
	blackboard.Set("hello", 5);
	blackboard.Set<std::string>("test", "test");
	
	int test = blackboard.Get<int>("hello");
	std::string yes = blackboard.Get<std::string>("test");

	blackboard.Erase<std::string>("test");

	std::string error = blackboard.Get<std::string>("test");

	return 0;
}
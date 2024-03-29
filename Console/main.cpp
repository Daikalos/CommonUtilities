#include <iostream>
#include <string>

#include <CommonUtilities/Structures/Blackboard.hpp>

int main()
{
	cu::Blackboard<> blackboard;

	int hey = 5;
	blackboard.Set("hello", hey);
	blackboard.Set<std::string>("test", "test");
	
	int test = blackboard.Get<int>("hello");
	std::string yes = blackboard.Get<std::string>("test");



	return 0;
}
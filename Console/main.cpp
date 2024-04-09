#include <iostream>
#include <string>

#include <CommonUtilities/Structures/Blackboard.hpp>

int main()
{
	struct Foo
	{
		int yes = 5;
		std::string work = "?";
	};

	cu::Blackboard blackboard;
	blackboard.Set("hey", Foo(5, "yes"));
	Foo& test = blackboard.Get<Foo>("hey");
	return 0;
}
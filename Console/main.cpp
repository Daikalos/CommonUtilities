#include <iostream>
#include <string>

#include <CommonUtilities/Structures/Blackboard.hpp>
#include <CommonUtilities/Utility/Random.hpp>
#include <CommonUtilities/Utility/Benchmark.h>

int main()
{
	cu::Blackboard blackboard;
	blackboard.Set("hey", 5);
	int yes = blackboard.Get<int>("hey");
	return 0;
}
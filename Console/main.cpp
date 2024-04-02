#include <iostream>
#include <string>

#include <CommonUtilities/Algorithms/Sort.hpp>
#include <CommonUtilities/Utility/Random.hpp>

int main()
{
	std::vector<int> test = cu::rn::RandomVector(100);
	std::vector<int> test2 = cu::rn::RandomVector(100);
	std::vector<int> test3 = cu::rn::RandomVector(100);

	CommonUtilities::QuickSort(test);
	CommonUtilities::QuickSort(test2);
	CommonUtilities::QuickSort(test3);

	return 0;
}
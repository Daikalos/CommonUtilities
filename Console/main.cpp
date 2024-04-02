#include <iostream>
#include <string>

#include <CommonUtilities/Algorithms/Sort.hpp>
#include <CommonUtilities/Utility/Random.hpp>
#include <CommonUtilities/Utility/Benchmark.h>

int main()
{
	std::vector<int> test = cu::rn::RandomVector(100000000);
	std::vector<int> test2 = cu::rn::RandomVector(100000000);
	std::vector<int> test3 = cu::rn::RandomVector(100000000);

	cu::bm::Begin();
	CommonUtilities::MergeSort(test);
	cu::bm::End();

	cu::bm::Begin();
	CommonUtilities::QuickSort(test2);
	cu::bm::End();

	cu::bm::Begin();
	CommonUtilities::SelectionSort(test3);
	cu::bm::End();

	return 0;
}
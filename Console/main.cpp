#include <iostream>
#include <string>

#include <CommonUtilities/Algorithms/Sort.hpp>
#include <CommonUtilities/Utility/Random.hpp>
#include <CommonUtilities/Utility/Benchmark.h>

int main()
{
	std::vector<int> test = cu::rn::RandomVector(1000);
	std::vector<int> test2 = cu::rn::RandomVector(1000);
	std::vector<int> test3 = cu::rn::RandomVector(1000);
	std::vector<int> test4 = cu::rn::RandomVector(1000);

	cu::bm::Begin("MERGE");
	CommonUtilities::MergeSort(test);
	cu::bm::End();

	cu::bm::Begin("QUICK");
	CommonUtilities::QuickSort(test2);
	cu::bm::End();

	cu::bm::Begin("SELECTION");
	CommonUtilities::SelectionSort(test3);
	cu::bm::End();

	cu::bm::Begin("BUBBLE");
	CommonUtilities::BubbleSort(test4);
	cu::bm::End();

	return 0;
}
#include <iostream>
#include <string>

#include <CommonUtilities/System/IDGenerator.h>

int main()
{
	int val0 = cu::id::Generator<>::Next();
	int val1 = cu::id::Generator<>::Next();
	int val2 = cu::id::Generator<>::Next();
	int val4 = cu::id::Generator<>::Next();
	return 0;
}
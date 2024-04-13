#include <iostream>
#include <string>

#include <CommonUtilities/System/IDGenerator.h>

int main()
{
	int val1 = cu::id::Generator<>::Next();
	int val2 = cu::id::Generator<>::Next();
	int val3 = cu::id::Generator<>::Next();

	return 0;
}
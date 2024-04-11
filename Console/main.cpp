#include <iostream>
#include <string>

#include <CommonUtilities/System/BinarySerializer.h>

int main()
{
	const int test  = 5;
	int test1 = 7;
	cu::BinaryWriteSerializer write;
	write.Serialize(test, 9);

	int test2 = 0;
	int test3 = 0;
	cu::BinaryReadSerializer read(write.GetBuffer());
	read.Serialize(test2, test3);

	return 0;
}
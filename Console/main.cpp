#include <iostream>
#include <string>

#include <CommonUtilities/System/BinarySerializer.h>

int main()
{
	int val0 = 5;
	const int val1 = 6;
	cu::BinaryWriteSerializer write;
	write.Serialize(val0, val1);

	int val2 = 0;
	int val3 = 0;
	cu::BinaryReadSerializer read(write.GetBuffer());
	read.Serialize(val2, val3);

	return 0;
}
#include <iostream>
#include <string>

#include <CommonUtilities/System/BinarySerializer.h>

int main()
{
	std::vector<int> val0{ 1, 2, 3 };
	cu::BinaryWriteSerializer write;
	write.Serialize(val0);

	int val2 = 0;
	int val3 = 0;
	int val4 = 0;
	cu::BinaryReadSerializer read(write.GetBuffer());
	read.Serialize(val2, val3, val4);

	return 0;
}
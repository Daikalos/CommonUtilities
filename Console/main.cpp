#include <iostream>
#include <string>

#include <CommonUtilities/System/BinarySerializer.h>

int main()
{
	int test = 5;
	BinaryWriteSerializer write;
	write.Serialize(test);

	int test2 = 0;
	BinaryReadSerializer read(write.GetBuffer());
	read.Serialize(test2);

	return 0;
}
#include <iostream>
#include <string>
#include <array>

#include <CommonUtilities/System/BinarySerializer.h>

int main()
{
	const std::array<int, 4> val0{ 1, 2, 3, 4 };
	cu::BinaryWriteSerializer write;
	write.Serialize(val0);

	const std::array<int, 4> val1{};
	cu::BinaryReadSerializer read(write.GetBuffer());
	read.Serialize(val1);

	return 0;
}
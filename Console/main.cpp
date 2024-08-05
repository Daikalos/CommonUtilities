#include <iostream>
#include <string>
#include <array>

#include <CommonUtilities/Math/Matrix4x4.hpp>
#include <CommonUtilities/Utility/Benchmark.h>
#include <CommonUtilities\Utility\Win32Utils.h>
#include <CommonUtilities/System/BinarySerializer.h>
#include <CommonUtilities/Utility/TypeUtils.hpp>

#include <CommonUtilities/System/TimedEvent.h>

#include <CommonUtilities/Structures/Blackboard.hpp>

#include <CommonUtilities/Math/AABB.hpp>

int main()
{
	std::vector<int> test { 4, 6, 8, 12, -3 };

	cu::BinaryWriteSerializer write;

	write.Serialize(test);

	cu::BinaryReadSerializer read(write.GetBuffer());

	std::vector<int> test2;
	read.Serialize(test2);

	constexpr auto a = cu::TypeName<int>();

	std::string_view huh = a;

	return 0;
}
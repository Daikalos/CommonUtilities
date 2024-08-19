#include <iostream>
#include <string>
#include <array>

#include <CommonUtilities/Math/Intersection.hpp>
#include <CommonUtilities/Math/Matrix4x4.hpp>
#include <CommonUtilities/Utility/Benchmark.h>
#include <CommonUtilities\Utility\Win32Utils.h>
#include <CommonUtilities/System/BinarySerializer.h>
#include <CommonUtilities/Utility/TypeUtils.hpp>
#include <CommonUtilities/Utility/Easings.hpp>

#include <CommonUtilities/System/TimedEvent.h>

#include <CommonUtilities/Structures/Blackboard.hpp>

#include <CommonUtilities/Math/AABB.hpp>

int main()
{
	cu::Vector2f vec(5.0f, 3.0f);
	auto l = vec.ToSIMD();

	cu::Mat4f mat;
	auto p = mat.ToSIMD();

	cu::AABBf aabb;
	cu::Rayf ray;

	cu::ISectf intersection = cu::IntersectionCapsuleSegment(
		cu::Capsulef(cu::Vector3f(0.0f, 0.0f, 0.0f), cu::Vector3f(0.0f, 10.0f, 0.0f), 16.0f),
		cu::Vector3f(-32.0f, 1.0f, 2.0f), 
		cu::Vector3f(32.0f, 1.0f, 2.0f));

	auto [p1, p2] = cu::Vector3f::ClosestPointsSegmentSegment({}, {}, {}, {});

	std::vector<int> test { 4, 6, 8, 12, -3 };

	cu::BinaryWriteSerializer write;

	write.Serialize(test);

	cu::BinaryReadSerializer read(write.GetBuffer());

	std::vector<int> test2;
	read.Serialize(test2);

	constexpr auto a = cu::EaseInBack(0.9f);

	return 0;
}
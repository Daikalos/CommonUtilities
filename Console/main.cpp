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

	cu::Mat4f mat = cu::Mat4f::CreateTRS(cu::Vector3f(0.0f, 5.0f, 0.0f), cu::Quatf::IDENTITY, cu::Vector3f::One);
	auto p = mat.ToSIMD();

	cu::AABBf aabb;
	cu::Rayf ray;

	constexpr cu::Capsulef caps = cu::Capsulef(cu::Vector3f(0.0f, 0.0f, 0.0f), cu::Vector3f(0.0f, 10.0f, 0.0f), 16.0f);

	cu::ISectf intersection = cu::IntersectionCapsuleSegment(
		caps,
		cu::Vector3f(-32.0f, 1.0f, 2.0f), 
		cu::Vector3f(32.0f, 1.0f, 2.0f));

	constexpr cu::AABBf test = caps.GetAABB();

	return 0;
}
#include <iostream>
#include <string>
#include <array>

#include <CommonUtilities/Math/Matrix4x4.hpp>
#include <CommonUtilities\Utility\WinUtils.h>

#include <CommonUtilities/System/TimedEvent.h>

#include <CommonUtilities/Structures/Blackboard.hpp>

#include <CommonUtilities/Math/AABB.hpp>

int main()
{
	constexpr cu::AABBf aabb1(cu::Vector3f(0, 0, -1.0f), cu::Vector3f(10.0f, 10.0f, 10.0f));
	constexpr cu::AABBf aabb2(cu::Vector3f(-1.0f, 0, -5.0f), cu::Vector3f(2.0f, 2.0f, 2.0f));

	constexpr cu::AABBf inter = aabb1.Union(aabb2);

	return 0;
}
#include <iostream>
#include <string>
#include <array>

#include <CommonUtilities/Math/Matrix4x4.hpp>
#include <CommonUtilities\Utility\WinUtils.h>

#include <CommonUtilities/System/TimedEvent.h>

#include <CommonUtilities/Structures/Blackboard.hpp>

int main()
{
	cu::Quatf quat(0.0f, 1.0f, 0.0f);

	cu::Mat4f m1;
	cu::Mat4f m2;

	const cu::Blackboard<> blackboard;
	const int* val = blackboard.TryGet<int>("hey");

	m1.SetRotation(cu::Vector3f(-cu::au::PI_2, 0.0f, 0.0f));
	m2.SetRotation(cu::Vector3f(0.0, cu::au::PI_2, 0.0f));

	cu::Mat4f m3 = m1 * m2;

	cu::Vector3f rot = m3.GetRotation();

	return 0;
}
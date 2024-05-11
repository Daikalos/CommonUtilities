#include <iostream>
#include <string>
#include <array>

#include <CommonUtilities/Math/Matrix4x4.hpp>
#include <CommonUtilities\Utility\WinUtils.h>

#include <CommonUtilities/System/TimedEvent.h>

int main()
{
	cu::Quatf rot(0.2f, -0.5f, 0.1f);
	rot.Normalize();

	cu::Mat4f mat;
	mat.SetRotation(rot);

	cu::Vector3f extrRot = rot.GetEulerAngles();
	cu::Vector3f extrScale = mat.GetScale();

	return 0;
}
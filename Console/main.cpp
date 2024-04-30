#include <iostream>
#include <string>
#include <array>

#include <CommonUtilities/Math/Matrix4x4.hpp>
#include <CommonUtilities\Utility\WinUtils.h>

int main()
{
	cu::Vector3f rot { 0.2f, -0.5f, 0.1f };

	cu::Mat4f mat;
	mat.SetRotation(rot);

	cu::Vector3f extrRot = mat.GetRotation();
	cu::Vector3f extrScale = mat.GetScale();

	return 0;
}
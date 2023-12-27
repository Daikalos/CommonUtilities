#include <CommonUtilities/Config.h>

#include <CommonUtilities/Utility/ArithmeticUtils.hpp>
#include <CommonUtilities/Math/Transform2D.h>

#include <CommonUtilities/System/Event.hpp>

int main()
{
	cu::Mat3f mat = cu::Mat3f::CreateTRS(cu::Vector2f(100.0f, 100.0f), cu::PI_2, cu::Vector2f(1.0f, 1.0f));
	cu::Mat3f mat2;

	mat2.Combine(mat);

	cu::Mat3f inv = mat.FastInverse();
	float rotation = mat2.GetRotation() * cu::RAD2DEG<>;
	float rotation2 = mat.GetRotation() * cu::RAD2DEG<>;

	mat2.Combine(inv);

	return 0;
}
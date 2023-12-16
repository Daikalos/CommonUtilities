#include <CommonUtilities/Config.h>

#include <CommonUtilities/Math/Transform2D.h>

int main()
{
	cu::Mat3f mat = cu::Mat3f::CreateTRS(cu::Vector2f(100.0f, 100.0f), 25.0f, cu::Vector2f(1.5f, 1.5f));
	cu::Mat3f mat2;

	mat2.Combine(mat);
	mat2.Combine(mat);

	cu::Mat3f inv = mat.FastInverse();

	mat2.Combine(inv);

	return 0;
}
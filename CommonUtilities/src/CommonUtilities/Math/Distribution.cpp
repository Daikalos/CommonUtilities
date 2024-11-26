#include <CommonUtilities/Math/Distribution.hpp>

#include <CommonUtilities/Utility/Random.hpp>

using namespace CommonUtilities;

Distribution<int> CommonUtilities::CreateDistributionUniform(int aMin, int aMax)
{
	return [=] { return Random(aMin, aMax); };
}

Distribution<unsigned int> CommonUtilities::CreateDistributionUniform(unsigned int aMin, unsigned int aMax)
{
	return [=] { return Random(aMin, aMax); };
}

Distribution<float> CommonUtilities::CreateDistributionUniform(float aMin, float aMax)
{
	return [=] { return Random(aMin, aMax); };
}

Distribution<Vector2f> CommonUtilities::CreateDistributionRect(const Vector2f& aCenter, const Vector2f& aHalfSize)
{
	return [=] { return RandomPointInRect(aCenter, aHalfSize); };
}

Distribution<Vector2f> CommonUtilities::CreateDistributionCircle(const Vector2f& aCenter, float aRadius)
{
	return [=] { return RandomPointInCircle(aCenter, aRadius); };
}

Distribution<Vector2f> CommonUtilities::CreateDistributionDeflect(const Vector2f& aDirection, float aMaxRotation)
{
	return [=] { return RandomDeflection(aDirection, aMaxRotation); };
}

Distribution<Vector3f> CommonUtilities::CreateDistributionBox(const Vector3f& aCenter, const Vector3f& aHalfSize)
{
	return [=] { return RandomPointInBox(aCenter, aHalfSize); };
}

Distribution<Vector3f> CommonUtilities::CreateDistributionSphere(const Vector3f& aCenter, float aRadius)
{
	return [=] { return RandomPointInSphere(aCenter, aRadius); };
}


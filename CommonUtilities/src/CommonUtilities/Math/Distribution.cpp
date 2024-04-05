#include <CommonUtilities/pch.h>
#include <CommonUtilities/Math/Distribution.hpp>

#include <CommonUtilities/Utility/Random.hpp>

using namespace CommonUtilities;

Distribution<int> CommonUtilities::CreateDistributionUniform(int aMin, int aMax)
{
	return [=] { return rn::Random(aMin, aMax); };
}

Distribution<unsigned int> CommonUtilities::CreateDistributionUniform(unsigned int aMin, unsigned int aMax)
{
	return [=] { return rn::Random(aMin, aMax); };
}

Distribution<float> CommonUtilities::CreateDistributionUniform(float aMin, float aMax)
{
	return [=] { return rn::Random(aMin, aMax); };
}

Distribution<cu::Vector2f> CommonUtilities::CreateDistributionRect(const cu::Vector2f& aCenter, const cu::Vector2f& aHalfSize)
{
	return [=] { return rn::RandomPointInRect(aCenter, aHalfSize); };
}

Distribution<cu::Vector2f> CommonUtilities::CreateDistributionCircle(const cu::Vector2f& aCenter, float aRadius)
{
	return [=] { return rn::RandomPointInCircle(aCenter, aRadius); };
}

Distribution<cu::Vector2f> CommonUtilities::CreateDistributionDeflect(const cu::Vector2f& aDirection, float aMaxRotation)
{
	return [=] { return rn::RandomDeflection(aDirection, aMaxRotation); };
}

Distribution<cu::Vector3f> CommonUtilities::CreateDistributionRect(const cu::Vector3f& aCenter, const cu::Vector3f& aHalfSize)
{
	return [=] { return rn::RandomPointInRect(aCenter, aHalfSize); };
}

Distribution<cu::Vector3f> CommonUtilities::CreateDistributionSphere(const cu::Vector3f& aCenter, float aRadius)
{
	return [=] { return rn::RandomPointInSphere(aCenter, aRadius); };
}


#pragma once

#include <iostream>

#include <CommonUtilities/Math/Vector2.hpp>
#include <CommonUtilities/Math/Vector3.hpp>
#include <CommonUtilities/Math/Vector4.hpp>

namespace CommonUtilities
{
	template<typename T>
	inline std::ostream& operator<<(std::ostream& aOut, const Vector2<T>& aVector)
	{
		aOut << "(" << aVector.x << ", " << aVector.y << ")";
		return aOut;
	}

	template<typename T>
	inline std::ostream& operator<<(std::ostream& aOut, const Vector3<T>& aVector)
	{
		aOut << "(" << aVector.x << ", " << aVector.y << ", " << aVector.z << ")";
		return aOut;
	}

	template<typename T>
	inline std::ostream& operator<<(std::ostream& aOut, const Vector4<T>& aVector)
	{
		aOut << "(" << aVector.x << ", " << aVector.y << ", " << aVector.z << ", " << aVector.w << ")";
		return aOut;
	}
}
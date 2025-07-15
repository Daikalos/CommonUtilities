#pragma once

#include <vector>

#include <CommonUtilities/Math/Vector3.hpp>

namespace CommonUtilities
{
	template<typename T>
	class Curve
	{
	public:
		constexpr Curve() = default;
		constexpr virtual ~Curve() = default;

		constexpr void AddPoint(const Vector3f& aPoint);
		constexpr void ErasePoint(std::size_t aIndex);

		constexpr void Clear();

	protected:
		virtual constexpr Vector3f Interpolate(float aT, const Vector3f& aP0, const Vector3f& aP1, const Vector3f& aP2, const Vector3f& aP3) = 0;

	private:
		std::vector<Vector3f>	myPoints;
		std::vector<float>		myDistances;
	};
}
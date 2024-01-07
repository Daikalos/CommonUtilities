#pragma once

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	class COMMON_UTILITIES_API Shape
	{
	public:
		enum class Type
		{
			None = -1,
			AABB3D,
			Sphere,
			Line,
			LineVolume,
			Plane,
			PlaneVolume,
			Ray,
			Count
		};

		CONSTEXPR Shape() = default;
		CONSTEXPR virtual ~Shape() = default;

		virtual auto GetType() const noexcept -> Type = 0;
	};
}
#pragma once

#include <memory>

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	class COMMON_UTILITIES_API Shape
	{
	public:
		constexpr Shape() = default;
		constexpr virtual ~Shape() = default;

		enum class Type // explicitly stated to prevent any human-error
		{
			None		= -1,
			AABB3D		= 0,
			Sphere		= 1,
			Line		= 2,
			LineVolume	= 3,
			Plane		= 4,
			PlaneVolume	= 5,
			Ray			= 6,
			Count		= 7
		};

		NODISC constexpr virtual auto GetType() const noexcept -> Type = 0;
		NODISC constexpr virtual std::unique_ptr<Shape> Clone() const = 0;
	};
}
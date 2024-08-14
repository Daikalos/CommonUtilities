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
			AABB		= 0,
			Sphere		= 1,
			Capsule		= 2,
			Ray			= 3,
			Plane		= 4,
			Triangle	= 5,
			Line		= 6,
			LineVolume	= 7,
			PlaneVolume	= 8,
			Count		= 9
		};

		NODISC constexpr virtual Type GetType() const noexcept = 0;
		NODISC constexpr virtual std::unique_ptr<Shape> Clone() const = 0;
	};
}
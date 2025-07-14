#pragma once

#include <CommonUtilities/Math/Vector2.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	COMMON_UTILITIES_API const Vector2f&				GetDesktopResolution();
	COMMON_UTILITIES_API float							GetDesktopAspectRatio();
	COMMON_UTILITIES_API const std::vector<Vector2f>&	GetAllResolutions();
	COMMON_UTILITIES_API const std::vector<Vector2f>&	GetInRatioResolutions();
	COMMON_UTILITIES_API long double					Get_CPU_Usage();
	COMMON_UTILITIES_API std::size_t					Get_RAM_Usage();
}
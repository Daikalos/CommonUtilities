#pragma once

#include <CommonUtilities\Math\Vector2.hpp>
#include <CommonUtilities\Config.h>

namespace CommonUtilities
{
	COMMON_UTILITIES_API const cu::Vector2f& GetDesktopResolution();
	COMMON_UTILITIES_API const std::vector<cu::Vector2f>& GetValidResolutions();
	COMMON_UTILITIES_API long double Get_CPU_Usage();
}
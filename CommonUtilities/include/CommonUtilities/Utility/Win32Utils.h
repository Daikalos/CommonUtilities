#pragma once

#include <Windows.h>

#include <CommonUtilities\Math\Vector2.hpp>
#include <CommonUtilities\Config.h>

namespace CommonUtilities
{
	COMMON_UTILITIES_API const cu::Vector2f& GetDesktopResolution();
	COMMON_UTILITIES_API const std::vector<cu::Vector2f>& GetValidResolutions();
	COMMON_UTILITIES_API long double Get_CPU_Usage();
	COMMON_UTILITIES_API std::size_t Get_RAM_Usage();

	//COMMON_UTILITIES_API Vector2i GetPosition(HANDLE aWindowHandle);
	//COMMON_UTILITIES_API void SetPosition(HANDLE aWindowHandle, const Vector2i& aPosition);

	//COMMON_UTILITIES_API Vector2u GetSize(HANDLE aWindowHandle);
	//COMMON_UTILITIES_API void SetSize(HANDLE aWindowHandle, const Vector2u& aSize);
}
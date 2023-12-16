#pragma once

#include <string>
#include <string_view>

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	NODISC COMMON_UTILITIES_API std::string RemoveTrailingZeroes(std::string_view aString);
}
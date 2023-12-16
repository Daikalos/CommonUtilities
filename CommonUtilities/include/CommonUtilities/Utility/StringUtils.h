#pragma once

#include <string>
#include <string_view>

#include <CommonUtilities/Config.h>

namespace CommonUtilities::su
{
	NODISC COMMON_UTILITIES_API std::string RemoveTrailingZeroes(std::string_view aString);
}
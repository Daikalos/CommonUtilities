#pragma once

#include <vector>
#include <filesystem>

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	COMMON_UTILITIES_API NODISC std::size_t FileSize(const std::filesystem::path& aPath);

	COMMON_UTILITIES_API NODISC bool ReadFileBytes(const std::filesystem::path& aPath, std::vector<std::byte>& outBytes);
}
#include <CommonUtilities/Utility/StringUtils.h>

namespace CommonUtilities::su
{
	std::string RemoveTrailingZeroes(std::string_view aStr)
	{
		std::string result = std::string(aStr.data());

		if (result.find('.') != std::string::npos)
		{
			result = result.substr(0, result.find_last_not_of('0') + 1);

			if (result.find('.') == result.size() - 1)
				result = result.substr(0, result.size() - 1);
		}

		return result;
	}
}
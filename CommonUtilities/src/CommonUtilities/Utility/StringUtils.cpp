#include <CommonUtilities/Utility/StringUtils.h>

namespace CommonUtilities
{
	std::string RemoveTrailingZeroes(std::string_view aStr)
	{
		if (aStr.find('.') != std::string::npos)
		{
			aStr = aStr.substr(0, aStr.find_last_not_of('0') + 1);

			if (aStr.find('.') == aStr.size() - 1)
				aStr = aStr.substr(0, aStr.size() - 1);
		}

		return std::string(aStr);
	}
}
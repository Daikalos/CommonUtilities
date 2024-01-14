#include <CommonUtilities/Utility/StringUtils.h>

#include <algorithm>

namespace CommonUtilities::st
{ 
	std::string ToLower(std::string_view aString)
	{
		std::string result{aString};

		std::transform(result.begin(), result.end(), result.begin(), 
			[](unsigned char aCharacter)
			{
				return static_cast<unsigned char>(std::tolower(aCharacter));
			});

		return result;
	}
	std::string ToUpper(std::string_view aString)
	{
		std::string result{aString};

		std::transform(result.begin(), result.end(), result.begin(), 
			[](unsigned char aCharacter)
			{
				return static_cast<unsigned char>(std::toupper(aCharacter));
			});

		return result;
	}

	std::string Capitalize(std::string_view aString)
	{
		std::string result{ToLower(aString)};

		if (!result.empty())
		{
			result.front() = static_cast<char>(std::toupper(result.front()));
		}

		return result;
	}

	bool Contains(std::string_view aString, std::string_view aSubString)
	{
		return aString.find(aSubString) != std::string_view::npos;
	}
	bool Contains(std::string_view aString, const char aCharacter)
	{
		return Contains(aString, std::string(1, aCharacter));
	}

	bool CompareIgnoreCase(std::string_view aFirstString, std::string_view aSecondString)
	{
		return ToLower(aFirstString) == ToLower(aSecondString);
	}

	void TrimLeft(std::string& aString)
	{
		aString.erase(aString.begin(),
			std::find_if(aString.begin(), aString.end(),
			[](int aCharacter)
			{ 
				return !std::isspace(aCharacter);
			}));
	}
	void TrimRight(std::string& aString)
	{
		aString.erase(
			std::find_if(aString.rbegin(), aString.rend(),
			[](int aCharacter) 
			{ 
				return !std::isspace(aCharacter);
			}).base(), aString.end());
	}
	void Trim(std::string& aString)
	{
		TrimLeft(aString);
		TrimRight(aString);
	}

	std::string TrimLeftCopy(std::string_view aString)
	{
		std::string result{aString};
		TrimLeft(result);
		return result;
	}
	std::string TrimRightCopy(std::string_view aString)
	{
		std::string result{aString};
		TrimRight(result);
		return result;
	}
	std::string TrimCopy(std::string_view aString)
	{
		std::string result{aString};
		Trim(result);
		return result;
	}

	bool ReplaceFirst(std::string& aString, std::string_view aTarget, std::string_view aReplacement)
	{
		if (aTarget.empty())
		{
			return false;
		}

		const std::size_t startPos = aString.find(aTarget);
		if (startPos == std::string::npos)
		{
			return false;
		}

		aString.replace(startPos, aTarget.length(), aReplacement);

		return true;
	}
	bool ReplaceLast(std::string& aString, std::string_view aTarget, std::string_view aReplacement)
	{
		if (aTarget.empty())
		{
			return false;
		}

		const std::size_t startPos = aString.rfind(aTarget);
		if (startPos == std::string::npos)
		{
			return false;
		}

		aString.replace(startPos, aTarget.length(), aReplacement);

		return true;
	}
	bool ReplaceAll(std::string& aString, std::string_view aTarget, std::string_view aReplacement)
	{
        if (aTarget.empty())
        {
            return false;
        }

		std::size_t startPos = aString.find(aTarget);
		if (startPos == std::string::npos)
		{
			return false;
		}

		do 
        {
			aString.replace(startPos, aTarget.length(), aReplacement);
			startPos += aReplacement.length();
		} 
		while ((startPos = aString.find(aTarget, startPos)) != std::string::npos);

        return true;
	}

	bool EndsWith(std::string_view aString, std::string_view aSuffix)
	{
		return aString.ends_with(aSuffix);
	}
	bool EndsWith(std::string_view aString, const char aSuffix)
	{
		return aString.ends_with(aSuffix);
	}
	bool StartsWith(std::string_view aString, std::string_view aPrefix)
	{
		return aString.starts_with(aPrefix);
	}
	bool StartsWith(std::string_view aString, const char aPrefix)
	{
		return aString.starts_with(aPrefix);
	}

	std::string RemoveTrailingZeroes(std::string_view aStr)
	{
		if (aStr.find('.') != std::string_view::npos)
		{
			aStr = aStr.substr(0, aStr.find_last_not_of('0') + 1);

			if (aStr.find('.') == aStr.size() - 1)
				aStr = aStr.substr(0, aStr.size() - 1);
		}

		return std::string(aStr);
	}
}
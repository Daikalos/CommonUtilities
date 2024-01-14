#pragma once

#include <string>
#include <string_view>
#include <sstream>

#include <CommonUtilities/Config.h>

namespace CommonUtilities::st
{
	template<typename T>
	NODISC std::string ToString(const T& aValue);

	template<typename T>
	NODISC T ParseString(std::string_view aString);

	NODISC COMMON_UTILITIES_API std::string ToLower(std::string_view aString);
	NODISC COMMON_UTILITIES_API std::string ToUpper(std::string_view aString);

	NODISC COMMON_UTILITIES_API std::string Capitalize(std::string_view aString);

	NODISC COMMON_UTILITIES_API bool Contains(std::string_view aString, std::string_view aSubString);
	NODISC COMMON_UTILITIES_API bool Contains(std::string_view aString, const char aCharacter);

	NODISC COMMON_UTILITIES_API bool CompareIgnoreCase(std::string_view aFirstString, std::string_view aSecondString);

	COMMON_UTILITIES_API void TrimLeft(std::string& aString);
	COMMON_UTILITIES_API void TrimRight(std::string& aString);
	COMMON_UTILITIES_API void Trim(std::string& aString);

	NODISC COMMON_UTILITIES_API std::string TrimLeftCopy(std::string_view aString);
	NODISC COMMON_UTILITIES_API std::string TrimRightCopy(std::string_view aString);
	NODISC COMMON_UTILITIES_API std::string TrimCopy(std::string_view aString);

	COMMON_UTILITIES_API bool ReplaceFirst(std::string& aString, std::string_view aTarget, std::string_view aReplacement);
	COMMON_UTILITIES_API bool ReplaceLast(std::string& aString, std::string_view aTarget, std::string_view aReplacement);
	COMMON_UTILITIES_API bool ReplaceAll(std::string& aString, std::string_view aTarget, std::string_view aReplacement);

	COMMON_UTILITIES_API bool EndsWith(std::string_view aString, std::string_view aSuffix);
	COMMON_UTILITIES_API bool EndsWith(std::string_view aString, const char aSuffix);
	COMMON_UTILITIES_API bool StartsWith(std::string_view aString, std::string_view aPrefix);
	COMMON_UTILITIES_API bool StartsWith(std::string_view aString, const char aPrefix);

	NODISC COMMON_UTILITIES_API std::string RemoveTrailingZeroes(std::string_view aString);

	template<typename T>
	inline std::string ToString(const T& aValue)
	{
		std::stringstream ss;
		ss << aValue;

		return ss.str();
	}

	template<typename T>
	inline T ParseString(std::string_view aString)
	{
		T result{};
		std::istringstream(std::string(aString)) >> result;

		return result;
	}
}
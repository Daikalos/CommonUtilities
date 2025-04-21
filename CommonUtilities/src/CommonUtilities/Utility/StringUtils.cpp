#include <CommonUtilities/Utility/StringUtils.h>

#include <algorithm>

namespace CommonUtilities
{ 
	void ToLower(std::string& aString)
	{
		std::transform(aString.begin(), aString.end(), aString.begin(),
			[](unsigned char aCharacter)
			{
				return static_cast<unsigned char>(std::tolower(aCharacter));
			});
	}
	void ToUpper(std::string& aString)
	{
		std::transform(aString.begin(), aString.end(), aString.begin(),
			[](unsigned char aCharacter)
			{
				return static_cast<unsigned char>(std::toupper(aCharacter));
			});
	}

	void Capitalize(std::string& aString)
	{
		ToLower(aString);

		if (!aString.empty())
		{
			aString.front() = static_cast<char>(std::toupper(aString.front()));
		}
	}

	std::string ToLowerCopy(std::string_view aString)
	{
		std::string result{aString};

		std::transform(result.begin(), result.end(), result.begin(), 
			[](unsigned char aCharacter)
			{
				return static_cast<unsigned char>(std::tolower(aCharacter));
			});

		return result;
	}
	std::string ToUpperCopy(std::string_view aString)
	{
		std::string result{aString};

		std::transform(result.begin(), result.end(), result.begin(), 
			[](unsigned char aCharacter)
			{
				return static_cast<unsigned char>(std::toupper(aCharacter));
			});

		return result;
	}

	std::string CapitalizeCopy(std::string_view aString)
	{
		std::string result{ ToLowerCopy(aString) };

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

	bool ContainsAny(std::string_view aString, std::initializer_list<std::string_view> aSubStrings)
	{
		for (auto& subString : aSubStrings)
		{
			if (Contains(aString, subString))
				return true;
		}
		return false;
	}

	bool CompareIgnoreCase(std::string_view aFirstString, std::string_view aSecondString)
	{
		return ToLowerCopy(aFirstString) == ToLowerCopy(aSecondString);
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

	std::string RemoveTrailingZeroes(std::string_view aStr, int aKeepPlaces)
	{
		if (aStr.empty())
			return "";

		auto dotIndex = aStr.find('.');
		if (dotIndex != std::string_view::npos && dotIndex != aStr.size() - 1)
		{
			auto startIndex	= aStr.find_last_not_of('0');
			auto endCount	= (startIndex - dotIndex);

			if (aKeepPlaces > endCount)
			{
				startIndex += aKeepPlaces - endCount;
			}

			aStr = aStr.substr(0, startIndex + 1);
		}

		return std::string(aStr);
	}

	float JaroWinklerDistance(std::string_view aFirst, std::string_view aSecond, bool aIgnoreCase)
	{
		float m = 0;

		size_t low, high;
		size_t k = 0, t = 0;

		if (aFirst.empty() || aSecond.empty())
			return 0;

		if (aFirst == aSecond)
			return 1;

		const size_t range = (std::max)(aFirst.length(), aSecond.length()) / 2 - 1;

		std::vector<size_t> str1Matches;
		str1Matches.resize(aFirst.length());
		std::vector<size_t> str2Matches;
		str2Matches.resize(aSecond.length());

		if (aIgnoreCase)
		{ 
			for (size_t i = 0; i < aFirst.length(); i++)
			{
				if (i >= range)
				{
					low = i - range;
				}
				else
				{
					low = 0;
				}

				if (i + range <= aSecond.length() - 1)
				{
					high = i + range;
				}
				else
				{
					high = aSecond.length() - 1;
				}

				for (size_t j = low; j <= high; j++)
				{
					if (str1Matches[i] != 1 && str2Matches[j] != 1 && std::tolower(aFirst[i]) == std::tolower(aSecond[j]))
					{
						m += 1;
						str1Matches[i] = 1;
						str2Matches[j] = 1;
						break;
					}
				}
			}

			if (m == 0)
			{
				return 0;
			}

			for(size_t i = 0; i < aFirst.length(); i++)
			{
				if (str1Matches[i] == 1)
				{
					size_t j = 0;
					for (j = k; j < aSecond.length(); j++)
					{
						if (str2Matches[j] == 1)
						{
							k = j + 1;
							break;
						}
					}

					if (std::tolower(aFirst[i]) != std::tolower(aSecond[j]))
					{
						t += 1;
					}
				}
			}

			float w = (m / aFirst.length() + m / aSecond.length() + (m - t / 2) / m) / 3;

			if (w > 0.7f)
			{
				float p		= 0.1f;
				size_t l	= 0;
				size_t e	= std::min({ aFirst.length(), aSecond.length(), 4ULL });

				while (l < e && std::tolower(aFirst[l]) == std::tolower(aSecond[l]))
				{
					l += 1;
				}

				w += l * p * (1 - w);
			}

			return w;
		}

		for (size_t i = 0; i < aFirst.length(); i++)
		{
			if (i >= range)
			{
				low = i - range;
			}
			else
			{
				low = 0;
			}

			if (i + range <= aSecond.length() - 1)
			{
				high = i + range;
			}
			else
			{
				high = aSecond.length() - 1;
			}

			for (size_t j = low; j <= high; j++)
			{
				if (str1Matches[i] != 1 && str2Matches[j] != 1 && aFirst[i] == aSecond[j])
				{
					m += 1;
					str1Matches[i] = 1;
					str2Matches[j] = 1;
					break;
				}
			}
		}

		if (m == 0)
		{
			return 0;
		}

		for (size_t i = 0; i < aFirst.length(); i++)
		{
			if (str1Matches[i] == 1)
			{
				size_t j = 0;
				for (j = k; j < aSecond.length(); j++)
				{
					if (str2Matches[j] == 1)
					{
						k = j + 1;
						break;
					}
				}

				if (aFirst[i] != aSecond[j])
				{
					t += 1;
				}
			}
		}

		float w = (m / aFirst.length() + m / aSecond.length() + (m - t / 2) / m) / 3;

		if (w > 0.7f)
		{
			float p		= 0.1f;
			size_t l	= 0;
			size_t e	= std::min({ aFirst.length(), aSecond.length(), 4ULL });

			while (l < e && aFirst[l] == aSecond[l])
			{
				l += 1;
			}

			w += l * p * (1 - w);
		}

		return w;
	}
}
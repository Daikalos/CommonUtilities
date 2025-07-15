#pragma once

#include <CommonUtilities/Utility/ArithmeticUtils.hpp>

#include <CommonUtilities/Config.h>

#include "Color.hpp"

namespace CommonUtilities
{
	template<typename T>
	class Curve
	{
	public:
		constexpr Curve() = default;
		constexpr ~Curve() = default;

		constexpr Curve(const T& aMin, const T& aMax);

		NODISC constexpr const std::unordered_map<unsigned, T>& GetKeyPositions() const;
		NODISC constexpr const std::unordered_map<unsigned, T>& GetKeys() const;

		NODISC constexpr T Get(float aPosition) const;

		constexpr unsigned AddKey(float aPosition, const T& aValue);

		constexpr bool RemoveKey(unsigned aRealValue);
		constexpr bool RemoveKey(float aPosition, float aTolerance = 0.01f);

		constexpr void Reset(const T& aMin, const T& aMax);

		constexpr void Clear();

	private:
		static constexpr float ourPrecision = 100000.0f;

		std::vector<unsigned>			myKeyPositions;
		std::unordered_map<unsigned, T> myKeys;
	};

	template<typename T>
	constexpr Curve<T>::Curve(const T& aMin, const T& aMax)
	{
		Reset(aMin, aMax);
	}

	template<typename T>
	constexpr const std::unordered_map<unsigned, T>& Curve<T>::GetKeyPositions() const
	{
		return myKeyPositions;
	}
	template<typename T>
	constexpr const std::unordered_map<unsigned, T>& Curve<T>::GetKeys() const
	{
		return myKeys;
	}

	template<typename T>
	constexpr T Curve<T>::Get(float aPosition) const
	{
		const unsigned realValue = static_cast<unsigned>(aPosition * ourPrecision);

		T startValue{}, endValue{};

		for (auto it = myKeyPositions.begin(); it != myKeyPositions.end(); ++it)
		{
			if (*it == realValue)
			{
				return myKeys.at(*it);
			}

			if (*it < realValue)
			{
				if (it + 1 != myKeyPositions.end())
				{
					if (*(it + 1) > realValue)
					{
						const float startPos = static_cast<float>(*it) / ourPrecision;
						const float endPos = static_cast<float>(*(it + 1)) / ourPrecision;

						startValue	= myKeys.at(*it);
						endValue	= myKeys.at(*(it + 1));

						const float alpha = MapToRange(aPosition, startPos, endPos, 0.0f, 1.0f);

						using CommonUtilities::Lerp;

						return Lerp(startValue, endValue, alpha);
					}
				}
				else
				{
					return myKeys.at(*it);
				}
			}
			else if (it == myKeyPositions.begin() && realValue < *it)
			{
				return myKeys.at(*it);
			}
		}

		return T();
	}

	template<typename T>
	constexpr unsigned Curve<T>::AddKey(float aPosition, const T& aValue)
	{
		aPosition = Saturate(aPosition);

		const unsigned realValue = static_cast<unsigned>(aPosition * ourPrecision);

		if (std::find(myKeyPositions.begin(), myKeyPositions.end(), realValue) == myKeyPositions.end())
		{
			myKeyPositions.emplace_back(realValue);
		}

		myKeys.emplace(realValue, aValue);

		return realValue;
	}
	template<typename T>
	constexpr bool Curve<T>::RemoveKey(unsigned aRealValue)
	{
		auto it = myKeys.find(aRealValue);

		if (it == myKeys.end())
			return false;

		myKeyPositions.erase(std::remove(myKeyPositions.begin(), myKeyPositions.end(), aRealValue), myKeyPositions.end());
		myKeys.erase(it);

		return true;
	}

	template<typename T>
	constexpr bool Curve<T>::RemoveKey(float aPosition, float aTolerance)
	{
		using CommonUtilities::Equal;

		const unsigned realValue		= static_cast<unsigned>(aPosition * ourPrecision);
		const unsigned realTolerance	= static_cast<unsigned>(aTolerance * ourPrecision);

		for (auto it = myKeyPositions.begin(); it != myKeyPositions.end(); ++it)
		{
			if (Equal(*it, realValue, realTolerance))
			{
				myKeyPositions.erase(it);
				myKeys.erase(*it);

				return true;
			}
		}

		return false;
	}

	template<typename T>
	constexpr void Curve<T>::Reset(const T& aMin, const T& aMax)
	{
		Clear();

		AddKey(0.0f, aMin);
		AddKey(1.0f, aMax);
	}

	template<typename T>
	constexpr void Curve<T>::Clear()
	{
		myKeyPositions.clear();
		myKeys.clear();
	}

	using ColorGradient = Curve<Color>;
}
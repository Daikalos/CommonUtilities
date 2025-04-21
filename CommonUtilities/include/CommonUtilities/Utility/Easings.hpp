#pragma once

#include <CommonUtilities/Structures/EnumArray.hpp>

#include "ArithmeticUtils.hpp"

namespace CommonUtilities
{
	// Check out https://easings.net/ for visualization of the easings

	enum class EaseType
	{
		None = -1,

		EaseInSine,
		EaseOutSine,
		EaseInOutSine,

		EaseInQuad,
		EaseOutQuad,
		EaseInOutQuad,

		EaseInCubic,
		EaseOutCubic,
		EaseInOutCubic,

		EaseInQuart,
		EaseOutQuart,
		EaseInOutQuart,

		EaseInQuint,
		EaseOutQuint,
		EaseInOutQuint,

		EaseInExpo,
		EaseOutExpo,
		EaseInOutExpo,

		EaseInCirc,
		EaseOutCirc,
		EaseInOutCirc,

		EaseInBack,
		EaseOutBack,
		EaseInOutBack,

		Count
	};

	template<IsFloatingPointType T>
	NODISC constexpr T EaseInSine(T aValue)
	{
		aValue = Saturate(aValue);

		return T(1) - std::cos((aValue * PI_V<T>) / T(2));
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseOutSine(T aValue)
	{
		aValue = Saturate(aValue);

		return std::sin((aValue * PI_V<T>) / T(2));
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseInOutSine(T aValue)
	{
		aValue = Saturate(aValue);

		return -(std::cos(aValue * PI_V<T>) - T(1)) / T(2);
	}

	template<IsFloatingPointType T>
	NODISC constexpr T EaseInQuad(T aValue)
	{
		aValue = Saturate(aValue);

		return aValue * aValue;
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseOutQuad(T aValue)
	{
		aValue = Saturate(aValue);

		return T(1) - (T(1) - aValue) * (T(1) - aValue);
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseInOutQuad(T aValue)
	{
		aValue = Saturate(aValue);
		return (aValue < T(0.5))
			? (T(2) * aValue * aValue)
			: (T(1) - std::pow(T(-2) * aValue + T(2), T(2)) / T(2));
	}

	template<IsFloatingPointType T>
	NODISC constexpr T EaseInCubic(T aValue)
	{
		aValue = Saturate(aValue);

		return aValue * aValue * aValue;
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseOutCubic(T aValue)
	{
		aValue = Saturate(aValue);

		return (aValue == T(1)) ?
			T(1) : T(1) - std::pow(T(1) - aValue, T(3));
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseInOutCubic(T aValue)
	{
		aValue = Saturate(aValue);

		return (aValue < T(0.5)) 
			? (T(4) * aValue * aValue * aValue) 
			: (T(1) - std::pow(T(-2) * aValue + T(2), T(3)) / T(2));
	}

	template<IsFloatingPointType T>
	NODISC constexpr T EaseInQuart(T aValue)
	{
		aValue = Saturate(aValue);

		return aValue * aValue * aValue * aValue;
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseOutQuart(T aValue)
	{
		aValue = Saturate(aValue);

		return (aValue == T(1)) 
			? T(1) : T(1) - std::pow(T(1) - aValue, T(4));
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseInOutQuart(T aValue)
	{
		aValue = Saturate(aValue);

		return (aValue < T(0.5)) 
			? (T(8) * aValue * aValue * aValue * aValue) 
			: (T(1) - std::pow(T(-2) * aValue + T(2), T(4)) / T(2));
	}

	template<IsFloatingPointType T>
	NODISC constexpr T EaseInQuint(T aValue)
	{
		aValue = Saturate(aValue);

		return aValue * aValue * aValue * aValue * aValue;
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseOutQuint(T aValue)
	{
		aValue = Saturate(aValue);

		return (aValue == T(1))
			? T(1) : T(1) - std::pow(T(1) - aValue, T(5));
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseInOutQuint(T aValue)
	{
		aValue = Saturate(aValue);

		return (aValue < T(0.5)) ? (T(16) * aValue * aValue * aValue * aValue * aValue) : (T(1) - std::pow(T(-2) * aValue + T(2), T(5)) / T(2));
	}

	template<IsFloatingPointType T>
	NODISC constexpr T EaseInExpo(T aValue)
	{
		aValue = Saturate(aValue);

		return (aValue == T(0)) 
			? T(0) : std::pow(T(2), T(10) * aValue - T(10));
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseOutExpo(T aValue)
	{
		aValue = Saturate(aValue);

		return (aValue == T(1))
			? T(1) : T(1) - std::pow(T(2), T(-10) * aValue);
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseInOutExpo(T aValue)
	{
		aValue = Saturate(aValue);

		return (aValue == T(0))
			? T(0) : (aValue == T(1)) 
			? T(1) : (aValue < T(0.5)) 
			? std::pow(T(2), T(20) * aValue - T(10)) / T(2) : (T(2) - std::pow(T(2), T(-20) * aValue + T(10))) / T(2);
	}

	template<IsFloatingPointType T>
	NODISC constexpr T EaseInCirc(T aValue)
	{
		aValue = Saturate(aValue);

		return (aValue == T(0)) 
			? T(0) : T(1) - std::sqrt(T(1) - std::pow(aValue, T(2)));
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseOutCirc(T aValue)
	{
		aValue = Saturate(aValue);

		return (aValue >= T(1)) 
			? T(1) : (aValue == T(0))
			? T(0) : std::sqrt(T(1) - std::pow(aValue - T(1), T(2)));
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseInOutCirc(T aValue)
	{
		aValue = Saturate(aValue);

		return (aValue < T(0.5))
			? (T(1) - std::sqrt(T(1) - std::pow(T(2) * aValue, T(2)))) / T(2)
			: (std::sqrt(T(1) - std::pow(T(-2) * aValue + T(2), T(2))) + T(1)) / T(2);
	}

	template<IsFloatingPointType T>
	NODISC constexpr T EaseInBack(T aValue)
	{
		aValue = Saturate(aValue);

		static constexpr T c1 = T(1.70158);
		static constexpr T c3 = c1 + T(1);

		return c3 * aValue * aValue * aValue - c1 * aValue * aValue;
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseOutBack(T aValue)
	{
		aValue = Saturate(aValue);

		static constexpr T c1 = T(1.70158);
		static constexpr T c3 = c1 + T(1);

		return (aValue == T(1)) 
			? T(1) : T(1) + c3 * std::pow(aValue - T(1), T(3)) + c1 * std::pow(aValue - T(1), T(2));
	}
	template<IsFloatingPointType T>
	NODISC constexpr T EaseInOutBack(T aValue)
	{
		aValue = Saturate(aValue);

		static constexpr T c1 = T(1.70158);
		static constexpr T c2 = c1 * T(1.525);

		return (aValue < T(0.5))
			? (std::pow(T(2) * aValue, T(2)) * ((c2 + T(1)) * T(2) * aValue - c2)) / T(2)
			: (std::pow(T(2) * aValue - T(2), T(2)) * ((c2 + T(1)) * (aValue * T(2) - T(2)) + c2) + T(2)) / T(2);
	}

	namespace details
	{
		template<IsFloatingPointType T>
		inline static cu::EnumArray<EaseType, T(*)(T), static_cast<int>(EaseType::Count)> globalEasingsFunc
		{
			EaseInSine,		EaseOutSine,	EaseInOutSine,
			EaseInQuad,		EaseOutQuad,	EaseInOutQuad,
			EaseInCubic,	EaseOutCubic,	EaseInOutCubic,
			EaseInQuart,	EaseOutQuart,	EaseInOutQuart,
			EaseInQuint,	EaseOutQuint,	EaseInOutQuint,
			EaseInExpo,		EaseOutExpo,	EaseInOutExpo,
			EaseInCirc,		EaseOutCirc,	EaseInOutCirc,
			EaseInBack,		EaseOutBack,	EaseInOutBack
		};
	}

	template<IsFloatingPointType T = float>
	class Easing
	{
	public:
		Easing(EaseType aEaseType, T aMin = T(0), T aMax = T(1), T aValue = T(0))
			: myType(aEaseType)
			, myEaseFunc((aEaseType != EaseType::None) ? details::globalEasingsFunc<T>[aEaseType] : nullptr)
			, myMin(aMin)
			, myMax(aMax)
			, myValue(Clamp(aValue, myMin, myMax))
			, myIsReverse(false)
		{

		}

		NODISC EaseType GetType() const { return myType; }
		NODISC T GetMin() const { return myMin; }
		NODISC T GetMax() const { return myMax; }
		NODISC bool IsReverse() const { return myIsReverse; }

		void SetEaseType(EaseType aEaseType)	{ myType = aEaseType; myEaseFunc = (myType != EaseType::None) ? details::globalEasingsFunc<T>[aEaseType] : nullptr; }
		void SetMin(T aMin)						{ myMin = aMin; }
		void SetMax(T aMax)						{ myMax = aMax; }
		void SetValue(T aValue)					{ myValue = Clamp(aValue, myMin, myMax); }
		void SetReverse(bool aFlag)				{ myIsReverse = aFlag; }

		NODISC T Get(T aValue)
		{
			return (myEaseFunc != nullptr) ? MapToRange(myEaseFunc(MapToRange(aValue, myMin, myMax, T(0), T(1))), T(0), T(1), myMin, myMax) : Clamp(aValue, myMin, myMax);
		}
		NODISC T Get()
		{
			return Get(myValue);
		}

		NODISC T operator()(T aValue) const
		{
			return Get(aValue);
		}
		NODISC T operator()() const
		{
			return Get();
		}

		void Reset()
		{
			myValue = myMin;
		}

		T Update(T aDT)
		{
			return Get(myValue = Clamp(myValue + (myIsReverse ? -aDT : aDT), myMin, myMax));
		}

		static const Easing EaseNone;

		static const Easing EaseInSine;
		static const Easing EaseOutSine;
		static const Easing EaseInOutSine;

		static const Easing EaseInQuad;
		static const Easing EaseOutQuad;
		static const Easing EaseInOutQuad;

		static const Easing EaseInCubic;
		static const Easing EaseOutCubic;
		static const Easing EaseInOutCubic;

		static const Easing EaseInQuart;
		static const Easing EaseOutQuart;
		static const Easing EaseInOutQuart;

		static const Easing EaseInQuint;
		static const Easing EaseOutQuint;
		static const Easing EaseInOutQuint;

		static const Easing EaseInExpo;
		static const Easing EaseOutExpo;
		static const Easing EaseInOutExpo;

		static const Easing EaseInCirc;
		static const Easing EaseOutCirc;
		static const Easing EaseInOutCirc;

		static const Easing EaseInBack;
		static const Easing EaseOutBack;
		static const Easing EaseInOutBack;

	private:
		EaseType myType {EaseType::None};
		T(*myEaseFunc)(T);
		T		myMin		{0};
		T		myMax		{0};
		T		myValue		{0};
		bool	myIsReverse{false};
	};

	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseNone(EaseType::None);

	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseInSine(EaseType::EaseInSine);
	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseOutSine(EaseType::EaseOutSine);
	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseInOutSine(EaseType::EaseInOutSine);

	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseInQuad(EaseType::EaseInQuad);
	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseOutQuad(EaseType::EaseOutQuad);
	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseInOutQuad(EaseType::EaseInOutQuad);

	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseInCubic(EaseType::EaseInCubic);
	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseOutCubic(EaseType::EaseOutCubic);
	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseInOutCubic(EaseType::EaseInOutCubic);

	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseInQuart(EaseType::EaseInQuart);
	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseOutQuart(EaseType::EaseOutQuart);
	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseInOutQuart(EaseType::EaseInOutQuart);

	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseInQuint(EaseType::EaseInQuint);
	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseOutQuint(EaseType::EaseOutQuint);
	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseInOutQuint(EaseType::EaseInOutQuint);

	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseInExpo(EaseType::EaseInExpo);
	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseOutExpo(EaseType::EaseOutExpo);
	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseInOutExpo(EaseType::EaseInOutExpo);

	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseInCirc(EaseType::EaseInCirc);
	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseOutCirc(EaseType::EaseOutCirc);
	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseInOutCirc(EaseType::EaseInOutCirc);

	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseInBack(EaseType::EaseInBack);
	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseOutBack(EaseType::EaseOutBack);
	template<IsFloatingPointType T>
	const Easing<T> Easing<T>::EaseInOutBack(EaseType::EaseInOutBack);

	using Easingf = Easing<float>;
}
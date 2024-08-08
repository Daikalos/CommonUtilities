#pragma once

#include <optional>

#include <CommonUtilities/Utility/Concepts.hpp>
#include <CommonUtilities/Config.h>

#include "Vector2.hpp"

namespace CommonUtilities
{
	template<IsArithmeticType T>
	class Rect
	{
	public:
		T left{}, bottom{}, right{}, top{};

		constexpr Rect() = default;
		constexpr ~Rect() = default;

		constexpr Rect(T aLeft, T aBottom, T aRight, T aTop);
		constexpr Rect(const Vector2<T>& aLeftBot, const Vector2<T>& aRightTop);

		NODISC constexpr static Rect InitWithMinAndMax(const Vector2<T>& aMin, const Vector2<T>& aMax);
		NODISC constexpr static Rect InitWithCenterAndSize(const Vector2<T>& aCenter, const Vector2<T>& aSize);

		template<IsArithmeticType U>
		constexpr explicit Rect(const Rect<U>& aRhs);

		NODISC constexpr Vector2<T> GetExtends() const;
		NODISC constexpr Vector2<T> GetSize() const;
		NODISC constexpr Vector2<T> GetCenter() const;

		NODISC constexpr T Width() const;
		NODISC constexpr T Height() const;

		NODISC constexpr T Area() const;

		constexpr void SetSize(const Vector2<T>& aSize);
		constexpr void SetCenter(const Vector2<T>& aCenter);

		NODISC constexpr Rect Union(const Rect& aOther) const;
		NODISC constexpr std::optional<Rect> Intersection(const Rect& aOther) const;

		NODISC constexpr bool Overlaps(const Rect& aOther) const;

		NODISC constexpr bool Contains(T aX, T aY) const;
		NODISC constexpr bool Contains(const Vector2<T>& aPosition) const;
		NODISC constexpr bool Contains(const Rect& aOther) const;
	};

	template<IsArithmeticType T>
	constexpr Rect<T>::Rect(T aLeft, T aBottom, T aRight, T aTop)
		: left(aLeft), bottom(aBottom), right(aRight), top(aTop)
	{

	}

	template<IsArithmeticType T>
	constexpr Rect<T>::Rect(const Vector2<T>& aLeftBot, const Vector2<T>& aRightTop)
		: Rect(aLeftBot.x, aLeftBot.y, aRightTop.x, aRightTop.y)
	{

	}

	template<IsArithmeticType T>
	constexpr Rect<T> Rect<T>::InitWithMinAndMax(const Vector2<T>& aMin, const Vector2<T>& aMax)
	{
		return Rect<T>(aMin, aMax);
	}
	template<IsArithmeticType T>
	constexpr Rect<T> Rect<T>::InitWithCenterAndSize(const Vector2<T>& aCenter, const Vector2<T>& aSize)
	{
		const Vector2<T> extends = aSize / 2.0f;
		return Rect<T>(aCenter - extends, aCenter + extends);
	}

	template<IsArithmeticType T>
	template<IsArithmeticType U>
	constexpr Rect<T>::Rect(const Rect<U>& aRhs)
		: Rect(aRhs.left, aRhs.bottom, aRhs.right, aRhs.top)
	{

	}

	template<IsArithmeticType T>
	constexpr Vector2<T> Rect<T>::GetExtends() const
	{
		return GetSize() / 2.0f;
	}
	template<IsArithmeticType T>
	constexpr Vector2<T> Rect<T>::GetSize() const
	{
		return Vector2<T>(Width(), Height());
	}
	template<IsArithmeticType T>
	constexpr Vector2<T> Rect<T>::GetCenter() const
	{
		return Vector2<T>(left + right, bottom + top) / 2.0f;
	}

	template<IsArithmeticType T>
	constexpr T Rect<T>::Width() const
	{
		return (right - left);
	}
	template<IsArithmeticType T>
	constexpr T Rect<T>::Height() const
	{
		return (top - bottom);
	}

	template<IsArithmeticType T>
	constexpr T Rect<T>::Area() const
	{
		return Width() * Height();
	}

	template<IsArithmeticType T>
	constexpr void Rect<T>::SetSize(const Vector2<T>& aSize)
	{
		const Vector2<T> center		= GetCenter();
		const Vector2<T> extends	= aSize / 2.0f;

		left	= center.x - extends.x;
		bottom	= center.y - extends.y;
		right	= center.x + extends.x;
		top		= center.y + extends.y;
	}
	template<IsArithmeticType T>
	constexpr void Rect<T>::SetCenter(const Vector2<T>& aCenter)
	{
		const Vector2<T> extends = GetSize() / 2.0f;

		left	= aCenter.x - extends.x;
		bottom	= aCenter.y - extends.y;
		right	= aCenter.x + extends.x;
		top		= aCenter.y + extends.y;
	}

	template<IsArithmeticType T>
	constexpr Rect<T> Rect<T>::Union(const Rect& aOther) const
	{
		// returns a rectangle that encompasses both rects

		constexpr auto Min = [](T aFirst, T aSecond) constexpr { return (aFirst < aSecond) ? aFirst : aSecond; };
		constexpr auto Max = [](T aFirst, T aSecond) constexpr { return (aFirst < aSecond) ? aSecond : aFirst; };

		const T r1l = Min(left, right);
		const T r1b = Min(top,  bottom);
		const T r1r = Max(left, right);
		const T r1t = Max(top,  bottom);

		const T r2l = Min(aOther.left,	aOther.right);
		const T r2b = Min(aOther.top,	aOther.bottom);
		const T r2r = Max(aOther.left,	aOther.right);
		const T r2t = Max(aOther.top,	aOther.bottom);

		const T r3l = Min(r1l, r2l);
		const T r3b = Min(r1b, r2b);
		const T r3r = Max(r1r, r2r);
		const T r3t = Max(r1t, r2t);

		return Rect<T>(r3l, r3b, r3r, r3t);
	}
	template<IsArithmeticType T>
	constexpr std::optional<Rect<T>> Rect<T>::Intersection(const Rect& aOther) const
	{
		constexpr auto Min = [](T aFirst, T aSecond) constexpr { return (aFirst < aSecond) ? aFirst : aSecond; };
		constexpr auto Max = [](T aFirst, T aSecond) constexpr { return (aFirst < aSecond) ? aSecond : aFirst; };

		const T r1l = Min(left, right);
		const T r1b = Min(top,  bottom);
		const T r1r = Max(left, right);
		const T r1t = Max(top,  bottom);

		const T r2l = Min(aOther.left,	aOther.right);
		const T r2b = Min(aOther.top,	aOther.bottom);
		const T r2r = Max(aOther.left,	aOther.right);
		const T r2t = Max(aOther.top,	aOther.bottom);

		const T interLeft   = Max(r1l, r2l);
		const T interBot    = Max(r1b, r2b);
		const T interRight  = Min(r1r, r2r);
		const T interTop	= Min(r1t, r2t);

		if ((interLeft < interRight) && (interBot < interTop))
		{
			return Rect<T>(interLeft, interBot, interRight, interTop);
		}

		return std::nullopt;
	}

	template<IsArithmeticType T>
	constexpr bool Rect<T>::Overlaps(const Rect& aOther) const
	{
		constexpr auto Min = [](T aFirst, T aSecond) constexpr { return (aFirst < aSecond) ? aFirst : aSecond; };
		constexpr auto Max = [](T aFirst, T aSecond) constexpr { return (aFirst < aSecond) ? aSecond : aFirst; };

		const T r1l = Min(left, right);
		const T r1b = Min(top,  bottom);
		const T r1r = Max(left, right);
		const T r1t = Max(top,  bottom);

		const T r2l = Min(aOther.left,	aOther.right);
		const T r2b = Min(aOther.top,	aOther.bottom);
		const T r2r = Max(aOther.left,	aOther.right);
		const T r2t = Max(aOther.top,	aOther.bottom);

		return !(r1l > r2r || r2l > r1r || r1b > r2t || r2b > r1t);
	}

	template<IsArithmeticType T>
	constexpr bool Rect<T>::Contains(T aX, T aY) const
	{
		constexpr auto Min = [](T aFirst, T aSecond) constexpr { return (aFirst < aSecond) ? aFirst : aSecond; };
		constexpr auto Max = [](T aFirst, T aSecond) constexpr { return (aFirst < aSecond) ? aSecond : aFirst; };

		const T r1l = Min(left, right);
		const T r1b = Min(top,  bottom);
		const T r1r = Max(left, right);
		const T r1t = Max(top,  bottom);

		return (aX >= r1l) && (aX < r1r) && (aY >= r1b) && (aY < r1t);
	}
	template<IsArithmeticType T>
	constexpr bool Rect<T>::Contains(const Vector2<T>& aPosition) const
	{
		return Contains(aPosition.x, aPosition.y);
	}
	template<IsArithmeticType T>
	constexpr bool Rect<T>::Contains(const Rect& aOther) const
	{
		constexpr auto Min = [](T aFirst, T aSecond) constexpr { return (aFirst < aSecond) ? aFirst : aSecond; };
		constexpr auto Max = [](T aFirst, T aSecond) constexpr { return (aFirst < aSecond) ? aSecond : aFirst; };

		const T r1l = Min(left, right);
		const T r1b = Min(top,  bottom);
		const T r1r = Max(left, right);
		const T r1t = Max(top,  bottom);

		const T r2l = Min(aOther.left,	aOther.right);
		const T r2b = Min(aOther.top,	aOther.bottom);
		const T r2r = Max(aOther.left,	aOther.right);
		const T r2t = Max(aOther.top,	aOther.bottom);

		return (r2l >= r1l && r2r < r1r && r2b >= r1b && r2t < r1t);
	}

	// GLOBAL OPERATORS

	template<IsArithmeticType T>
	NODISC constexpr Rect<T> operator-(const Rect<T>& aRect)
	{
		return Rect(-aRect.left, -aRect.bottom, -aRect.right, -aRect.top);
	}

	template<IsArithmeticType T>
	constexpr Rect<T>& operator+=(Rect<T>& aLeft, const Rect<T>& aRight) 
	{
		aLeft.left		+= aRight.left;
		aLeft.bottom	+= aRight.bottom;
		aLeft.right		+= aRight.right;
		aLeft.top		+= aRight.top;

		return aLeft;
	}
	template<IsArithmeticType T>
	constexpr Rect<T>& operator-=(Rect<T>& aLeft, const Rect<T>& aRight) 
	{
		aLeft.left		-= aRight.left;
		aLeft.bottom	-= aRight.bottom;
		aLeft.right		-= aRight.right;
		aLeft.top		-= aRight.top;

		return aLeft;
	}

	template<IsArithmeticType T>
	NODISC constexpr Rect<T> operator+(const Rect<T>& aLeft, const Rect<T>& aRight) 
	{
		return Rect<T>(
			aLeft.left		+ aRight.left,
			aLeft.bottom	+ aRight.bottom,
			aLeft.right		+ aRight.right, 
			aLeft.top		+ aRight.top);
	}
	template<IsArithmeticType T>
	NODISC constexpr Rect<T> operator-(const Rect<T>& aLeft, const Rect<T>& aRight) 
	{
		return Rect<T>(
			aLeft.left		- aRight.left,
			aLeft.bottom	- aRight.bottom,
			aLeft.right		- aRight.right,
			aLeft.top		- aRight.top);
	}

	template<IsArithmeticType T>
	NODISC constexpr bool operator==(const Rect<T>& aLeft, const Rect<T>& aRight) 
	{
		return (aLeft.left == aRight.left) && (aLeft.bottom == aRight.bottom) && (aLeft.right == aRight.right) && (aLeft.top == aRight.top);
	}
	template<IsArithmeticType T>
	NODISC constexpr bool operator!=(const Rect<T>& aLeft, const Rect<T>& aRight)
	{
		return !(aLeft == aRight);
	}

	using Rectf = Rect<float>;
	using Rectd = Rect<double>;
	using Recti = Rect<int>;
}
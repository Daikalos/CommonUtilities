#pragma once

#include <optional>

#include <CommonUtilities/Utility/Concepts.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<IsArithmetic T>
	class Rect
	{
	public:
		T left{}, top{}, width{}, height{};

		CONSTEXPR Rect() = default;
		CONSTEXPR ~Rect() = default;

		CONSTEXPR Rect(T aLeft, T aTop, T aWidth, T aHeight);

		template<IsArithmetic U>
		CONSTEXPR explicit Rect(const Rect<U>& aRhs);

		NODISC CONSTEXPR T Right() const;
		NODISC CONSTEXPR T Bottom() const;

		NODISC CONSTEXPR T Area() const;

		NODISC CONSTEXPR Rect Union(const Rect& aOther) const;
		NODISC CONSTEXPR std::optional<Rect> Intersection(const Rect& aOther) const;

		NODISC CONSTEXPR bool Overlaps(const Rect& aOther) const;

		NODISC CONSTEXPR bool Contains(T aX, T aY) const;
		NODISC CONSTEXPR bool Contains(const Rect& aOther) const;
	};

	template<IsArithmetic T>
	CONSTEXPR Rect<T>::Rect(T aLeft, T aTop, T aWidth, T aHeight)
		: left(aLeft), top(aTop), width(aWidth), height(aHeight) 
	{

	}

	template<IsArithmetic T>
	template<IsArithmetic U>
	CONSTEXPR Rect<T>::Rect(const Rect<U>& aRhs)
		: Rect(aRhs.left, aRhs.top, aRhs.width, aRhs.height)
	{

	}

	template<IsArithmetic T>
	CONSTEXPR T Rect<T>::Right() const
	{
		return static_cast<T>(left + width);
	}
	template<IsArithmetic T>
	CONSTEXPR T Rect<T>::Bottom() const
	{
		return static_cast<T>(top + height);
	}

	template<IsArithmetic T>
	CONSTEXPR T Rect<T>::Area() const
	{
		return width * height;
	}

	template<IsArithmetic T>
	CONSTEXPR Rect<T> Rect<T>::Union(const Rect& aOther) const
	{
		// returns a rectangle that encompasses both rects

		const auto min = [](T aFirst, T aSecond) { return (aFirst < aSecond) ? aFirst : aSecond; };
		const auto max = [](T aFirst, T aSecond) { return (aFirst < aSecond) ? aSecond : aFirst; };

		const T r1l = min(left, Right());
		const T r1t = min(top,	Bottom());
		const T r1r = max(left, Right());
		const T r1b = max(top,	Bottom());

		const T r2l = min(aOther.left,	aOther.Right());
		const T r2t = min(aOther.top,	aOther.Bottom());
		const T r2r = max(aOther.left,	aOther.Right());
		const T r2b = max(aOther.top,	aOther.Bottom());

		const T r3l = min(r1l, r2l);
		const T r3t = min(r1t, r2t);
		const T r3r = max(r1r, r2r);
		const T r3b = max(r1b, r2b);

		return Rect<T>(r3l, r3t, r3r - r3l, r3b - r3t);
	}
	template<IsArithmetic T>
	CONSTEXPR std::optional<Rect<T>> Rect<T>::Intersection(const Rect& aOther) const
	{
		const auto min = [](T aFirst, T aSecond) { return (aFirst < aSecond) ? aFirst : aSecond; };
		const auto max = [](T aFirst, T aSecond) { return (aFirst < aSecond) ? aSecond : aFirst; };

		const T r1l = min(left, Right());
		const T r1t = min(top,	Bottom());
		const T r1r = max(left, Right());
		const T r1b = max(top,	Bottom());

		const T r2l = min(aOther.left,	aOther.Right());
		const T r2t = min(aOther.top,	aOther.Bottom());
		const T r2r = max(aOther.left,	aOther.Right());
		const T r2b = max(aOther.top,	aOther.Bottom());

		const T interLeft   = max(r1MinX, r2MinX);
		const T interTop    = max(r1MinY, r2MinY);
		const T interRight  = min(r1MaxX, r2MaxX);
		const T interBottom = min(r1MaxY, r2MaxY);

		if ((interLeft < interRight) && (interTop < interBottom))
		{
			return Rect<T>({interLeft, interTop}, {interRight - interLeft, interBottom - interTop});
		}
		else
		{
			return std::nullopt;
		}
	}

	template<IsArithmetic T>
	CONSTEXPR bool Rect<T>::Overlaps(const Rect& aOther) const
	{
		const auto min = [](T aFirst, T aSecond) { return (aFirst < aSecond) ? aFirst : aSecond; };
		const auto max = [](T aFirst, T aSecond) { return (aFirst < aSecond) ? aSecond : aFirst; };

		const T r1l = min(left, Right());
		const T r1t = min(top,	Bottom());
		const T r1r = max(left, Right());
		const T r1b = max(top,	Bottom());

		const T r2l = min(aOther.left,	aOther.Right());
		const T r2t = min(aOther.top,	aOther.Bottom());
		const T r2r = max(aOther.left,	aOther.Right());
		const T r2b = max(aOther.top,	aOther.Bottom());

		return !(r1l > r2r || r2l > r1r || r1t > r2b || r2t > r1b);
	}

	template<IsArithmetic T>
	CONSTEXPR bool Rect<T>::Contains(T aX, T aY) const
	{
		const auto min = [](T aFirst, T aSecond) { return (aFirst < aSecond) ? aFirst : aSecond; };
		const auto max = [](T aFirst, T aSecond) { return (aFirst < aSecond) ? aSecond : aFirst; };

		const T r1l = min(left, Right());
		const T r1t = min(top,	Bottom());
		const T r1r = max(left, Right());
		const T r1b = max(top,	Bottom());

		return (aX >= r1l) && (aX < r1r) && (aY >= r1t) && (aY < r1b);
	}

	template<IsArithmetic T>
	CONSTEXPR bool Rect<T>::Contains(const Rect& aOther) const
	{
		const auto min = [](T aFirst, T aSecond) { return (aFirst < aSecond) ? aFirst : aSecond; };
		const auto max = [](T aFirst, T aSecond) { return (aFirst < aSecond) ? aSecond : aFirst; };

		const T r1l = min(left, Right());
		const T r1t = min(top,	Bottom());
		const T r1r = max(left, Right());
		const T r1b = max(top,	Bottom());

		const T r2l = min(aOther.left,	aOther.Right());
		const T r2t = min(aOther.top,	aOther.Bottom());
		const T r2r = max(aOther.left,	aOther.Right());
		const T r2b = max(aOther.top,	aOther.Bottom());

		return (r2l >= r1l && r2r < r1r && r2t >= r1t && r2b < r1b);
	}

	// GLOBAL OPERATORS

	template<IsArithmetic T>
	NODISC CONSTEXPR Rect<T> operator-(const Rect<T>& aRect)
	{
		return Rect(-aRect.left, -aRect.top, -aRect.width, -aRect.height);
	}

	template<IsArithmetic T>
	CONSTEXPR Rect<T>& operator+=(Rect<T>& aLeft, const Rect<T>& aRight) 
	{
		aLeft.left		+= aRhs.left;
		aLeft.top		+= aRhs.top;
		aLeft.width		+= aRhs.width;
		aLeft.height	+= aRhs.height;

		return aLeft;
	}
	template<IsArithmetic T>
	CONSTEXPR Rect<T>& operator-=(Rect<T>& aLeft, const Rect<T>& aRight) 
	{
		aLeft.left		-= aRhs.left;
		aLeft.top		-= aRhs.top;
		aLeft.width		-= aRhs.width;
		aLeft.height	-= aRhs.height;

		return aLeft;
	}

	template<IsArithmetic T>
	NODISC CONSTEXPR Rect<T> operator+(const Rect<T>& aLeft, const Rect<T>& aRight) 
	{
		return Rect<T>(aLeft.left + aRight.left, aLeft.top + aRight.top, aLeft.width + aRight.width, aLeft.height + aRight.height);
	}
	template<IsArithmetic T>
	NODISC CONSTEXPR Rect<T> operator-(const Rect<T>& aLeft, const Rect<T>& aRight) 
	{
		return Rect<T>(aLeft.left - aRight.left, aLeft.top - aRight.top, aLeft.width - aRight.width, aLeft.height - aRight.height);
	}

	template<IsArithmetic T>
	NODISC CONSTEXPR bool operator==(const Rect<T>& aLeft, const Rect<T>& aRight) 
	{
		return (aLeft.left == aRight.left) && (aLeft.top == aRight.top) && (aLeft.width == aRight.width) && (aLeft.height == aRight.height);
	}
	template<IsArithmetic T>
	NODISC CONSTEXPR bool operator!=(const Rect<T>& aLeft, const Rect<T>& aRight)
	{
		return !(aLeft == aRight);
	}

	using RectInt = Rect<int>;
	using RectFloat = Rect<float>;
}
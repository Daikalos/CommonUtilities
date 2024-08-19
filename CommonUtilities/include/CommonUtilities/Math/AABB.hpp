#pragma once

#include <cassert>
#include <optional>

#include "Vector3.hpp"
#include "Shape.h"

#include <CommonUtilities/Utility/Clonable.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T>
	class AABB final : public Clonable<Shape, AABB<T>>
	{
	public:
		constexpr AABB() = default;
		constexpr ~AABB() = default;

		constexpr AABB(const Vector3<T>& aMin, const Vector3<T>& aMax);
		constexpr AABB(T aMinX, T aMinY, T aMinZ, T aMaxX, T aMaxY, T aMaxZ);

		NODISC constexpr static AABB InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax);
		NODISC constexpr static AABB InitWithCenterAndSize(const Vector3<T>& aCenter, const Vector3<T>& aSize);

		NODISC constexpr const Vector3<T>& GetMin() const noexcept;
		NODISC constexpr const Vector3<T>& GetMax() const noexcept;
		NODISC constexpr Vector3<T> GetExtends() const;
		NODISC constexpr Vector3<T> GetSize() const;
		NODISC constexpr Vector3<T> GetCenter() const;

		constexpr void SetMin(const Vector3<T>& aMin);
		constexpr void SetMax(const Vector3<T>& aMax);
		constexpr void SetSize(const Vector3<T>& aSize);
		constexpr void SetCenter(const Vector3<T>& aCenter);

		NODISC constexpr bool IsInside(const Vector3<T>& aPosition) const;

		NODISC constexpr AABB Union(const AABB& aOther) const;
		NODISC constexpr std::optional<AABB> Intersection(const AABB& aOther) const;

		NODISC constexpr bool Overlaps(const AABB& aOther) const;

		NODISC constexpr bool Contains(T aX, T aY, T aZ) const;
		NODISC constexpr bool Contains(const Vector3<T>& aPosition) const;
		NODISC constexpr bool Contains(const AABB& aOther) const;

		NODISC constexpr Shape::Type GetType() const noexcept override;

	private:
		Vector3<T> myMin;
		Vector3<T> myMax;
	};

	template<typename T>
	constexpr AABB<T>::AABB(const Vector3<T>& aMin, const Vector3<T>& aMax)
		: myMin((assert(aMax.x >= aMin.x && aMax.y >= aMin.y && "Maximum cannot be smaller than minimum"), aMin))
		, myMax(aMax)
	{

	}
	template<typename T>
	constexpr AABB<T>::AABB(T aMinX, T aMinY, T aMinZ, T aMaxX, T aMaxY, T aMaxZ)
		: AABB({ aMinX, aMinY, aMinZ }, { aMaxX, aMaxY, aMaxZ })
	{

	}

	template<typename T>
	constexpr AABB<T> AABB<T>::InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		return AABB<T>(aMin, aMax);
	}
	template<typename T>
	constexpr AABB<T> AABB<T>::InitWithCenterAndSize(const Vector3<T>& aCenter, const Vector3<T>& aSize)
	{
		const Vector3<T> extends = aSize / 2.0f;
		return AABB<T>(aCenter - extends, aCenter + extends);
	}

	template<typename T>
	constexpr const Vector3<T>& AABB<T>::GetMin() const noexcept
	{
		return myMin;
	}
	template<typename T>
	constexpr const Vector3<T>& AABB<T>::GetMax() const noexcept
	{
		return myMax;
	}
	template<typename T>
	constexpr Vector3<T> AABB<T>::GetExtends() const
	{
		return GetSize() / 2.0f;
	}
	template<typename T>
	constexpr Vector3<T> AABB<T>::GetSize() const
	{
		return (myMax - myMin);
	}
	template<typename T>
	constexpr Vector3<T> AABB<T>::GetCenter() const
	{
		return (myMin + myMax) / 2.0f;
	}

	template<typename T>
	constexpr void AABB<T>::SetMin(const Vector3<T>& aMin)
	{
		assert(myMax.x >= aMin.x && myMax.y >= aMin.y && myMax.z >= aMin.z && "Maximum cannot be smaller than minimum");
		myMin = aMin;
	}
	template<typename T>
	constexpr void AABB<T>::SetMax(const Vector3<T>& aMax)
	{
		assert(aMax.x >= myMin.x && aMax.y >= myMin.y && aMax.z >= myMin.z && "Maximum cannot be smaller than minimum");
		myMax = aMax;
	}
	template<typename T>
	constexpr void AABB<T>::SetSize(const Vector3<T>& aSize)
	{
		const Vector3<T> center = GetCenter();
		const Vector3<T> extends = aSize / 2.0f;

		myMax = center + extends;
		myMin = center - extends;
	}
	template<typename T>
	constexpr void AABB<T>::SetCenter(const Vector3<T>& aCenter)
	{
		const Vector3<T> extends = GetExtends();

		myMax = aCenter + extends;
		myMin = aCenter - extends;
	}

	template<typename T>
	constexpr bool AABB<T>::IsInside(const Vector3<T>& aPosition) const
	{
		return 
			(aPosition.x >= myMin.x) && (aPosition.x <= myMax.x) && 
			(aPosition.y >= myMin.y) && (aPosition.y <= myMax.y) && 
			(aPosition.z >= myMin.z) && (aPosition.z <= myMax.z);
	}

	template<typename T>
	constexpr AABB<T> AABB<T>::Union(const AABB& aOther) const
	{
		constexpr auto Min = [](T aFirst, T aSecond) constexpr { return (aFirst < aSecond) ? aFirst : aSecond; };
		constexpr auto Max = [](T aFirst, T aSecond) constexpr { return (aFirst < aSecond) ? aSecond : aFirst; };

		const T left	= Min(myMin.x, aOther.myMin.x);
		const T bot		= Min(myMin.y, aOther.myMin.y);
		const T back	= Min(myMin.z, aOther.myMin.z);
		const T right	= Max(myMax.x, aOther.myMax.x);
		const T top		= Max(myMax.y, aOther.myMax.y);
		const T front	= Max(myMax.z, aOther.myMax.z);

		return AABB<T>(left, bot, back, right, top, front);
	}
	template<typename T>
	constexpr std::optional<AABB<T>> AABB<T>::Intersection(const AABB& aOther) const
	{
		constexpr auto Min = [](T aFirst, T aSecond) constexpr { return (aFirst < aSecond) ? aFirst : aSecond; };
		constexpr auto Max = [](T aFirst, T aSecond) constexpr { return (aFirst < aSecond) ? aSecond : aFirst; };

		const T interLeft   = Max(myMin.x, aOther.myMin.x);
		const T interBot    = Max(myMin.y, aOther.myMin.y);
		const T interBack   = Max(myMin.z, aOther.myMin.z);
		const T interRight  = Min(myMax.x, aOther.myMax.x);
		const T interTop	= Min(myMax.y, aOther.myMax.y);
		const T interFront	= Min(myMax.z, aOther.myMax.z);

		if ((interLeft < interRight) && (interBot < interTop) && (interBack < interFront))
		{
			return AABB<T>(interLeft, interBot, interBack, interRight, interTop, interFront);
		}

		return std::nullopt;
	}

	template<typename T>
	constexpr bool AABB<T>::Overlaps(const AABB& aOther) const
	{
		return !(myMin.x > aOther.myMax.x || aOther.myMin.x > myMax.x ||
				 myMin.y > aOther.myMax.y || aOther.myMin.y > myMax.y || 
				 myMin.z > aOther.myMax.z || aOther.myMin.z > myMax.z);
	}

	template<typename T>
	constexpr bool AABB<T>::Contains(T aX, T aY, T aZ) const
	{
		return	(aX >= myMin.x) && (aX < myMax.x) &&
				(aY >= myMin.y) && (aY < myMax.y) &&
				(aZ >= myMin.z) && (aZ < myMax.z);
	}
	template<typename T>
	constexpr bool AABB<T>::Contains(const Vector3<T>& aPosition) const
	{
		return Contains(aPosition.x, aPosition.y, aPosition.z);
	}
	template<typename T>
	constexpr bool AABB<T>::Contains(const AABB& aOther) const
	{
		return	(aOther.myMin.x >= myMin.x && aOther.myMax.x < myMax.x) && 
				(aOther.myMin.y >= myMin.y && aOther.myMax.y < myMax.y) &&
				(aOther.myMin.z >= myMin.z && aOther.myMax.z < myMax.z);
	}

	template<typename T>
	constexpr Shape::Type AABB<T>::GetType() const noexcept
	{
		return Shape::Type::AABB;
	}

	using AABBf	= AABB<float>;
	using AABBd	= AABB<double>;
	using AABBi	= AABB<int>;
}
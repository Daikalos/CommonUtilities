#pragma once

#include "Vector3.hpp"
#include "Shape.h"

#include <CommonUtilities/Utility/Clonable.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T>
	class AABB;

	template<typename T>
	class Capsule final : public Clonable<Shape, Capsule<T>>
	{
	public:
		constexpr Capsule() = default;
		constexpr ~Capsule() = default;

		constexpr Capsule(const Vector3<T>& aBase, const Vector3<T>& aTip, T aRadius);

		NODISC constexpr const Vector3<T>& GetBase() const noexcept;
		NODISC constexpr const Vector3<T>& GetTip() const noexcept;
		NODISC constexpr T GetRadius() const noexcept;
		NODISC constexpr T GetRadiusSqr() const noexcept;
		NODISC constexpr T GetBodyLength() const;
		NODISC constexpr T GetBodyLengthSqr() const;

		constexpr void SetBase(const Vector3<T>& aBase);
		constexpr void SetTip(const Vector3<T>& aTip);
		constexpr void SetRadius(T aRadius);

		NODISC constexpr AABB<T> GetAABB() const;

		NODISC constexpr bool IsInside(const Vector3<T>& aPosition) const;

		NODISC constexpr bool Overlaps(const Capsule& aOther) const;

		NODISC constexpr bool Contains(T aX, T aY, T aZ) const;
		NODISC constexpr bool Contains(const Vector3<T>& aPosition) const;
		NODISC constexpr bool Contains(const Capsule& aOther) const;

		NODISC constexpr Shape::Type GetType() const noexcept override;

	private:
		Vector3<T>	myBase;
		Vector3<T>	myTip;
		T			myRadius{};
		T			myRadiusSqr{};
	};

	template<typename T>
	constexpr Capsule<T>::Capsule(const Vector3<T>& aBase, const Vector3<T>& aTip, T aRadius)
		: myBase(aBase)
		, myTip(aTip)
		, myRadius(aRadius)
		, myRadiusSqr(aRadius* aRadius)
	{

	}

	template<typename T>
	constexpr const Vector3<T>& Capsule<T>::GetBase() const noexcept
	{
		return myBase;
	}
	template<typename T>
	constexpr const Vector3<T>& Capsule<T>::GetTip() const noexcept
	{
		return myTip;
	}
	template<typename T>
	constexpr T Capsule<T>::GetRadius() const noexcept
	{
		return myRadius;
	}
	template<typename T>
	constexpr T Capsule<T>::GetRadiusSqr() const noexcept
	{
		return myRadiusSqr;
	}
	template<typename T>
	constexpr T Capsule<T>::GetBodyLength() const
	{
		return Vector3<T>::Distance(myBase, myTip);
	}
	template<typename T>
	constexpr T Capsule<T>::GetBodyLengthSqr() const
	{
		return Vector3<T>::DistanceSqr(myBase, myTip);
	}

	template<typename T>
	constexpr void Capsule<T>::SetBase(const Vector3<T>& aBase)
	{
		myBase = aBase;
	}
	template<typename T>
	constexpr void Capsule<T>::SetTip(const Vector3<T>& aTip)
	{
		myTip = aTip;
	}
	template<typename T>
	constexpr void Capsule<T>::SetRadius(T aRadius)
	{
		myRadius = aRadius;
		myRadiusSqr = aRadius * aRadius;
	}

	template<typename T>
	constexpr AABB<T> Capsule<T>::GetAABB() const
	{
		Vector3<T> bCorner = GetBase() - Vector3<T>(GetRadius());
		Vector3<T> tCorner = GetTip() + Vector3<T>(GetRadius());

		Vector3<T> min;
		Vector3<T> max;

		min.x = Min(bCorner.x, tCorner.x), min.y = Min(bCorner.y, tCorner.y), min.z = Min(bCorner.z, tCorner.z);
		max.x = Max(bCorner.x, tCorner.x), max.y = Max(bCorner.y, tCorner.y), max.z = Max(bCorner.z, tCorner.z);

		return AABB<T>(min, max);
	}

	template<typename T>
	constexpr bool Capsule<T>::IsInside(const Vector3<T>& aPosition) const
	{
		return Vector3<T>::DistanceSqrToSegment(GetBase(), GetTip(), aPosition) <= myRadiusSqr;
	}

	template<typename T>
	constexpr bool Capsule<T>::Overlaps(const Capsule& aOther) const
	{
		auto [p1, p2] = Vector3<T>::ClosestPointsSegmentSegment(GetTip(), GetBase(), aOther.GetTip(), aOther.GetBase());

		const T distSqr = Vector3<T>::DistanceSqr(p1, p2);
		const T radius  = GetRadius() + aOther.GetRadius();

		return distSqr < radius * radius;
	}

	template<typename T>
	constexpr bool Capsule<T>::Contains(T aX, T aY, T aZ) const
	{
		return Contains(Vector3<T>(aX, aY, aZ));
	}
	template<typename T>
	constexpr bool Capsule<T>::Contains(const Vector3<T>& aPosition) const
	{
		return Vector3<T>::DistanceSqrToSegment(GetBase(), GetTip(), aPosition) < myRadiusSqr;
	}
	template<typename T>
	constexpr bool Capsule<T>::Contains(const Capsule& aOther) const
	{
		auto [p1, p2] = Vector3<T>::ClosestPointsSegmentSegment(GetTip(), GetBase(), aOther.GetTip(), aOther.GetBase());

		const T distSqr = Vector3<T>::DistanceSqr(p1, p2);
		const T radius  = GetRadius() - aOther.GetRadius();

		return distSqr < radius * radius;
	}

	template<typename T>
	constexpr Shape::Type Capsule<T>::GetType() const noexcept
	{
		return Shape::Type::Capsule;
	}

	using Capsulef = Capsule<float>;
	using Capsuled = Capsule<double>;
	using Capsulei = Capsule<int>;
}
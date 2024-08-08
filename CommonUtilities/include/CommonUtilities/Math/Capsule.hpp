#pragma once

#include "Vector3.hpp"
#include "Shape.h"

#include <CommonUtilities/Utility/Clonable.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T>
	class Capsule final : public Clonable<Shape, Capsule<T>>
	{
	public:
		constexpr Capsule() = default;
		constexpr ~Capsule() = default;

		constexpr Capsule(const Vector3<T>& aCenter, T aRadius, T aHeight);

		NODISC constexpr const Vector3<T>& GetCenter() const noexcept;
		NODISC constexpr T GetRadius() const noexcept;
		NODISC constexpr T GetRadiusSqr() const noexcept;
		NODISC constexpr T GetHeight() const noexcept;

		constexpr void SetCenter(const Vector3<T>& aCenter);
		constexpr void SetRadius(T aRadius);
		constexpr void SetHeight(T aHeight);

		NODISC constexpr bool IsInside(const Vector3<T>& aPosition) const;

		NODISC constexpr bool Overlaps(const Capsule& aOther) const;

		NODISC constexpr bool Contains(T aX, T aY, T aZ) const;
		NODISC constexpr bool Contains(const Vector3<T>& aPosition) const;
		NODISC constexpr bool Contains(const Capsule& aOther) const;

		NODISC constexpr Shape::Type GetType() const noexcept override;

	private:
		Vector3<T>	myCenter;
		T			myRadius{};
		T			myRadiusSqr{};
		T			myHeight{};
	};

	template<typename T>
	constexpr Capsule<T>::Capsule(const Vector3<T>& aCenter, T aRadius, T aHeight)
		: myCenter(aCenter)
		, myRadius(aRadius)
		, myRadiusSqr(aRadius* aRadius)
		, myHeight(aHeight)
	{

	}

	template<typename T>
	constexpr const Vector3<T>& Capsule<T>::GetCenter() const noexcept
	{
		return myCenter;
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
	constexpr T Capsule<T>::GetHeight() const noexcept
	{
		return myHeight;
	}

	template<typename T>
	constexpr void Capsule<T>::SetCenter(const Vector3<T>& aCenter)
	{
		myCenter = aCenter;
	}
	template<typename T>
	constexpr void Capsule<T>::SetRadius(T aRadius)
	{
		myRadius = aRadius;
		myRadiusSqr = aRadius * aRadius;
	}
	template<typename T>
	constexpr void Capsule<T>::SetHeight(T aHeight)
	{
		myHeight = aHeight;
	}

	template<typename T>
	constexpr bool Capsule<T>::IsInside(const Vector3<T>& aPosition) const
	{
		return Vector3<T>::DistanceSqr(GetCenter(), aPosition) <= myRadiusSqr;
	}

	template<typename T>
	constexpr bool Capsule<T>::Overlaps(const Capsule& aOther) const
	{
		Vector3<T> dir = Vector3<T>::Direction(GetCenter(), aOther.GetCenter());

		const T distSqr = dir.LengthSqr();
		const T radius = GetRadius() + aOther.GetRadius();

		return distSqr < radius * radius;
	}

	template<typename T>
	constexpr bool Capsule<T>::Contains(T aX, T aY, T aZ) const
	{
		return Contains(cu::Vector3f(aX, aY, aZ));
	}
	template<typename T>
	constexpr bool Capsule<T>::Contains(const Vector3<T>& aPosition) const
	{
		return Vector3<T>::DistanceSqr(GetCenter(), aPosition) < myRadiusSqr;
	}
	template<typename T>
	constexpr bool Capsule<T>::Contains(const Capsule& aOther) const
	{
		Vector3<T> dir = Vector3<T>::Direction(GetCenter(), aOther.GetCenter());

		const T distSqr = dir.LengthSqr();
		const T radius = GetRadius() - aOther.GetRadius();

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
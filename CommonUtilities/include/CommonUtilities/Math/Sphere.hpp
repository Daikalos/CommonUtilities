#pragma once

#include "Vector3.hpp"
#include "Shape.h"

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T>
	class Sphere final : public Shape
	{
	public:
		CONSTEXPR Sphere() = default;
		CONSTEXPR ~Sphere() = default;

		CONSTEXPR Sphere(const Vector3<T>& aCenter, T aRadius);

		CONSTEXPR static Sphere<T> InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius);

		NODISC CONSTEXPR const Vector3<T>& GetCenter() const noexcept;
		NODISC CONSTEXPR T GetRadius() const noexcept;
		NODISC CONSTEXPR T GetRadiusSqr() const noexcept;

		CONSTEXPR void SetCenter(const Vector3<T>& aCenter);
		CONSTEXPR void SetRadius(T aRadius);

		NODISC CONSTEXPR bool IsInside(const Vector3<T>& aPosition) const;

		NODISC CONSTEXPR auto GetType() const noexcept -> Type override;

	private:
		Vector3<T>	myCenter;
		T			myRadius	{T()};
		T			myRadiusSqr	{T()};
	};

	template<typename T>
	CONSTEXPR Sphere<T>::Sphere(const Vector3<T>& aCenter, T aRadius)
		: myCenter(aCenter), myRadius(aRadius), myRadiusSqr(aRadius * aRadius)
	{

	}

	template<typename T>
	CONSTEXPR Sphere<T> Sphere<T>::InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius)
	{
		return Sphere<T>(aCenter, aRadius);
	}

	template<typename T>
	CONSTEXPR const Vector3<T>& Sphere<T>::GetCenter() const noexcept
	{
		return myCenter;
	}
	template<typename T>
	CONSTEXPR T Sphere<T>::GetRadius() const noexcept
	{
		return myRadius;
	}
	template<typename T>
	CONSTEXPR T Sphere<T>::GetRadiusSqr() const noexcept
	{
		return myRadiusSqr;
	}

	template<typename T>
	CONSTEXPR void Sphere<T>::SetCenter(const Vector3<T>& aCenter)
	{
		myCenter = aCenter;
	}
	template<typename T>
	CONSTEXPR void Sphere<T>::SetRadius(T aRadius)
	{
		myRadius = aRadius;
		myRadiusSqr = aRadius * aRadius;
	}

	template<typename T>
	CONSTEXPR bool Sphere<T>::IsInside(const Vector3<T>& aPosition) const
	{
		return Vector3<T>::DistanceSqr(myCenter, aPosition) <= myRadiusSqr;
	}

	template<typename T>
	CONSTEXPR auto Sphere<T>::GetType() const noexcept -> Type
	{
		return Type::Sphere;
	}

	using SphereFloat	= Sphere<float>;
	using SphereDouble	= Sphere<double>;
	using SphereInt		= Sphere<int>;
}
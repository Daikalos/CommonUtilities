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
		constexpr Sphere() = default;
		constexpr ~Sphere() = default;

		constexpr Sphere(const Vector3<T>& aCenter, T aRadius);

		NODISC constexpr static Sphere<T> InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius);

		NODISC constexpr const Vector3<T>& GetCenter() const noexcept;
		NODISC constexpr T GetRadius() const noexcept;
		NODISC constexpr T GetRadiusSqr() const noexcept;

		constexpr void SetCenter(const Vector3<T>& aCenter);
		constexpr void SetRadius(T aRadius);

		NODISC constexpr bool IsInside(const Vector3<T>& aPosition) const;

		NODISC constexpr auto GetType() const noexcept -> Type override;
		NODISC constexpr std::unique_ptr<Shape> Clone() const override;

	private:
		Vector3<T>	myCenter;
		T			myRadius	{T()};
		T			myRadiusSqr	{T()};
	};

	template<typename T>
	constexpr Sphere<T>::Sphere(const Vector3<T>& aCenter, T aRadius)
		: myCenter(aCenter), myRadius(aRadius), myRadiusSqr(aRadius * aRadius)
	{

	}

	template<typename T>
	constexpr Sphere<T> Sphere<T>::InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius)
	{
		return Sphere<T>(aCenter, aRadius);
	}

	template<typename T>
	constexpr const Vector3<T>& Sphere<T>::GetCenter() const noexcept
	{
		return myCenter;
	}
	template<typename T>
	constexpr T Sphere<T>::GetRadius() const noexcept
	{
		return myRadius;
	}
	template<typename T>
	constexpr T Sphere<T>::GetRadiusSqr() const noexcept
	{
		return myRadiusSqr;
	}

	template<typename T>
	constexpr void Sphere<T>::SetCenter(const Vector3<T>& aCenter)
	{
		myCenter = aCenter;
	}
	template<typename T>
	constexpr void Sphere<T>::SetRadius(T aRadius)
	{
		myRadius = aRadius;
		myRadiusSqr = aRadius * aRadius;
	}

	template<typename T>
	constexpr bool Sphere<T>::IsInside(const Vector3<T>& aPosition) const
	{
		return Vector3<T>::DistanceSqr(myCenter, aPosition) <= myRadiusSqr;
	}

	template<typename T>
	constexpr auto Sphere<T>::GetType() const noexcept -> Type
	{
		return Type::Sphere;
	}
	template<typename T>
	constexpr std::unique_ptr<Shape> Sphere<T>::Clone() const
	{
		return std::make_unique<Sphere<T>>(*this);
	}

	using SphereFloat	= Sphere<float>;
	using SphereDouble	= Sphere<double>;
	using SphereInt		= Sphere<int>;
}
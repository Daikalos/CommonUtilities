#pragma once

#include "Vector3.hpp"
#include "Shape.h"

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T>
	class Plane : public Shape
	{
	public:
		constexpr Plane() = default;
		constexpr ~Plane() = default;

		constexpr Plane(const Vector3<T>& aPoint, const Vector3<T>& aNormal);
		constexpr Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);

		NODISC constexpr static Plane<T> InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal);
		NODISC constexpr static Plane<T> InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);

		NODISC constexpr const Vector3<T>& GetOrigin() const noexcept;
		NODISC constexpr const Vector3<T>& GetNormal() const noexcept;

		constexpr void SetOrigin(const Vector3<T>& aOrigin);
		constexpr void SetNormal(const Vector3<T>& aNormal);

		NODISC constexpr bool IsInside(const Vector3<T>& aPosition) const;

		NODISC constexpr auto GetType() const noexcept -> Type override;
		NODISC constexpr std::unique_ptr<Shape> Clone() const override;

	private:
		Vector3<T> myOrigin;
		Vector3<T> myNormal;
	};

	template<typename T>
	constexpr Plane<T>::Plane(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
		: myOrigin(aPoint)
		, myNormal(aNormal.GetNormalized()) // make sure is normalized
	{

	}
	template<typename T>
	constexpr Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
		: Plane<T>(aPoint0, (aPoint1 - aPoint0).Cross(aPoint2 - aPoint0))
	{

	}

	template<typename T>
	constexpr Plane<T> Plane<T>::InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
	{
		return Plane<T>(aPoint, aNormal);
	}
	template<typename T>
	constexpr Plane<T> Plane<T>::InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		return Plane<T>(aPoint0, aPoint1, aPoint2);
	}

	template<typename T>
	constexpr const Vector3<T>& Plane<T>::GetOrigin() const noexcept
	{
		return myOrigin;
	}
	template<typename T>
	constexpr const Vector3<T>& Plane<T>::GetNormal() const noexcept
	{
		return myNormal;
	}

	template<typename T>
	constexpr void Plane<T>::SetOrigin(const Vector3<T>& aOrigin)
	{
		myOrigin = aOrigin;
	}
	template<typename T>
	constexpr void Plane<T>::SetNormal(const Vector3<T>& aNormal)
	{
		myNormal = aNormal.GetNormalized();
	}

	template<typename T>
	constexpr bool Plane<T>::IsInside(const Vector3<T>& aPosition) const
	{
		return Vector3<T>::Direction(myOrigin, aPosition).Dot(GetNormal()) <= T{};
	}

	template<typename T>
	constexpr auto Plane<T>::GetType() const noexcept -> Type
	{
		return Type::Plane;
	}
	template<typename T>
	constexpr std::unique_ptr<Shape> Plane<T>::Clone() const
	{
		return std::make_unique<Plane<T>>(*this);
	}

	// using declarations

	using PlaneFloat	= Plane<float>;
	using PlaneDouble	= Plane<double>;
	using PlaneInt		= Plane<int>;
}
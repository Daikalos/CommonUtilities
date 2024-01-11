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
		CONSTEXPR Plane();
		CONSTEXPR ~Plane();

		CONSTEXPR Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);
		CONSTEXPR Plane(const Vector3<T>& aPoint, const Vector3<T>& aNormal);

		CONSTEXPR void InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);
		CONSTEXPR void InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal);

		NODISC CONSTEXPR const Vector3<T>& GetOrigin() const noexcept;
		NODISC CONSTEXPR const Vector3<T>& GetNormal() const noexcept;

		CONSTEXPR void SetOrigin(const Vector3<T>& aOrigin);
		CONSTEXPR void SetNormal(const Vector3<T>& aNormal);

		NODISC CONSTEXPR bool IsInside(const Vector3<T>& aPosition) const;

		NODISC CONSTEXPR auto GetType() const noexcept -> Type override;

	private:
		Vector3<T> myOrigin;
		Vector3<T> myNormal;
	};

	template<typename T>
	CONSTEXPR Plane<T>::Plane() = default;
	template<typename T>
	CONSTEXPR Plane<T>::~Plane() = default;

	template<typename T>
	CONSTEXPR Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		InitWith3Points(aPoint0, aPoint1, aPoint2);
	}
	template<typename T>
	CONSTEXPR Plane<T>::Plane(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
	{
		InitWithPointAndNormal(aPoint, aNormal);
	}

	template<typename T>
	CONSTEXPR void Plane<T>::InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		myOrigin = aPoint0;

		Vector3<T> originToPoint1 = Vector3<T>::Direction(myOrigin, aPoint1);
		Vector3<T> originToPoint2 = Vector3<T>::Direction(myOrigin, aPoint2);

		myNormal = originToPoint1.Cross(originToPoint2).GetNormalized();
	}
	template<typename T>
	CONSTEXPR void Plane<T>::InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
	{
		myOrigin = aPoint;
		myNormal = aNormal.GetNormalized(); // make sure is normalized
	}

	template<typename T>
	CONSTEXPR const Vector3<T>& Plane<T>::GetOrigin() const noexcept
	{
		return myOrigin;
	}
	template<typename T>
	CONSTEXPR const Vector3<T>& Plane<T>::GetNormal() const noexcept
	{
		return myNormal;
	}

	template<typename T>
	CONSTEXPR void Plane<T>::SetOrigin(const Vector3<T>& aOrigin)
	{
		myOrigin = aOrigin;
	}
	template<typename T>
	CONSTEXPR void Plane<T>::SetNormal(const Vector3<T>& aNormal)
	{
		myNormal = aNormal.GetNormalized();
	}

	template<typename T>
	CONSTEXPR bool Plane<T>::IsInside(const Vector3<T>& aPosition) const
	{
		return Vector3<T>::Direction(myOrigin, aPosition).Dot(GetNormal()) <= T{};
	}

	template<typename T>
	CONSTEXPR auto Plane<T>::GetType() const noexcept -> Type
	{
		return Type::Plane;
	}

	// using declarations

	using PlaneFloat	= Plane<float>;
	using PlaneDouble	= Plane<double>;
	using PlaneInt		= Plane<int>;
}
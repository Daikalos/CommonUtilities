#pragma once

#include <cmath>

#include "Vector3.hpp"
#include "Shape.h"

#include <CommonUtilities/Utility/Clonable.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T>
	class Plane : public Clonable<Shape, Plane<T>>
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
		NODISC constexpr T GetDistance() const noexcept;

		constexpr void SetOrigin(const Vector3<T>& aOrigin);
		constexpr void SetNormal(const Vector3<T>& aNormal);

		NODISC constexpr Vector3<T> ClosestPoint(const Vector3<T>& aPoint);

		NODISC constexpr bool IsInside(const Vector3<T>& aPosition) const;
		NODISC constexpr bool IsInside(const Vector3<T>& aPosition, T aRadius) const;
		NODISC constexpr bool IsInside(const Vector3<T>& aPosition, const Vector3<T>& aExtends) const;

		NODISC constexpr Shape::Type GetType() const noexcept override;

	private:
		Vector3<T>	myOrigin;
		Vector3<T>	myNormal;
		T			myDistance {0};
	};

	template<typename T>
	constexpr Plane<T>::Plane(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
		: myOrigin(aPoint)
		, myNormal(aNormal.GetNormalized()) // make sure is normalized
		, myDistance(-Vector3<T>::Dot(myNormal, myOrigin))
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
	constexpr T Plane<T>::GetDistance() const noexcept
	{
		return myDistance;
	}

	template<typename T>
	constexpr void Plane<T>::SetOrigin(const Vector3<T>& aOrigin)
	{
		myOrigin	= aOrigin;
		myDistance	= -Vector3<T>::Dot(myNormal, myOrigin);
	}
	template<typename T>
	constexpr void Plane<T>::SetNormal(const Vector3<T>& aNormal)
	{
		myNormal	= aNormal.GetNormalized();
		myDistance	= -Vector3<T>::Dot(myNormal, myOrigin);
	}

	template<typename T>
	constexpr Vector3<T> Plane<T>::ClosestPoint(const Vector3<T>& aPoint)
	{
		const T distance = Vector3<T>::Dot(GetNormal(), aPoint) - myDistance;
		return aPoint - distance * GetNormal();
	}

	template<typename T>
	constexpr bool Plane<T>::IsInside(const Vector3<T>& aPosition) const
	{
		return Vector3<T>::Direction(GetOrigin(), aPosition).Dot(GetNormal()) <= T{};
	}
	template<typename T>
	constexpr bool Plane<T>::IsInside(const Vector3<T>& aPosition, T aRadius) const
	{
		return Vector3<T>::Direction(GetOrigin(), aPosition).Dot(GetNormal()) <= aRadius;
	}
	template<typename T>
	constexpr bool Plane<T>::IsInside(const Vector3<T>& aPosition, const Vector3<T>& aExtends) const
	{
		const T r = 
			aExtends.x * std::abs(GetNormal().x) +
			aExtends.y * std::abs(GetNormal().y) +
			aExtends.z * std::abs(GetNormal().z);

		return Vector3<T>::Direction(GetOrigin(), aPosition).Dot(GetNormal()) <= -r;
	}

	template<typename T>
	constexpr Shape::Type Plane<T>::GetType() const noexcept
	{
		return Shape::Type::Plane;
	}

	// using declarations

	using Planef = Plane<float>;
	using Planed = Plane<double>;
	using Planei = Plane<int>;
}
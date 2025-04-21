#pragma once

#include <array>

#include "Vector3.hpp"
#include "Shape.h"

#include <CommonUtilities/Utility/Clonable.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T>
	class AABB;

	template<typename T>
	class Triangle final : public Clonable<Shape, Triangle<T>>
	{
	public:
		constexpr Triangle() = default;
		constexpr ~Triangle() = default;

		constexpr Triangle(const Vector3<T>& aPointA, const Vector3<T>& aPointB, const Vector3<T>& aPointC);
		constexpr Triangle(const std::array<Vector3<T>, 3>& aPoints);

		NODISC constexpr const Vector3<T>& operator[](std::size_t aIndex) const;

		NODISC constexpr const Vector3<T>& GetPointA() const noexcept;
		NODISC constexpr const Vector3<T>& GetPointB() const noexcept;
		NODISC constexpr const Vector3<T>& GetPointC() const noexcept;
		NODISC constexpr const Vector3<T>& GetNormal() const noexcept;

		NODISC constexpr const std::array<Vector3<T>, 3>& GetPoints() const;
		NODISC constexpr const Vector3<T>& GetPoint(std::size_t aIndex) const;

		NODISC constexpr Vector3<T> GetCenter() const;
		NODISC constexpr AABB<T> GetAABB() const;
		NODISC constexpr T GetArea() const;

		constexpr void SetPointA(const Vector3<T>& aPoint);
		constexpr void SetPointB(const Vector3<T>& aPoint);
		constexpr void SetPointC(const Vector3<T>& aPoint);

		constexpr void SetPoint(const Vector3<T>& aPoint, std::size_t aIndex);

		NODISC constexpr Shape::Type GetType() const noexcept override;

	private:
		std::array<Vector3<T>, 3> myPoints;
		Vector3<T> myNormal;
	};

	template<typename T>
	constexpr Triangle<T>::Triangle(const Vector3<T>& aPointA, const Vector3<T>& aPointB, const Vector3<T>& aPointC)
		: myPoints(aPointA, aPointB, aPointC)
		, myNormal((GetPointB() - GetPointA()).Cross(GetPointC() - GetPointA()).GetNormalized())
	{

	}

	template<typename T>
	constexpr Triangle<T>::Triangle(const std::array<Vector3<T>, 3>& aPoints)
		: Triangle(aPoints[0], aPoints[1], aPoints[2])
	{

	}

	template<typename T>
	constexpr const Vector3<T>& Triangle<T>::operator[](std::size_t aIndex) const
	{
		return myPoints[aIndex];
	}

	template<typename T>
	constexpr const Vector3<T>& Triangle<T>::GetPointA() const noexcept
	{
		return myPoints[0];
	}
	template<typename T>
	constexpr const Vector3<T>& Triangle<T>::GetPointB() const noexcept
	{
		return myPoints[1];
	}
	template<typename T>
	constexpr const Vector3<T>& Triangle<T>::GetPointC() const noexcept
	{
		return myPoints[2];
	}
	template<typename T>
	constexpr const Vector3<T>& Triangle<T>::GetNormal() const noexcept
	{
		return myNormal;
	}

	template<typename T>
	constexpr const std::array<Vector3<T>, 3>& Triangle<T>::GetPoints() const
	{
		return myPoints;
	}
	template<typename T>
	constexpr const Vector3<T>& Triangle<T>::GetPoint(std::size_t aIndex) const
	{
		return myPoints[aIndex];
	}

	template<typename T>
	constexpr Vector3<T> Triangle<T>::GetCenter() const
	{
		return (GetPointA() + GetPointB() + GetPointC()) / T(3);
	}
	template<typename T>
	constexpr AABB<T> Triangle<T>::GetAABB() const
	{
		Vector3<T> min;
		Vector3<T> max;

		min.x = Min(GetPointA().x, GetPointB().x, GetPointC().x), min.y = Min(GetPointA().y, GetPointB().y, GetPointC().y), min.z = Min(GetPointA().z, GetPointB().z, GetPointC().z);
		max.x = Max(GetPointA().x, GetPointB().x, GetPointC().x), max.y = Max(GetPointA().y, GetPointB().y, GetPointC().y), max.z = Max(GetPointA().z, GetPointB().z, GetPointC().z);

		return AABB<T>(min, max);
	}

	template<typename T>
	constexpr T Triangle<T>::GetArea() const
	{
		const Vector3<T> AB = (GetPointB() - GetPointA());
		const Vector3<T> AC = (GetPointC() - GetPointA());

		const Vector3<T> ABxAC = AB.Cross(AC);

		return ABxAC.Length() / T(2);
	}

	template<typename T>
	constexpr void Triangle<T>::SetPointA(const Vector3<T>& aPoint)
	{
		myPoints[0] = aPoint;
		myNormal = (GetPointB() - GetPointA()).Cross(GetPointC() - GetPointA()).GetNormalized();
	}
	template<typename T>
	constexpr void Triangle<T>::SetPointB(const Vector3<T>& aPoint)
	{
		myPoints[1] = aPoint;
		myNormal = (GetPointB() - GetPointA()).Cross(GetPointC() - GetPointA()).GetNormalized();
	}
	template<typename T>
	constexpr void Triangle<T>::SetPointC(const Vector3<T>& aPoint)
	{
		myPoints[2] = aPoint;
		myNormal = (GetPointB() - GetPointA()).Cross(GetPointC() - GetPointA()).GetNormalized();
	}

	template<typename T>
	constexpr void Triangle<T>::SetPoint(const Vector3<T>& aPoint, std::size_t aIndex)
	{
		myPoints[aIndex] = aPoint;
	}

	template<typename T>
	constexpr Shape::Type Triangle<T>::GetType() const noexcept
	{
		return Shape::Type::Triangle;
	}

	// GLOBAL OPERATORS

	template<typename T>
	NODISC constexpr bool operator==(const Triangle<T>& aLeft, const Triangle<T>& aRight)
	{
		return  aLeft.GetPointA() == aRight.GetPointA() &&
				aLeft.GetPointB() == aRight.GetPointB() &&
				aLeft.GetPointC() == aRight.GetPointC();
	}
	template<typename T>
	NODISC constexpr bool operator!=(const Triangle<T>& aLeft, const Triangle<T>& aRight)
	{
		return !(aLeft == aRight);
	}

	using Trif = Triangle<float>;
	using Trid = Triangle<double>;
	using Trii = Triangle<int>;
}
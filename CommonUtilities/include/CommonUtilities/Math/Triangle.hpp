#pragma once

#include "Vector3.hpp"
#include "Shape.h"

#include <CommonUtilities/Utility/Clonable.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T>
	class Triangle final : public Clonable<Shape, Triangle<T>>
	{
	public:
		constexpr Triangle() = default;
		constexpr ~Triangle() = default;

		constexpr Triangle(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);

		NODISC constexpr const Vector3<T>& GetPoint0() const noexcept;
		NODISC constexpr const Vector3<T>& GetPoint1() const noexcept;
		NODISC constexpr const Vector3<T>& GetPoint2() const noexcept;
		NODISC constexpr const Vector3<T>& GetNormal() const noexcept;

		NODISC constexpr Vector3<T> GetCenter() const noexcept;

		constexpr void SetPoint0(const Vector3<T>& aPoint);
		constexpr void SetPoint1(const Vector3<T>& aPoint);
		constexpr void SetPoint2(const Vector3<T>& aPoint);

		NODISC constexpr Shape::Type GetType() const noexcept override;

	private:
		Vector3<T> myPoint0;
		Vector3<T> myPoint1;
		Vector3<T> myPoint2;
		Vector3<T> myNormal;
	};

	template<typename T>
	constexpr Triangle<T>::Triangle(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
		: myPoint0(aPoint0)
		, myPoint1(aPoint1)
		, myPoint2(aPoint2)
		, myNormal((myPoint1 - myPoint0).Cross(myPoint2 - myPoint0).GetNormalized())
	{

	}

	template<typename T>
	constexpr const Vector3<T>& Triangle<T>::GetPoint0() const noexcept
	{
		return myPoint0;
	}
	template<typename T>
	constexpr const Vector3<T>& Triangle<T>::GetPoint1() const noexcept
	{
		return myPoint1;
	}
	template<typename T>
	constexpr const Vector3<T>& Triangle<T>::GetPoint2() const noexcept
	{
		return myPoint2;
	}
	template<typename T>
	constexpr const Vector3<T>& Triangle<T>::GetNormal() const noexcept
	{
		return myNormal;
	}

	template<typename T>
	constexpr Vector3<T> Triangle<T>::GetCenter() const noexcept
	{
		return (myPoint0 + myPoint1 + myPoint2) / T(3);
	}

	template<typename T>
	constexpr void Triangle<T>::SetPoint0(const Vector3<T>& aPoint)
	{
		myPoint0 = aPoint;
		myNormal = (myPoint1 - myPoint0).Cross(myPoint2 - myPoint0).GetNormalized();
	}
	template<typename T>
	constexpr void Triangle<T>::SetPoint1(const Vector3<T>& aPoint)
	{
		myPoint1 = aPoint;
		myNormal = (myPoint1 - myPoint0).Cross(myPoint2 - myPoint0).GetNormalized();
	}
	template<typename T>
	constexpr void Triangle<T>::SetPoint2(const Vector3<T>& aPoint)
	{
		myPoint2 = aPoint;
		myNormal = (myPoint1 - myPoint0).Cross(myPoint2 - myPoint0).GetNormalized();
	}

	template<typename T>
	constexpr Shape::Type Triangle<T>::GetType() const noexcept
	{
		return Shape::Type::Triangle;
	}

	using Trif = Triangle<float>;
	using Trid = Triangle<double>;
	using Trii = Triangle<int>;
}
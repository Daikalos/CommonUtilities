#pragma once

#include <cassert>

#include "Vector3.hpp"
#include "Shape.h"

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T>
	class AABB3D final : public Shape
	{
	public:
		CONSTEXPR AABB3D();
		CONSTEXPR ~AABB3D();

		CONSTEXPR AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax);

		CONSTEXPR void InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax);
		CONSTEXPR void InitWithCenterAndSize(const Vector3<T>& aCenter, const Vector3<T>& aSize);

		CONSTEXPR NODISC const Vector3<T>& GetMin() const noexcept;
		CONSTEXPR NODISC const Vector3<T>& GetMax() const noexcept;

		CONSTEXPR NODISC Vector3<T> GetSize() const;
		CONSTEXPR NODISC Vector3<T> GetCenter() const;

		CONSTEXPR void SetMin(const Vector3<T>& aMin);
		CONSTEXPR void SetMax(const Vector3<T>& aMax);
		CONSTEXPR void SetSize(const Vector3<T>& aSize);
		CONSTEXPR void SetCenter(const Vector3<T>& aCenter);

		CONSTEXPR NODISC bool IsInside(const Vector3<T>& aPosition) const;

		auto GetType() const noexcept -> Type override;

	private:
		Vector3<T> myMin;
		Vector3<T> myMax;
	};

	template<typename T>
	CONSTEXPR AABB3D<T>::AABB3D() = default;

	template<typename T>
	CONSTEXPR AABB3D<T>::~AABB3D() = default;

	template<typename T>
	CONSTEXPR AABB3D<T>::AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		InitWithMinAndMax(aMin, aMax);
	}

	template<typename T>
	CONSTEXPR void AABB3D<T>::InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		assert(myMax.x >= myMin.x && myMax.y >= myMin.y && "Maximum cannot be smaller than minimum");

		myMin = aMin;
		myMax = aMax;
	}

	template<typename T>
	CONSTEXPR void AABB3D<T>::InitWithCenterAndSize(const Vector3<T>& aCenter, const Vector3<T>& aSize)
	{
		const Vector3<T> extends = aSize / 2.0f;

		myMax = aCenter + extends;
		myMin = aCenter - extends;
	}

	template<typename T>
	CONSTEXPR const Vector3<T>& AABB3D<T>::GetMin() const noexcept
	{
		return myMin;
	}
	template<typename T>
	CONSTEXPR const Vector3<T>& AABB3D<T>::GetMax() const noexcept
	{
		return myMax;
	}

	template<typename T>
	CONSTEXPR Vector3<T> AABB3D<T>::GetSize() const
	{
		return (myMax - myMin);
	}
	template<typename T>
	CONSTEXPR Vector3<T> AABB3D<T>::GetCenter() const
	{
		return GetSize() / 2.0f + myMin;
	}

	template<typename T>
	CONSTEXPR void AABB3D<T>::SetMin(const Vector3<T>& aMin)
	{
		assert(myMax.x >= aMin.x && myMax.y >= aMin.y && "Maximum cannot be smaller than minimum");
		myMin = aMin;
	}
	template<typename T>
	CONSTEXPR void AABB3D<T>::SetMax(const Vector3<T>& aMax)
	{
		assert(aMax.x >= myMin.x && aMax.y >= myMin.y && "Maximum cannot be smaller than minimum");
		myMax = aMax;
	}
	template<typename T>
	CONSTEXPR void AABB3D<T>::SetSize(const Vector3<T>& aSize)
	{
		const Vector3<T> center = GetCenter();
		const Vector3<T> extends = aSize / 2.0f;

		myMax = center + extends;
		myMin = center - extends;
	}
	template<typename T>
	CONSTEXPR void AABB3D<T>::SetCenter(const Vector3<T>& aCenter)
	{
		const Vector3<T> halfExtends = (myMax - myMin) / 2.0f;

		myMax = aCenter + halfExtends;
		myMin = aCenter - halfExtends;
	}

	template<typename T>
	CONSTEXPR bool AABB3D<T>::IsInside(const Vector3<T>& aPosition) const
	{
		return 
			(aPosition.x >= myMin.x) && (aPosition.x <= myMax.x) && 
			(aPosition.y >= myMin.y) && (aPosition.y <= myMax.y) && 
			(aPosition.z >= myMin.z) && (aPosition.z <= myMax.z);
	}

	template<typename T>
	inline auto AABB3D<T>::GetType() const noexcept -> Type
	{
		return Type::AABB3D;
	}
}
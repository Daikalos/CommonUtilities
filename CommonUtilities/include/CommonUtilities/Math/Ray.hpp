#pragma once

#include "Vector3.hpp"
#include "Shape.h"

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T>
	class Ray final : public Shape
	{
	public:
		CONSTEXPR Ray(); 
		CONSTEXPR ~Ray();

		CONSTEXPR Ray(const Vector3<T>& aOrigin, const Vector3<T>& aDirection);

		CONSTEXPR void InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint);
		CONSTEXPR void InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>& aDirection);

		NODISC CONSTEXPR const Vector3<T>& GetOrigin() const noexcept;
		NODISC CONSTEXPR const Vector3<T>& GetDirection() const noexcept;

		CONSTEXPR void SetOrigin(const Vector3<T>& aOrigin);
		CONSTEXPR void SetDirection(const Vector3<T>& aDirection);

		NODISC CONSTEXPR auto GetType() const noexcept -> Type override;

	private:
		Vector3<T> myOrigin;
		Vector3<T> myDirection;
	};

	template<typename T>
	CONSTEXPR Ray<T>::Ray() = default;

	template<typename T>
	CONSTEXPR Ray<T>::~Ray() = default;

	template<typename T>
	CONSTEXPR Ray<T>::Ray(const Vector3<T>& aOrigin, const Vector3<T>& aDirection)
	{
		InitWithOriginAndDirection(aOrigin, aDirection);
	}

	template<typename T>
	CONSTEXPR void Ray<T>::InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint)
	{
		myOrigin = aOrigin;
		myDirection = Vector3<T>::Direction(aOrigin, aPoint).GetNormalized();
	}
	template<typename T>
	CONSTEXPR void Ray<T>::InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>& aDirection)
	{
		myOrigin = aOrigin;
		myDirection = aDirection.GetNormalized(); // make sure it is normalized
	}

	template<typename T>
	CONSTEXPR const Vector3<T>& Ray<T>::GetOrigin() const noexcept
	{
		return myOrigin;
	}
	template<typename T>
	CONSTEXPR const Vector3<T>& Ray<T>::GetDirection() const noexcept
	{
		return myDirection;
	}

	template<typename T>
	CONSTEXPR void Ray<T>::SetOrigin(const Vector3<T>& aOrigin)
	{
		myOrigin = aOrigin;
	}
	template<typename T>
	CONSTEXPR void Ray<T>::SetDirection(const Vector3<T>& aDirection)
	{
		myDirection = aDirection;
	}

	template<typename T>
	CONSTEXPR auto Ray<T>::GetType() const noexcept -> Type
	{
		return Type::Ray;
	}
}
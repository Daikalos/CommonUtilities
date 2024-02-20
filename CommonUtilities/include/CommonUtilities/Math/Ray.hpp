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
		constexpr Ray() = default;
		constexpr ~Ray() = default;

		constexpr Ray(const Vector3<T>& aOrigin, const Vector3<T>& aDirection);

		NODISC constexpr static Ray<T> InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint);
		NODISC constexpr static Ray<T> InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>& aDirection);

		NODISC constexpr const Vector3<T>& GetOrigin() const noexcept;
		NODISC constexpr const Vector3<T>& GetDirection() const noexcept;

		constexpr void SetOrigin(const Vector3<T>& aOrigin);
		constexpr void SetDirection(const Vector3<T>& aDirection);

		NODISC constexpr auto GetType() const noexcept -> Type override;
		NODISC constexpr std::unique_ptr<Shape> Clone() const override;

	private:
		Vector3<T> myOrigin;
		Vector3<T> myDirection;
	};

	template<typename T>
	constexpr Ray<T>::Ray(const Vector3<T>& aOrigin, const Vector3<T>& aDirection)
		: myOrigin(aOrigin)
		, myDirection(aDirection.GetNormalized()) // make sure it is normalized
	{

	}

	template<typename T>
	constexpr Ray<T> Ray<T>::InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint)
	{
		return Ray<T>(aOrigin, Vector3<T>::Direction(aOrigin, aPoint));
	}
	template<typename T>
	constexpr Ray<T> Ray<T>::InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>& aDirection)
	{
		return Ray<T>(aOrigin, aDirection);
	}

	template<typename T>
	constexpr const Vector3<T>& Ray<T>::GetOrigin() const noexcept
	{
		return myOrigin;
	}
	template<typename T>
	constexpr const Vector3<T>& Ray<T>::GetDirection() const noexcept
	{
		return myDirection;
	}

	template<typename T>
	constexpr void Ray<T>::SetOrigin(const Vector3<T>& aOrigin)
	{
		myOrigin = aOrigin;
	}
	template<typename T>
	constexpr void Ray<T>::SetDirection(const Vector3<T>& aDirection)
	{
		myDirection = aDirection.GetNormalized();
	}

	template<typename T>
	constexpr auto Ray<T>::GetType() const noexcept -> Type
	{
		return Type::Ray;
	}
	template<typename T>
	constexpr std::unique_ptr<Shape> Ray<T>::Clone() const
	{
		return std::make_unique<Ray<T>>(*this);
	}

	using RayFloat	= Ray<float>;
	using RayDouble	= Ray<double>;
	using RayInt	= Ray<int>;
}
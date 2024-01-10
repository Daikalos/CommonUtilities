#pragma once

#include "Vector2.hpp"
#include "Shape.h"

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T>
	class Line final : public Shape
	{
	public:
		CONSTEXPR Line();
		CONSTEXPR ~Line();

		CONSTEXPR Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);

		CONSTEXPR void InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);
		CONSTEXPR void InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection);

		CONSTEXPR const Vector2<T>& GetDirection() const;
		CONSTEXPR Vector2<T> GetNormal() const;

		CONSTEXPR void SetOrigin(const Vector2<T>& aOrigin);
		CONSTEXPR void SetDirection(const Vector2<T>& aDirection);

		CONSTEXPR bool IsInside(const Vector2<T>& aPosition) const;

		auto GetType() const noexcept -> Type override;

	private:
		Vector2<T> myOrigin;
		Vector2<T> myDirection;
	};

	template<typename T>
	CONSTEXPR Line<T>::Line() = default;

	template<typename T>
	CONSTEXPR Line<T>::~Line() = default;

	template<typename T>
	CONSTEXPR Line<T>::Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		InitWith2Points(aPoint0, aPoint1);
	}

	template<typename T>
	CONSTEXPR void Line<T>::InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		myOrigin = aPoint0;
		myDirection = Vector2<T>::Direction(aPoint0, aPoint1).GetNormalized();
	}
	template<typename T>
	CONSTEXPR void Line<T>::InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection)
	{
		myOrigin = aPoint;
		myDirection = aDirection.GetNormalized(); // make sure it is normalized
	}

	template<typename T>
	CONSTEXPR const Vector2<T>& Line<T>::GetDirection() const
	{
		return myDirection;
	}
	template<typename T>
	CONSTEXPR Vector2<T> Line<T>::GetNormal() const
	{
		return Vector2<T>(-myDirection.y, myDirection.x);
	}

	template<typename T>
	CONSTEXPR void Line<T>::SetOrigin(const Vector2<T>& aOrigin)
	{
		myOrigin = aOrigin;
	}
	template<typename T>
	CONSTEXPR void Line<T>::SetDirection(const Vector2<T>& aDirection)
	{
		myDirection = aDirection.GetNormalized();
	}

	template<typename T>
	CONSTEXPR bool Line<T>::IsInside(const Vector2<T>& aPosition) const
	{
		return Vector2<T>::Direction(myOrigin, aPosition).Dot(GetNormal()) <= T{};
	}

	template<typename T>
	inline auto Line<T>::GetType() const noexcept -> Type
	{
		return Type::Line;
	}

	// using declarations

	using LineFloat		= Line<float>;
	using LineDouble	= Line<double>;
	using LineInt		= Line<int>;
}
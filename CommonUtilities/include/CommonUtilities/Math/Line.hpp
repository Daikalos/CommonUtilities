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
		constexpr Line() = default;
		constexpr ~Line() = default;

		constexpr Line(const Vector2<T>& aOrigin, const Vector2<T>& aDirection);

		NODISC constexpr static Line InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);
		NODISC constexpr static Line InitWithPointAndDirection(const Vector2<T>& aOrigin, const Vector2<T>& aDirection);

		NODISC constexpr const Vector2<T>& GetOrigin() const noexcept;
		NODISC constexpr const Vector2<T>& GetDirection() const noexcept;
		NODISC constexpr Vector2<T> GetNormal() const;

		constexpr void SetOrigin(const Vector2<T>& aOrigin);
		constexpr void SetDirection(const Vector2<T>& aDirection);

		NODISC constexpr bool IsInside(const Vector2<T>& aPosition) const;

		NODISC constexpr auto GetType() const noexcept -> Type override;

	private:
		Vector2<T> myOrigin;
		Vector2<T> myDirection;
	};

	template<typename T>
	constexpr Line<T>::Line(const Vector2<T>& aOrigin, const Vector2<T>& aDirection)
		: myOrigin(aPoint0)
		, myDirection(aDirection.GetNormalized()) // make sure it is normalized
	{

	}

	template<typename T>
	constexpr Line<T> Line<T>::InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		return Line<T>(aPoint0, Vector2<T>::Direction(aPoint0, aPoint1));
	}
	template<typename T>
	constexpr Line<T> Line<T>::InitWithPointAndDirection(const Vector2<T>& aOrigin, const Vector2<T>& aDirection)
	{
		return Line<T>(aOrigin, aDirection);
	}

	template<typename T>
	constexpr const Vector2<T>& Line<T>::GetOrigin() const noexcept
	{
		return myOrigin;
	}
	template<typename T>
	constexpr const Vector2<T>& Line<T>::GetDirection() const noexcept
	{
		return myDirection;
	}
	template<typename T>
	constexpr Vector2<T> Line<T>::GetNormal() const
	{
		return Vector2<T>(-myDirection.y, myDirection.x);
	}

	template<typename T>
	constexpr void Line<T>::SetOrigin(const Vector2<T>& aOrigin)
	{
		myOrigin = aOrigin;
	}
	template<typename T>
	constexpr void Line<T>::SetDirection(const Vector2<T>& aDirection)
	{
		myDirection = aDirection.GetNormalized();
	}

	template<typename T>
	constexpr bool Line<T>::IsInside(const Vector2<T>& aPosition) const
	{
		return Vector2<T>::Direction(myOrigin, aPosition).Dot(GetNormal()) <= T{};
	}

	template<typename T>
	constexpr auto Line<T>::GetType() const noexcept -> Type
	{
		return Type::Line;
	}

	// using declarations

	using LineFloat		= Line<float>;
	using LineDouble	= Line<double>;
	using LineInt		= Line<int>;
}
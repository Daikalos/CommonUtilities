#pragma once

#include "Vector2.hpp"

namespace CommonUtilities
{
	template<typename T>
	class Line
	{
	public:
		Line();
		~Line();

		Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);

		void InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);
		void InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection);

		const Vector2<T>& GetDirection() const;
		Vector2<T> GetNormal() const;

		bool IsInside(const Vector2<T>& aPosition) const;

	private:
		Vector2<T> myOrigin;
		Vector2<T> myDirection;
	};

	template<typename T>
	inline Line<T>::Line() = default;

	template<typename T>
	inline Line<T>::~Line() = default;

	template<typename T>
	inline Line<T>::Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		InitWith2Points(aPoint0, aPoint1);
	}

	template<typename T>
	inline void Line<T>::InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		myOrigin = aPoint0;
		myDirection = Vector2<T>::Direction(aPoint0, aPoint1).GetNormalized();
	}
	template<typename T>
	inline void Line<T>::InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection)
	{
		myOrigin = aPoint;
		myDirection = aDirection.GetNormalized(); // make sure it is normalized
	}

	template<typename T>
	inline const Vector2<T>& Line<T>::GetDirection() const
	{
		return myDirection;
	}
	template<typename T>
	inline Vector2<T> Line<T>::GetNormal() const
	{
		return Vector2<T>(-myDirection.y, myDirection.x);
	}

	template<typename T>
	inline bool Line<T>::IsInside(const Vector2<T>& aPosition) const
	{
		return Vector2<T>::Direction(myOrigin, aPosition).Dot(GetNormal()) <= T{};
	}

	// using declarations

	using LineFloat		= Line<float>;
	using LineDouble	= Line<double>;
	using LineInt		= Line<int>;
}
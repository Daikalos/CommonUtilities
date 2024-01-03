#pragma once

#include "Vector3.hpp"

namespace CommonUtilities
{
	template<typename T>
	class Ray
	{
	public:
		Ray(); 
		~Ray();

		Ray(const Vector3<T>& aOrigin, const Vector3<T>& aDirection);

		void InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint);
		void InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>& aDirection);

		const Vector3<T>& GetOrigin() const noexcept;
		const Vector3<T>& GetDirection() const noexcept;

	private:
		Vector3<T> myOrigin;
		Vector3<T> myDirection;
	};

	template<typename T>
	inline Ray<T>::Ray() = default;

	template<typename T>
	inline Ray<T>::~Ray() = default;

	template<typename T>
	inline Ray<T>::Ray(const Vector3<T>& aOrigin, const Vector3<T>& aDirection)
	{
		InitWithOriginAndDirection(aOrigin, aDirection);
	}

	template<typename T>
	inline void Ray<T>::InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint)
	{
		myOrigin = aOrigin;
		myDirection = Vector3<T>::Direction(aOrigin, aPoint).GetNormalized();
	}

	template<typename T>
	inline void Ray<T>::InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>& aDirection)
	{
		myOrigin = aOrigin;
		myDirection = aDirection.GetNormalized(); // make sure it is normalized
	}

	template<typename T>
	inline const Vector3<T>& Ray<T>::GetOrigin() const noexcept
	{
		return myOrigin;
	}

	template<typename T>
	inline const Vector3<T>& Ray<T>::GetDirection() const noexcept
	{
		return myDirection;
	}
}
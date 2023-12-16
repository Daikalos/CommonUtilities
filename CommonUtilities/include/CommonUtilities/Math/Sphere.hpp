#pragma once

#include "Vector3.hpp"

namespace CommonUtilities
{
	template<typename T>
	class Sphere
	{
	public:
		Sphere();
		~Sphere();

		Sphere(const Vector3<T>& aCenter, T aRadius);

		void InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius);

		bool IsInside(const Vector3<T>& aPosition) const;

		const Vector3<T>& GetCenter() const noexcept;
		T GetRadius() const noexcept;
		T GetRadiusSqr() const noexcept;

	private:
		Vector3<T> myCenter;
		T myRadius{};
		T myRadiusSqr{};
	};

	template<typename T>
	inline Sphere<T>::Sphere() = default;

	template<typename T>
	inline Sphere<T>::~Sphere() = default;

	template<typename T>
	inline Sphere<T>::Sphere(const Vector3<T>& aCenter, T aRadius)
	{
		InitWithCenterAndRadius(aCenter, aRadius);
	}

	template<typename T>
	inline void Sphere<T>::InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius)
	{
		myCenter = aCenter;
		myRadius = aRadius;
		myRadiusSqr = myRadius * myRadius;
	}

	template<typename T>
	inline bool Sphere<T>::IsInside(const Vector3<T>& aPosition) const
	{
		T distanceSqr = Vector3<T>::Direction(myCenter, aPosition).LengthSqr();
		return distanceSqr <= myRadiusSqr;
	}

	template<typename T>
	inline const Vector3<T>& Sphere<T>::GetCenter() const noexcept
	{
		return myCenter;
	}
	template<typename T>
	inline T Sphere<T>::GetRadius() const noexcept
	{
		return myRadius;
	}
	template<typename T>
	inline T Sphere<T>::GetRadiusSqr() const noexcept
	{
		return myRadiusSqr;
	}
}
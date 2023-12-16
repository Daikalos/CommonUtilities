#pragma once

#include "Vector3.hpp"

namespace CommonUtilities
{
	template<typename T>
	class Plane
	{
	public:
		Plane();
		~Plane();

		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);
		Plane(const Vector3<T>& aPoint, const Vector3<T>& aNormal);

		void InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);
		void InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal);

		const Vector3<T>& GetOrigin() const noexcept;
		const Vector3<T>& GetNormal() const noexcept;

		bool IsInside(const Vector3<T>& aPosition) const;

	private:
		Vector3<T> myOrigin;
		Vector3<T> myNormal;
	};

	template<typename T>
	inline Plane<T>::Plane() = default;
	template<typename T>
	inline Plane<T>::~Plane() = default;

	template<typename T>
	inline Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		InitWith3Points(aPoint0, aPoint1, aPoint2);
	}
	template<typename T>
	inline Plane<T>::Plane(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
	{
		InitWithPointAndNormal(aPoint, aNormal);
	}

	template<typename T>
	inline void Plane<T>::InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		myOrigin = aPoint0;

		Vector3<T> originToPoint1 = Vector3<T>::Direction(myOrigin, aPoint1);
		Vector3<T> originToPoint2 = Vector3<T>::Direction(myOrigin, aPoint2);

		myNormal = originToPoint1.Cross(originToPoint2).GetNormalized();
	}
	template<typename T>
	inline void Plane<T>::InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
	{
		myOrigin = aPoint;
		myNormal = aNormal.GetNormalized(); // make sure is normalized
	}

	template<typename T>
	inline const Vector3<T>& Plane<T>::GetOrigin() const noexcept
	{
		return myOrigin;
	}
	template<typename T>
	inline const Vector3<T>& Plane<T>::GetNormal() const noexcept
	{
		return myNormal;
	}

	template<typename T>
	inline bool Plane<T>::IsInside(const Vector3<T>& aPosition) const
	{
		return Vector3<T>::Direction(myOrigin, aPosition).Dot(GetNormal()) <= T{};
	}

	// using declarations

	using PlaneFloat	= Plane<float>;
	using PlaneDouble	= Plane<double>;
	using PlaneInt		= Plane<int>;
}
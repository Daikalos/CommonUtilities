#pragma once

#include <cassert>

#include "Vector3.hpp"

namespace CommonUtilities
{
	template<typename T>
	class AABB3D
	{
	public:
		AABB3D();
		~AABB3D();

		AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax);

		void InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax);

		const Vector3<T>& GetMin() const noexcept;
		const Vector3<T>& GetMax() const noexcept;

		bool IsInside(const Vector3<T>& aPosition) const;

	private:
		Vector3<T> myMin;
		Vector3<T> myMax;
	};

	template<typename T>
	inline AABB3D<T>::AABB3D() = default;

	template<typename T>
	inline AABB3D<T>::~AABB3D() = default;

	template<typename T>
	inline AABB3D<T>::AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		InitWithMinAndMax(aMin, aMax);
	}

	template<typename T>
	inline void AABB3D<T>::InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		assert(myMax.x >= myMin.x && myMax.y >= myMin.y && "Maximum cannot be smaller than minimum");

		myMin = aMin;
		myMax = aMax;
	}

	template<typename T>
	inline const Vector3<T>& AABB3D<T>::GetMin() const noexcept
	{
		return myMin;
	}
	template<typename T>
	inline const Vector3<T>& AABB3D<T>::GetMax() const noexcept
	{
		return myMax;
	}

	template<typename T>
	inline bool AABB3D<T>::IsInside(const Vector3<T>& aPosition) const
	{
		return 
			(aPosition.x >= myMin.x) && (aPosition.x <= myMax.x) && 
			(aPosition.y >= myMin.y) && (aPosition.y <= myMax.y) && 
			(aPosition.z >= myMin.z) && (aPosition.z <= myMax.z);
	}
}
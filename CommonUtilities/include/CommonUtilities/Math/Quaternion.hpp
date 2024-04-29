#pragma once

#include <cmath>

#include "Vector3.hpp"

namespace CommonUtilities
{
	template <class T>
	class Matrix4x4;

	template<typename T>
	class Quaternion
	{
	public:
		T w{}; // scalar
		T x{};
		T y{};
		T z{};

		Quaternion() = default;

		Quaternion(const T& aW, const T& aX, const T& aY, const T& aZ);
		Quaternion(const T& aYaw, const T& aPitch, const T& aRoll);
		Quaternion(const Vector3<T>& aYawPitchRoll);
		Quaternion(const Vector3<T>& aVector, const T& aAngle);
		Quaternion(const Matrix4x4<T>& aMatrix);
	};

	template<typename T>
	inline Quaternion<T>::Quaternion(const T& aW, const T& aX, const T& aY, const T& aZ)
	{
		w = aW;
		x = aX;
		y = aY;
		z = aZ;
	}

	template<typename T>
	inline Quaternion<T>::Quaternion(const T& aYaw, const T& aPitch, const T& aRoll)
	{
		T cy = std::cos(aYaw   * T(0.5));
		T sy = std::sin(aYaw   * T(0.5));
		T cr = std::cos(aRoll  * T(0.5));
		T sr = std::sin(aRoll  * T(0.5));
		T cp = std::cos(aPitch * T(0.5));
		T sp = std::sin(aPitch * T(0.5));

		w = cy * cr * cp + sy * sr * sp;
		x = cy * sr * cp - sy * cr * sp;
		y = cy * cr * sp + sy * sr * cp;
		z = sy * cr * cp - cy * sr * sp;
	}

	template<typename T>
	inline Quaternion<T>::Quaternion(const Vector3<T>& aYawPitchRoll)
	{
		T cx = std::cos(aYawPitchRoll.x * T(0.5));
		T cy = std::cos(aYawPitchRoll.y * T(0.5));
		T cz = std::cos(aYawPitchRoll.z * T(0.5));
		T sx = std::sin(aYawPitchRoll.x * T(0.5));
		T sy = std::sin(aYawPitchRoll.y * T(0.5));
		T sz = std::sin(aYawPitchRoll.z * T(0.5));

		w = cx * cy * cz + sx * sy * sz;
		x = sx * cy * cz - cx * sy * sz;
		y = cx * sy * cz + sx * cy * sz;
		z = cx * cy * sz - sx * sy * cz;
	}

	template<typename T>
	inline Quaternion<T>::Quaternion(const Vector3<T>& aVector, const T& aAngle)
	{
		T halfAngle = aAngle / T(2);

		T c = std::cos(halfAngle);
		T s = std::sin(halfAngle);

		w = c;
		x = aVector.x * s;
		y = aVector.y * s;
		z = aVector.z * s;
	}

	template<typename T>
	inline Quaternion<T>::Quaternion(const Matrix4x4<T>& aMatrix)
	{
		w = std::sqrt(std::max(T(0), T(1) + aMatrix.m11 + aMatrix.m22 + aMatrix.m33)) * T(0.5);
		x = std::sqrt(std::max(T(0), T(1) + aMatrix.m11 - aMatrix.m22 - aMatrix.m33)) * T(0.5);
		y = std::sqrt(std::max(T(0), T(1) - aMatrix.m11 + aMatrix.m22 - aMatrix.m33)) * T(0.5);
		z = std::sqrt(std::max(T(0), T(1) - aMatrix.m11 - aMatrix.m22 + aMatrix.m33)) * T(0.5);
		x = std::copysign(x, aMatrix.m32 - aMatrix.m23);
		y = std::copysign(y, aMatrix.m13 - aMatrix.m31);
		z = std::copysign(z, aMatrix.m21 - aMatrix.m12);
	}

	using Quatf = Quaternion<float>;
	using Quatd = Quaternion<double>;
}
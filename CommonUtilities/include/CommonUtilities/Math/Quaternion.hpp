#pragma once

#include <cmath>
#include <cassert>

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

		constexpr Quaternion() = default;

		constexpr Quaternion(T aW, T aX, T aY, T aZ);
		constexpr Quaternion(T aYaw, T aPitch, T aRoll);
		constexpr Quaternion(const Vector3<T>& aYawPitchRoll);
		constexpr Quaternion(const Vector3<T>& aVector, T aAngle);
		constexpr Quaternion(const Matrix4x4<T>& aMatrix);

		constexpr void Normalize(T aNormLength = static_cast<T>(1));

		constexpr void RotateWithEuler(const Vector3<T>& aEuler);
		constexpr void RotateWithRadians(const Vector3<T>& aRadians);

		NODISC constexpr T Dot(const Quaternion<T>& aQuat) const;

		/// Norm of the quaternion
		/// 
		NODISC constexpr T Length() const;
		NODISC constexpr T LengthSqr() const;

		NODISC constexpr Quaternion<T> GetNormalized(T aNormLength = static_cast<T>(1)) const;
		NODISC constexpr Quaternion<T> GetConjugate() const;

		NODISC constexpr Vector3<T> GetEulerAnglesRadians() const;
		NODISC constexpr Vector3<T> GetEulerAnglesRadiansd() const;
		NODISC constexpr Vector3<T> GetEulerAnglesDegrees() const;

		NODISC constexpr Vector3<T> GetRight() const;
		NODISC constexpr Vector3<T> GetUp() const;
		NODISC constexpr Vector3<T> GetForward() const;

		NODISC constexpr Quaternion<T> GetInverse() const;

		NODISC constexpr static Quaternion<T> Lerp(const Quaternion<T>& aQuatA, const Quaternion<T>& aQuatB, T aDelta);
		NODISC constexpr static Quaternion<T> Slerp(const Quaternion<T>& aQuatA, const Quaternion<T>& aQuatB, T aDelta);

		NODISC constexpr static Vector3<T> RotateVectorByQuaternion(const Quaternion<T>& aQuaternion, const Vector3f& aVectorToRotate);
	};

	template<typename T>
	constexpr Quaternion<T>::Quaternion(T aW, T aX, T aY, T aZ)
		: w(aW), x(aX), y(aY), z(aZ)
	{
	}

	template<typename T>
	constexpr Quaternion<T>::Quaternion(T aYaw, T aPitch, T aRoll)
	{
		T cr = std::cos(aRoll  * T(0.5));
		T sr = std::sin(aRoll  * T(0.5));
		T cy = std::cos(aYaw   * T(0.5));
		T sy = std::sin(aYaw   * T(0.5));
		T cp = std::cos(aPitch * T(0.5));
		T sp = std::sin(aPitch * T(0.5));

		w = cy * cr * cp + sy * sr * sp;
		x = cy * sr * cp - sy * cr * sp;
		y = cy * cr * sp + sy * sr * cp;
		z = sy * cr * cp - cy * sr * sp;
	}

	template<typename T>
	constexpr Quaternion<T>::Quaternion(const Vector3<T>& aYawPitchRoll)
		: Quaternion(aYawPitchRoll.x, aYawPitchRoll.y, aYawPitchRoll.z)
	{

	}

	template<typename T>
	constexpr Quaternion<T>::Quaternion(const Vector3<T>& aVector, T aAngle)
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
	constexpr Quaternion<T>::Quaternion(const Matrix4x4<T>& aMatrix)
	{
		w = std::sqrt((std::max)(T(0), T(1) + aMatrix[0] + aMatrix[5] + aMatrix[10])) * T(0.5);
		x = std::sqrt((std::max)(T(0), T(1) + aMatrix[0] - aMatrix[5] - aMatrix[10])) * T(0.5);
		y = std::sqrt((std::max)(T(0), T(1) - aMatrix[0] + aMatrix[5] - aMatrix[10])) * T(0.5);
		z = std::sqrt((std::max)(T(0), T(1) - aMatrix[0] - aMatrix[5] + aMatrix[10])) * T(0.5);
		x = std::copysign(x, aMatrix[6] - aMatrix[9]);
		y = std::copysign(y, aMatrix[8] - aMatrix[2]);
		z = std::copysign(z, aMatrix[1] - aMatrix[4]);
	}

	template<typename T>
	constexpr void Quaternion<T>::Normalize(T aNormLength)
	{
		*this = GetNormalized(aNormLength);
	}

	template<typename T>
	constexpr void Quaternion<T>::RotateWithEuler(const Vector3<T>& aEuler)
	{

	}
	template<typename T>
	constexpr void Quaternion<T>::RotateWithRadians(const Vector3<T>& aRadians)
	{

	}

	template<typename T>
	constexpr Quaternion<T> Quaternion<T>::GetNormalized(T aNormLength) const
	{
		T length = Length();
		assert(length > T{} && "Negative or zero length is an error");
		return (*this) * (aNormLength / length);
	}
	template<typename T>
	constexpr Quaternion<T> Quaternion<T>::GetConjugate() const
	{
		return Quaternion<T>(w, -x, -y, -z);
	}

	template<typename T>
	constexpr T Quaternion<T>::Dot(const Quaternion<T>& aQuat) const
	{
		return x * aQuat.x + y * aQuat.y + z * aQuat.z + w * aQuat.w;
	}

	template<typename T>
	constexpr T Quaternion<T>::Length() const
	{
		return std::sqrt(LengthSqr());
	}
	template<typename T>
	constexpr T Quaternion<T>::LengthSqr() const 
	{
		return Dot(*this);
	}

	template<typename T>
	constexpr Vector3<T> Quaternion<T>::GetEulerAnglesRadians() const
	{

	}
	template<typename T>
	constexpr Vector3<T> Quaternion<T>::GetEulerAnglesRadiansd() const 
	{

	}
	template<typename T>
	constexpr Vector3<T> Quaternion<T>::GetEulerAnglesDegrees() const
	{

	}

	template<typename T>
	constexpr Vector3<T> Quaternion<T>::GetRight() const
	{

	}
	template<typename T>
	constexpr Vector3<T> Quaternion<T>::GetUp() const 
	{

	}
	template<typename T>
	constexpr Vector3<T> Quaternion<T>::GetForward() const 
	{

	}

	template<typename T>
	constexpr Quaternion<T> Quaternion<T>::GetInverse() const
	{
		return GetConjugate() / Length();
	}

	template<typename T>
	constexpr Quaternion<T> Quaternion<T>::Lerp(const Quaternion<T>& aQuatA, const Quaternion<T>& aQuatB, T aDelta)
	{

	}
	template<typename T>
	constexpr Quaternion<T> Quaternion<T>::Slerp(const Quaternion<T>& aQuatA, const Quaternion<T>& aQuatB, T aDelta)
	{

	}

	template<typename T>
	constexpr Vector3<T> Quaternion<T>::RotateVectorByQuaternion(const Quaternion<T>& aQuaternion, const Vector3f& aVectorToRotate)
	{
		Quaternion<T> rVec = ((aQuaternion * Quaternion<T>(0, aVectorToRotate.x, aVectorToRotate.y, aVectorToRotate.z)) * aQuaternion.GetConjugate());
		return Vector3<T>(rVec.x, rVec.y, rVec.z);
	}

	// GLOBAL OPERATORS

	template <typename T>
	NODISC constexpr Quaternion<T> operator-(const Quaternion<T>& aQuat)
	{
		return Quaternion<T>(-aQuat.w, -aQuat.x, -aQuat.y, -aQuat.z);
	}

	template <typename T>
	NODISC constexpr Quaternion<T> operator+(const Quaternion<T>& aQuatA, const Quaternion<T>& aQuatB)
	{
		return Quaternion<T>(aQuatA.w + aQuatB.w, aQuatA.x + aQuatB.x, aQuatA.y + aQuatB.y, aQuatA.z + aQuatB.z);
	}

	template <typename T>
	NODISC constexpr Quaternion<T> operator-(const Quaternion<T>& aQuatA, const Quaternion<T>& aQuatB)
	{
		return Quaternion<T>(aQuatA.w - aQuatB.w, aQuatA.x - aQuatB.x, aQuatA.y - aQuatB.y, aQuatA.z - aQuatB.z);
	}

	template <typename T>
	NODISC constexpr Quaternion<T> operator*(const Quaternion<T>& aQuatA, const Quaternion<T>& aQuatB)
	{
		return Quaternion<T>
		(
			(aQuatB.w * aQuatA.w) - (aQuatB.x * aQuatA.x) - (aQuatB.y * aQuatA.y) - (aQuatB.z * aQuatA.z),
			(aQuatB.w * aQuatA.x) + (aQuatB.x * aQuatA.w) + (aQuatB.y * aQuatA.z) - (aQuatB.z * aQuatA.y),
			(aQuatB.w * aQuatA.y) + (aQuatB.y * aQuatA.w) + (aQuatB.z * aQuatA.x) - (aQuatB.x * aQuatA.z),
			(aQuatB.w * aQuatA.z) + (aQuatB.z * aQuatA.w) + (aQuatB.x * aQuatA.y) - (aQuatB.y * aQuatA.x)
		);
	}

	template <typename T>
	NODISC constexpr Quaternion<T> operator*(const Quaternion<T>& aQuat, T aScalar)
	{
		return Quaternion<T>(aQuat.w * aScalar, aQuat.x * aScalar, aQuat.y * aScalar, aQuat.z * aScalar);
	}

	template <typename T>
	NODISC constexpr Quaternion<T> operator*(T aScalar, const Quaternion<T>& aQuat)
	{
		return Quaternion<T>(aQuat.w * aScalar, aQuat.x * aScalar, aQuat.y * aScalar, aQuat.z * aScalar);
	}

	template <typename T>
	NODISC constexpr Quaternion<T> operator/(const Quaternion<T>& aQuat, T aScalar)
	{
		assert(aScalar != 0 && "Trying to divide by zero!");
		return Quaternion<T>(aQuat.w / aScalar, aQuat.x / aScalar, aQuat.y / aScalar, aQuat.z / aScalar);
	}

	template <typename T>
	constexpr Quaternion<T>& operator+=(Quaternion<T>& aQuatA, const Quaternion<T>& aQuatB)
	{
		aQuatA.w += aQuatB.w;
		aQuatA.x += aQuatB.x;
		aQuatA.y += aQuatB.y;
		aQuatA.z += aQuatB.z;

		return aQuatA;
	}

	template <typename T>
	constexpr Quaternion<T>& operator-=(Quaternion<T>& aQuatA, const Quaternion<T>& aQuatB)
	{
		aQuatA.w -= aQuatB.w;
		aQuatA.x -= aQuatB.x;
		aQuatA.y -= aQuatB.y;
		aQuatA.z -= aQuatB.z;

		return aQuatA;
	}

	template <typename T>
	constexpr Quaternion<T>& operator*=(Quaternion<T>& aQuatA, const Quaternion<T>& aQuatB)
	{
		T w = aQuatA.w;
		T x = aQuatA.x;
		T y = aQuatA.y;
		T z = aQuatA.z;

		aQuatA.w = (aQuatB.w * w) - (aQuatB.x * x) - (aQuatB.y * y) - (aQuatB.z * z);
		aQuatA.x = (aQuatB.w * x) + (aQuatB.x * w) + (aQuatB.y * z) - (aQuatB.z * y);
		aQuatA.y = (aQuatB.w * y) + (aQuatB.y * w) + (aQuatB.z * x) - (aQuatB.x * z);
		aQuatA.z = (aQuatB.w * z) + (aQuatB.z * w) + (aQuatB.x * y) - (aQuatB.y * x);

		return aQuatA;
	}

	template <typename T>
	constexpr Quaternion<T>& operator*=(Quaternion<T>& aQuat, const T& aScalar)
	{
		aQuat.w *= aScalar;
		aQuat.x *= aScalar;
		aQuat.y *= aScalar;
		aQuat.z *= aScalar;

		return aQuat;
	}

	using Quatf = Quaternion<float>;
	using Quatd = Quaternion<double>;
}
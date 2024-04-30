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

		constexpr Quaternion() = default;

		constexpr Quaternion(T aW, T aX, T aY, T aZ);
		constexpr Quaternion(T aYaw, T aPitch, T aRoll);
		constexpr Quaternion(const Vector3<T>& aYawPitchRoll);
		constexpr Quaternion(const Vector3<T>& aVector, T aAngle);
		constexpr Quaternion(const Matrix4x4<T>& aMatrix);

		constexpr void RotateWithEuler(const Vector3<T>& aEuler);
		constexpr void RotateWithRadians(const Vector3<T>& aRadians);

		constexpr void Normalize();

		NODISC constexpr Quaternion<T> GetNormalized() const;
		NODISC constexpr Quaternion<T> GetConjugate() const;

		NODISC constexpr T Length() const;
		NODISC constexpr T LengthSqr() const;

		NODISC constexpr Vector3<T> GetEulerAnglesRadians() const;
		NODISC constexpr Vector3<T> GetEulerAnglesRadiansd() const;
		NODISC constexpr Vector3<T> GetEulerAnglesDegrees() const;

		NODISC constexpr T Dot(const Quaternion<T>& aQuat) const;

		NODISC constexpr Vector3<T> GetRight() const;
		NODISC constexpr Vector3<T> GetUp() const;
		NODISC constexpr Vector3<T> GetForward() const;

		NODISC constexpr static Quaternion<T> Lerp(const Quaternion<T>& aQuatA, const Quaternion<T>& aQuatB, T aDelta);
		NODISC constexpr static Quaternion<T> Slerp(const Quaternion<T>& aQuatA, const Quaternion<T>& aQuatB, T aDelta);

		NODISC constexpr static Vector3<T> RotateVectorByQuaternion(const Quaternion<T>& aQuaternion, const Vector3f& aVectorToRotate);
	};

	template<typename T>
	constexpr Quaternion<T>::Quaternion(T aW, T aX, T aY, T aZ)
	{
		w = aW;
		x = aX;
		y = aY;
		z = aZ;
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

	using Quatf = Quaternion<float>;
	using Quatd = Quaternion<double>;
}
#pragma once

#include <cmath>
#include <cassert>

#include <CommonUtilities/Utility/ArithmeticUtils.hpp>

#include "Vector3.hpp"

namespace CommonUtilities
{
	template <class T>
	class Matrix4x4;

	template<typename T>
	class Quaternion
	{
	public:
		T w{1}; // scalar
		T x{};
		T y{};
		T z{};

		constexpr Quaternion() = default;

		constexpr explicit Quaternion(T aW, T aX, T aY, T aZ);
		constexpr explicit Quaternion(T aYaw, T aPitch, T aRoll);
		constexpr explicit Quaternion(const Vector3<T>& aPitchYawRoll);
		constexpr explicit Quaternion(const Vector3<T>& aVector, T aAngle);
		constexpr explicit Quaternion(const Matrix4x4<T>& aMatrix);

		constexpr void Normalize(T aNormLength = T(1));

		constexpr void RotateWithEuler(const Vector3<T>& aEuler);

		NODISC constexpr T Dot(const Quaternion<T>& aQuat) const;

		/// Norm of the quaternion
		/// 
		NODISC constexpr T Length() const;
		NODISC constexpr T LengthSqr() const;

		NODISC constexpr Quaternion GetNormalized(T aNormLength = static_cast<T>(1)) const;
		NODISC constexpr Quaternion GetConjugate() const;

		NODISC constexpr Vector3<T> GetEulerAngles() const;
		NODISC constexpr Vector3<T> GetEulerAnglesDetect() const;

		NODISC constexpr Vector3<T> GetRight() const;
		NODISC constexpr Vector3<T> GetUp() const;
		NODISC constexpr Vector3<T> GetForward() const;

		NODISC constexpr Vector4<T> ToVec() const;

		NODISC constexpr Quaternion GetInverse() const;

		constexpr Quaternion& Add(const Quaternion& aRight);
		constexpr Quaternion& Subtract(const Quaternion& aRight);
		constexpr Quaternion& Combine(const Quaternion& aRight);

		NODISC constexpr static Quaternion Lerp(const Quaternion& aQuatA, const Quaternion& aQuatB, T aDelta);
		NODISC constexpr static Quaternion Slerp(const Quaternion& aQuatA, const Quaternion& aQuatB, T aDelta);

		NODISC constexpr static Vector3<T> RotateVectorByQuaternion(const Quaternion& aQuaternion, const Vector3<T>& aVectorToRotate);
		NODISC constexpr static Quaternion RotationFromTo(Vector3<T> aFrom, Vector3<T> aTo);

		static const Quaternion IDENTITY;
	};

	template<typename T>
	constexpr Quaternion<T>::Quaternion(T aW, T aX, T aY, T aZ)
		: w(aW), x(aX), y(aY), z(aZ)
	{
	}

	template<typename T>
	constexpr Quaternion<T>::Quaternion(T aYaw, T aPitch, T aRoll)
	{
		T cx = std::cos(aPitch	* T(0.5));
		T sx = std::sin(aPitch	* T(0.5));
		T cy = std::cos(aYaw	* T(0.5));
		T sy = std::sin(aYaw	* T(0.5));
		T cz = std::cos(aRoll	* T(0.5));
		T sz = std::sin(aRoll	* T(0.5));

		w = cx * cy * cz + sx * sy * sz;
		x = sx * cy * cz - cx * sy * sz;
		y = cx * sy * cz + sx * cy * sz;
		z = cx * cy * sz - sx * sy * cz;
	}

	template<typename T>
	constexpr Quaternion<T>::Quaternion(const Vector3<T>& aPitchYawRoll)
		: Quaternion(aPitchYawRoll.y, aPitchYawRoll.x, aPitchYawRoll.z)
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
		*this *= Quaternion(aEuler);
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
	constexpr T Quaternion<T>::Dot(const Quaternion& aQuat) const
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
	constexpr Vector3<T> Quaternion<T>::GetEulerAngles() const
	{
		// Read more about it here: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

		const T sqx = x * x;
		const T sqy = y * y;
		const T sqz = z * z;

		// pitch (x-axis rotation)
		const T sinp	= T(2) * (w * x + y * z);
		const T cosp	= T(1) - T(2) * (sqx + sqy);
		const T pitch	= std::atan2(sinp, cosp);

		// yaw (y-axis rotation)
		const T siny = T(2) * (w * y - z * x);

		T yaw = (std::abs(siny) > T(0.99999) ? std::copysign(au::PI_2_V<T>, siny) : std::asin(siny));

		// roll (z-axis rotation)
		const T sinr	= T(2) * (w * z + x * y);
		const T cosr	= T(1) - T(2) * (sqy + sqz);
		const T roll	= std::atan2(sinr, cosr);

		return Vector3<T>(pitch, yaw, roll);
	}
	template<typename T>
	constexpr Vector3<T> Quaternion<T>::GetEulerAnglesDetect() const
	{
		// Read more about it here: https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/

		const T test = T(2) * (w * y - z * x);

		if (test > T(0.99999)) // singularity at north pole
		{ 
			const T pitch	= 0;
			const T yaw		= au::PI_2_V<T>;
			const T roll	= -T(2) * std::atan2(x, w);

			return Vector3<T>{ pitch, yaw, roll };
		}

		if (test < -T(0.99999)) // singularity at south pole
		{ 
			const T pitch	= 0;
			const T yaw		= -au::PI_2_V<T>;;
			const T roll	= T(2) * std::atan2(x, w);

			return Vector3<T>{ pitch, yaw, roll };
		}

		const T sqw = w * w;
		const T sqx = x * x;
		const T sqy = y * y;
		const T sqz = z * z;

		const T pitch	= std::atan2(T(2) * (w * x + y * z), -sqx - sqy + sqz + sqw);
		const T yaw		= std::asin(test);
		const T roll	= std::atan2(T(2) * (w * z + x * y), sqx - sqy - sqz + sqw);

		return Vector3<T>{ pitch, yaw, roll };
	}

	template<typename T>
	constexpr Vector3<T> Quaternion<T>::GetRight() const
	{
		return Quaternion<T>::RotateVectorByQuaternion(*this, { 1, 0, 0 });;
	}
	template<typename T>
	constexpr Vector3<T> Quaternion<T>::GetUp() const 
	{
		return Quaternion<T>::RotateVectorByQuaternion(*this, { 0, 1, 0 });;
	}
	template<typename T>
	constexpr Vector3<T> Quaternion<T>::GetForward() const 
	{
		return Quaternion<T>::RotateVectorByQuaternion(*this, { 0, 0, 1 });;
	}

	template<typename T>
	inline constexpr Vector4<T> Quaternion<T>::ToVec() const
	{
		return Vector4<T>(x, y, z, w);
	}

	template<typename T>
	constexpr Quaternion<T> Quaternion<T>::GetInverse() const
	{
		return GetConjugate() / Length();
	}

	template<typename T>
	constexpr Quaternion<T>& Quaternion<T>::Add(const Quaternion& aRight)
	{
		w += aRight.w;
		x += aRight.x;
		y += aRight.y;
		z += aRight.z;

		return *this;
	}
	template<typename T>
	constexpr Quaternion<T>& Quaternion<T>::Subtract(const Quaternion& aRight)
	{
		w -= aRight.w;
		x -= aRight.x;
		y -= aRight.y;
		z -= aRight.z;

		return *this;
	}
	template<typename T>
	constexpr Quaternion<T>& Quaternion<T>::Combine(const Quaternion& aRight)
	{
		*this = Quaternion<T>
		(
			(aRight.w * w) - (aRight.x * x) - (aRight.y * y) - (aRight.z *z),
			(aRight.w * x) + (aRight.x * w) + (aRight.y * z) - (aRight.z *y),
			(aRight.w * y) + (aRight.y * w) + (aRight.z * x) - (aRight.x *z),
			(aRight.w * z) + (aRight.z * w) + (aRight.x * y) - (aRight.y *x)
		);

		return *this;
	}

	template<typename T>
	constexpr Quaternion<T> Quaternion<T>::Lerp(const Quaternion& aQuatA, const Quaternion& aQuatB, T aDelta)
	{
		Quaternion<T> result;

		// basically just a normal vec4 lerp

		T deltaInv = T(1) - aDelta;
		result.w = deltaInv * aQuatA.w + aDelta * aQuatB.w;
		result.x = deltaInv * aQuatA.x + aDelta * aQuatB.x;
		result.y = deltaInv * aQuatA.y + aDelta * aQuatB.y;
		result.z = deltaInv * aQuatA.z + aDelta * aQuatB.z;
		result.Normalize();

		return result;
	}
	template<typename T>
	constexpr Quaternion<T> Quaternion<T>::Slerp(const Quaternion& aQuatA, const Quaternion& aQuatB, T aDelta)
	{
		Quaternion<T> qz = aQuatB;

		T cosTheta = aQuatA.Dot(aQuatB);

		// If cosTheta < 0, the interpolation will take the long way around the sphere. 
		// To fix this, one quat must be negated.
		if (cosTheta < T(0))
		{
			cosTheta = -cosTheta;
			qz = -qz;
		}

		// Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
		if (cosTheta > T(1) - T(0.9995))
		{
			// Linear interpolation
			return Lerp(aQuatA, qz, aDelta);
		}

		// Essential Mathematics, page 467

		T angle = std::acos(cosTheta);
		return (std::sin((T(1) - aDelta) * angle) * aQuatA + std::sin(aDelta * angle) * qz) / std::sin(angle);
	}

	template<typename T>
	constexpr Vector3<T> Quaternion<T>::RotateVectorByQuaternion(const Quaternion& aQuaternion, const Vector3<T>& aVectorToRotate)
	{
		const Vector3<T> v(aQuaternion.x, aQuaternion.y, aQuaternion.z);
		const T s = aQuaternion.w;

		return 2.0f * v.Dot(aVectorToRotate) * v
			+ (s * s - v.Dot(v)) * aVectorToRotate
			+ 2.0f * s * v.Cross(aVectorToRotate);
	}

	template<typename T>
	constexpr Quaternion<T> Quaternion<T>::RotationFromTo(Vector3<T> aFrom, Vector3<T> aTo)
	{
		aFrom.Normalize();
		aTo.Normalize();

		const float d = aFrom.Dot(aTo);
		if (d >= T(1)) // same direction
		{
			return IDENTITY; 
		}
		else if (d >= -T(1)) // opposites
		{
			Vector3<T> axis(1, 0, 0);
			axis = axis.Cross(aFrom);
			if (axis == Vector3<T>::Zero)
			{
				axis = Vector3<T>(0, 1, 0);
				axis = axis.Cross(aFrom);
			}

			return Quaternion(T(0), axis.x, axis.y, axis.z).GetNormalized();
		}

		const T s			= std::sqrt((T(1) + d) * T(2));
		const T invs		= T(1) / s;
		const Vector3<T> c	= aFrom.Cross(aTo) * invs;

		return Quaternion(s * T(0.5), c.x, c.y, c.z).GetNormalized();
	}

	// GLOBAL OPERATORS

	template <typename T>
	NODISC constexpr Quaternion<T> operator-(const Quaternion<T>& aQuat)
	{
		return Quaternion<T>(-aQuat.w, -aQuat.x, -aQuat.y, -aQuat.z);
	}

	template <typename T>
	NODISC constexpr Quaternion<T> operator+(const Quaternion<T>& aLeft, const Quaternion<T>& aRight)
	{
		return Quaternion<T>(aLeft).Add(aRight);
	}
	template <typename T>
	NODISC constexpr Quaternion<T> operator-(const Quaternion<T>& aLeft, const Quaternion<T>& aRight)
	{
		return Quaternion<T>(aLeft).Subtract(aRight);
	}
	template <typename T>
	NODISC constexpr Quaternion<T> operator*(const Quaternion<T>& aLeft, const Quaternion<T>& aRight)
	{
		return Quaternion<T>(aLeft).Combine(aRight);
	}

	template <typename T>
	NODISC constexpr Quaternion<T> operator*(const Quaternion<T>& aLeft, T aRight)
	{
		return Quaternion<T>(aLeft.w * aRight, aLeft.x * aRight, aLeft.y * aRight, aLeft.z * aRight);
	}
	template <typename T>
	NODISC constexpr Quaternion<T> operator*(T aLeft, const Quaternion<T>& aRight)
	{
		return aRight * aLeft;
	}

	template <typename T>
	NODISC constexpr Quaternion<T> operator/(const Quaternion<T>& aLeft, T aRight)
	{
		assert(aRight != 0 && "Trying to divide by zero!");
		return Quaternion<T>(aLeft.w / aRight, aLeft.x / aRight, aLeft.y / aRight, aLeft.z / aRight);
	}

	template <typename T>
	constexpr Quaternion<T>& operator+=(Quaternion<T>& aLeft, const Quaternion<T>& aRight)
	{
		return aLeft.Add(aRight);
	}
	template <typename T>
	constexpr Quaternion<T>& operator-=(Quaternion<T>& aLeft, const Quaternion<T>& aRight)
	{
		return aLeft.Subtract(aRight);
	}
	template <typename T>
	constexpr Quaternion<T>& operator*=(Quaternion<T>& aLeft, const Quaternion<T>& aRight)
	{
		return aLeft.Combine(aRight);
	}

	template <typename T>
	constexpr Quaternion<T>& operator*=(Quaternion<T>& aLeft, T aRight)
	{
		aLeft.w *= aRight;
		aLeft.x *= aRight;
		aLeft.y *= aRight;
		aLeft.z *= aRight;

		return aLeft;
	}

	template<typename T>
	inline const Quaternion<T> Quaternion<T>::IDENTITY;

	using Quatf = Quaternion<float>;
	using Quatd = Quaternion<double>;
}
#pragma once

#include <cmath>
#include <utility>
#include <cassert>

#include <CommonUtilities/Utility/ArithmeticUtils.hpp>
#include <CommonUtilities/Utility/Traits.h>

#include "Vector3.hpp"

namespace CommonUtilities
{
	enum class AxisOrder
	{
		XYZ,
		XZY,
		YXZ,
		YZX,
		ZXY,
		ZYX
	};

	inline constexpr AxisOrder RotationOrder = AxisOrder::XYZ;

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
		constexpr explicit Quaternion(T aYaw, T aPitch, T aRoll, AxisOrder aRotationOrder);
		constexpr explicit Quaternion(const Vector3<T>& aPitchYawRoll);
		constexpr explicit Quaternion(const Vector3<T>& aPitchYawRoll, AxisOrder aRotationOrder);
		constexpr explicit Quaternion(const Vector3<T>& aVector, T aAngle);
		constexpr explicit Quaternion(const Matrix4x4<T>& aMatrix);

		constexpr void Normalize(T aNormLength = T(1));

		constexpr void RotateWithEuler(const Vector3<T>& aEuler);

		NODISC constexpr T Dot(const Quaternion<T>& aQuat) const;

		/// Norm of the quaternion
		/// 
		NODISC constexpr T Length() const;
		NODISC constexpr T LengthSqr() const;

		NODISC constexpr T Angle() const;
		NODISC constexpr T AngleTo(const Quaternion& aRight) const;

		NODISC constexpr Quaternion GetNormalized(T aNormLength = static_cast<T>(1)) const;
		NODISC constexpr Quaternion GetConjugate() const;

		NODISC constexpr Vector3<T> ToEuler() const;
		NODISC constexpr std::pair<T, Vector3<T>> ToRadiansUnitAxis() const;

		NODISC constexpr Vector3<T> GetRight() const;
		NODISC constexpr Vector3<T> GetUp() const;
		NODISC constexpr Vector3<T> GetForward() const;

		NODISC constexpr Vector4<T> ToVec() const;

		NODISC constexpr Quaternion GetInverse() const;

		NODISC constexpr Quaternion GetAbs() const;

		constexpr Quaternion& Add(const Quaternion& aRight);
		constexpr Quaternion& Subtract(const Quaternion& aRight);
		constexpr Quaternion& Combine(const Quaternion& aRight);

		NODISC constexpr static Quaternion Lerp(const Quaternion& aQuatA, const Quaternion& aQuatB, T aDelta);
		NODISC constexpr static Quaternion Slerp(const Quaternion& aQuatA, const Quaternion& aQuatB, T aDelta);

		/// Will ignore shorter slerp paths, if any.
		/// 
		NODISC constexpr static Quaternion SlerpLong(const Quaternion& aQuatA, const Quaternion& aQuatB, T aDelta);

		NODISC constexpr static Quaternion AxisAngle(const Vector3<T>& aVector, T aAngle);
		NODISC constexpr static Quaternion LookRotation(const Vector3<T>& aForward, const Vector3<T>& aUp = Vector3<T>::Up);
		NODISC constexpr static Quaternion RotateTowards(Quaternion aQuatA, Quaternion aQuatB, T aMaxRadiansDelta);
		NODISC constexpr static Vector3<T> RotateVectorByQuaternion(const Quaternion& aQuaternion, const Vector3<T>& aVectorToRotate);
		NODISC constexpr static Quaternion RotationFromTo(Vector3<T> aFrom, Vector3<T> aTo);
		NODISC constexpr static Quaternion Difference(Quaternion aQuatA, Quaternion aQuatB);

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
		Quaternion xRot(Vector3<T>(1, 0, 0), aPitch);
		Quaternion yRot(Vector3<T>(0, 1, 0), aYaw);
		Quaternion zRot(Vector3<T>(0, 0, 1), aRoll);

		if constexpr (RotationOrder == AxisOrder::XYZ)
			*this = xRot * yRot * zRot;
		else if constexpr (RotationOrder == AxisOrder::XZY)
			*this = xRot * zRot * yRot;
		else if constexpr (RotationOrder == AxisOrder::YXZ)
			*this = yRot * xRot * zRot;
		else if constexpr (RotationOrder == AxisOrder::YZX)
			*this = yRot * zRot * xRot;
		else if constexpr (RotationOrder == AxisOrder::ZXY)
			*this = zRot * xRot * yRot;
		else if constexpr (RotationOrder == AxisOrder::ZYX)
			*this = zRot * yRot * xRot;
		else
			static_assert(tr::DependentFalse<T>::value, "Invalid rotation order selected");
	}

	template<typename T>
	constexpr Quaternion<T>::Quaternion(T aYaw, T aPitch, T aRoll, AxisOrder aRotationOrder)
	{
		Quaternion xRot(Vector3<T>(1, 0, 0), aPitch);
		Quaternion yRot(Vector3<T>(0, 1, 0), aYaw);
		Quaternion zRot(Vector3<T>(0, 0, 1), aRoll);

		switch (aRotationOrder)
		{
			case AxisOrder::XYZ: *this = xRot * yRot * zRot; break;
			case AxisOrder::XZY: *this = xRot * zRot * yRot; break;
			case AxisOrder::YXZ: *this = yRot * xRot * zRot; break;
			case AxisOrder::YZX: *this = yRot * zRot * xRot; break;
			case AxisOrder::ZXY: *this = zRot * xRot * yRot; break;
			case AxisOrder::ZYX: *this = zRot * yRot * xRot; break;
			[[unlikely]] default: 
				assert(false && "Invalid rotation order selected"); 
				break;
		}
	}

	template<typename T>
	constexpr Quaternion<T>::Quaternion(const Vector3<T>& aPitchYawRoll)
		: Quaternion(aPitchYawRoll.y, aPitchYawRoll.x, aPitchYawRoll.z)
	{

	}

	template<typename T>
	constexpr Quaternion<T>::Quaternion(const Vector3<T>& aPitchYawRoll, AxisOrder aRotationOrder)
		: Quaternion(aPitchYawRoll.y, aPitchYawRoll.x, aPitchYawRoll.z, aRotationOrder)
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
		T trace = aMatrix[0] + aMatrix[5] + aMatrix[10];
		if (trace > T(0))
		{
			float s = T(0.5) / std::sqrt(trace + T(1));
			w = T(0.25) / s;
			x = (aMatrix[6] - aMatrix[9]) * s;
			y = (aMatrix[8] - aMatrix[2]) * s;
			z = (aMatrix[1] - aMatrix[4]) * s;
		}
		else 
		{
			if (aMatrix[0] > aMatrix[5] && aMatrix[0] > aMatrix[10])
			{
				T s = T(2) * std::sqrt(1.0f + aMatrix[0] - aMatrix[5] - aMatrix[10]);
				w = (aMatrix[6] - aMatrix[9]) / s;
				x = T(0.25) * s;
				y = (aMatrix[4] + aMatrix[1]) / s;
				z = (aMatrix[8] + aMatrix[2]) / s;
			}
			else if (aMatrix[5] > aMatrix[10]) 
			{
				T s = T(2) * std::sqrt(1.0f + aMatrix[5] - aMatrix[0] - aMatrix[10]);
				w = (aMatrix[8] - aMatrix[2]) / s;
				x = (aMatrix[4] + aMatrix[1]) / s;
				y = T(0.25) * s;
				z = (aMatrix[9] + aMatrix[6]) / s;
			}
			else
			{
				T s = T(2) * std::sqrt(1.0f + aMatrix[10] - aMatrix[0] - aMatrix[5]);
				w = (aMatrix[1] - aMatrix[4]) / s;
				x = (aMatrix[8] + aMatrix[2]) / s;
				y = (aMatrix[9] + aMatrix[6]) / s;
				z = T(0.25) * s;
			}
		}
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
	constexpr T Quaternion<T>::Angle() const
	{
		return std::acos(w) * T(2);
	}
	template<typename T>
	constexpr T Quaternion<T>::AngleTo(const Quaternion& aRight) const
	{
		return std::acos(Clamp(std::abs(Dot(aRight)), T(-1), T(1))) * T(2);
	}

	template<typename T>
	constexpr Vector3<T> Quaternion<T>::ToEuler() const
	{
		// Read more about it here: https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/

		const T test = T(2) * (w * y - z * x);

		if (test > T(0.999999999)) // singularity at north pole
		{ 
			const T pitch	= -T(2) * std::atan2(x, w);
			const T yaw		= PI_2_V<T>;
			const T roll	= 0;

			return Vector3<T>{ pitch, yaw, roll };
		}

		if (test < T(-0.999999999)) // singularity at south pole
		{ 
			const T pitch	= T(2) * std::atan2(x, w);
			const T yaw		= -PI_2_V<T>;
			const T roll	= 0;

			return Vector3<T>{ pitch, yaw, roll };
		}

		// TODO: FIX THIS CRAP

		const T sqx = x * x;
		const T sqy = y * y;
		const T sqz = z * z;

		const T siny = std::sqrt(T(1) + test);
		const T cosp = std::sqrt(T(1) - test);

		const T pitch	= std::atan2(T(2) * (w * x + y * z), T(1) - T(2) * (sqx + sqy));
		const T yaw		= T(2) * std::atan2(siny, cosp) - PI_2;
		const T roll	= std::atan2(T(2) * (w * z + x * y), T(1) - T(2) * (sqy + sqz));

		return Vector3<T>{ pitch, yaw, roll };
	}

	template<typename T>
	constexpr std::pair<T, Vector3<T>> Quaternion<T>::ToRadiansUnitAxis() const
	{
		static constexpr T epsilon = T(1.0e-8);

		T angle = T(0);
		Vector3<T> axis = Vector3<T>::Right;

		const T s2 = x * x + y * y + z * z;
		if (s2 >= epsilon * epsilon)
		{
			const T s = T(1) / std::sqrt(s2);

			axis	= Vector3<T>(x, y, z) * s;
			angle	= std::abs(w) < epsilon ? PI_V<T> : std::atan2(s2 * s, w) * T(2);
		}

		return std::make_pair(angle, axis);
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
	constexpr Vector4<T> Quaternion<T>::ToVec() const
	{
		return Vector4<T>(x, y, z, w);
	}

	template<typename T>
	constexpr Quaternion<T> Quaternion<T>::GetInverse() const
	{
		return GetConjugate() / Length();
	}

	template<typename T>
	constexpr Quaternion<T> Quaternion<T>::GetAbs() const
	{
		return w < T(0) ? -*this : *this;
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
			(aRight.w * w) - (aRight.x * x) - (aRight.y * y) - (aRight.z * z),
			(aRight.w * x) + (aRight.x * w) + (aRight.y * z) - (aRight.z * y),
			(aRight.w * y) + (aRight.y * w) + (aRight.z * x) - (aRight.x * z),
			(aRight.w * z) + (aRight.z * w) + (aRight.x * y) - (aRight.y * x)
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
		if (cosTheta >= T(0.999999))
		{
			// Linear interpolation
			return Lerp(aQuatA, qz, aDelta);
		}

		// Essential Mathematics, page 467

		const T angle = std::acos(cosTheta);
		return (std::sin((T(1) - aDelta) * angle) * aQuatA + std::sin(aDelta * angle) * qz) / std::sin(angle);
	}

	template<typename T>
	constexpr Quaternion<T> Quaternion<T>::SlerpLong(const Quaternion& aQuatA, const Quaternion& aQuatB, T aDelta)
	{
		Quaternion<T> qz = aQuatB;

		T cosTheta = aQuatA.Dot(aQuatB);

		// If cosTheta < 0, the interpolation will take the long way around the sphere. 
		// In this function, that is probably intended.

		// Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
		if (cosTheta >= T(0.999999))
		{
			// Linear interpolation
			return Lerp(aQuatA, qz, aDelta);
		}

		// Essential Mathematics, page 467

		const T angle = std::acos(cosTheta);
		return (std::sin((T(1) - aDelta) * angle) * aQuatA + std::sin(aDelta * angle) * qz) / std::sin(angle);
	}

	template<typename T>
	constexpr Quaternion<T> Quaternion<T>::AxisAngle(const Vector3<T>& aVector, T aAngle)
	{
		return Quaternion(aVector, aAngle);
	}
	template<typename T>
	constexpr Quaternion<T> Quaternion<T>::LookRotation(const Vector3<T>& aForward, const Vector3<T>& aUp)
	{
		if (aForward.LengthSqr() < T(0.000001))
			return Quaternion<T>();

		const Vector3<T> f = aForward.GetNormalized();
		const Vector3<T> u = aUp.GetNormalized();

		const Quaternion<T> rot1 = Quaternion<T>::RotationFromTo(Vector3<T>::Forward, f);

		const Vector3<T> newUp = rot1 * Vector3<T>::Up;

		const Vector3<T> xAxis = !Vector3<T>::Equal(u.GetAbs(), f.GetAbs(), T(0.000001)) ? u.Cross(f) : Vector3<T>::Right;
		const Vector3<T> yAxis = f.Cross(xAxis);

		const Quaternion<T> rot2 = Quaternion<T>::RotationFromTo(newUp, yAxis);

		return rot1 * rot2;
	}
	template<typename T>
	constexpr Quaternion<T> Quaternion<T>::RotateTowards(Quaternion aQuatA, Quaternion aQuatB, T aMaxRadiansDelta)
	{
		if (aMaxRadiansDelta <= EPSILON_V<T>)
			return aQuatA;

		T cosTheta = aQuatA.Dot(aQuatB);

		if (cosTheta >= T(0.999999999)) // already equal
			return aQuatB;

		if (cosTheta < T(0))
		{
			aQuatA = -aQuatA;
			cosTheta = -cosTheta;
		}

		T angle = std::acos(Clamp(cosTheta, T(-1), T(1)));

		if (angle <= aMaxRadiansDelta) // arrived
			return aQuatB;

		const T fT = aMaxRadiansDelta / angle;
		angle = aMaxRadiansDelta;

		Quaternion<T> result = (std::sin((T(1) - fT) * angle) * aQuatA + std::sin(fT * angle) * aQuatB) / std::sin(angle);
		return result.GetNormalized();
	}
	template<typename T>
	constexpr Vector3<T> Quaternion<T>::RotateVectorByQuaternion(const Quaternion& aQuaternion, const Vector3<T>& aVectorToRotate)
	{
		const Vector3<T> qv = { aQuaternion.x, aQuaternion.y, aQuaternion.z };
		const Vector3<T> t = T(2) * qv.Cross(aVectorToRotate);

		return aVectorToRotate + aQuaternion.w * t + qv.Cross(t);
	}

	template<typename T>
	constexpr Quaternion<T> Quaternion<T>::RotationFromTo(Vector3<T> aFrom, Vector3<T> aTo)
	{
		aFrom.Normalize();
		aTo.Normalize();

		const T d = aFrom.Dot(aTo);

		if (d >= T(0.999999999)) // same direction
		{
			return IDENTITY; 
		}
		else if (d <= T(-0.999999999)) // opposites
		{
			Vector3<T> axis = Vector3<T>::Right;
			axis = axis.Cross(aFrom);
			if (axis == Vector3<T>::Zero)
			{
				axis = Vector3<T>::Up;
				axis = axis.Cross(aFrom);
			}

			return Quaternion(T(0), axis.x, axis.y, axis.z).GetNormalized();
		}

		const T s			= std::sqrt((T(1) + d) * T(2));
		const T invs		= T(1) / s;
		const Vector3<T> c	= aFrom.Cross(aTo) * invs;

		return Quaternion(s * T(0.5), c.x, c.y, c.z).GetNormalized();
	}

	template<typename T>
	constexpr Quaternion<T> Quaternion<T>::Difference(Quaternion aQuatA, Quaternion aQuatB)
	{
		return aQuatB * aQuatA.GetInverse();
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

	template<typename T>
	NODISC constexpr Vector3<T> operator*(const Quaternion<T>& aLeft, const Vector3<T>& aRight)
	{
		return Quaternion<T>::RotateVectorByQuaternion(aLeft, aRight);
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
	NODISC constexpr bool operator==(const Quaternion<T>& aLeft, const Quaternion<T>& aRight)
	{
		return (aLeft.x == aRight.x && aLeft.y == aRight.y && aLeft.z == aRight.z && aLeft.w == aRight.w);
	}
	template<typename T>
	NODISC constexpr bool operator!=(const Quaternion<T>& aLeft, const Quaternion<T>& aRight)
	{
		return !(aLeft == aRight);
	}

	template<typename T>
	inline const Quaternion<T> Quaternion<T>::IDENTITY;

	using Quatf = Quaternion<float>;
	using Quatd = Quaternion<double>;
}
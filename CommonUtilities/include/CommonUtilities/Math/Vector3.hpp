#pragma once

#include <cmath>
#include <cassert>
#include <bit>

#include <CommonUtilities/Utility/ArithmeticUtils.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T>
	class Vector2;

	template<typename T>
	class Vector4;

	template<typename T>
	class Vector3
	{
	public:
		T x{};
		T y{};
		T z{};

		CONSTEXPR Vector3() = default;
		CONSTEXPR ~Vector3() = default;

		CONSTEXPR Vector3(T aX, T aY, T aZ);

		template <typename U>
		NODISC CONSTEXPR explicit Vector3(const Vector3<U>& aVector);

		template<class OtherVector>
		NODISC CONSTEXPR explicit operator OtherVector() const;

		/// \returns Directional vector pointing from current to target.
		/// 
		NODISC CONSTEXPR static Vector3 Direction(const Vector3& aCurrent, const Vector3& aTarget);

		/// \returns Distance from current to target.
		/// 
		NODISC CONSTEXPR static Vector3 Distance(const Vector3& aCurrent, const Vector3& aTarget);

		/// \returns Distance squared from current to target.
		/// 
		NODISC CONSTEXPR static Vector3 DistanceSqr(const Vector3& aCurrent, const Vector3& aTarget);

		/// \returns Lerped vector between current and target.
		/// 
		NODISC CONSTEXPR static Vector3 Lerp(const Vector3& aCurrent, const Vector3& aTarget, float aPercentage);

		/// \returns Slerped vector between current and target.
		/// 
		NODISC CONSTEXPR static Vector3 Slerp(const Vector3& aCurrent, const Vector3& aTarget, float aPercentage);

		/// \returns Moved vector going from current towards target.
		/// 
		NODISC CONSTEXPR static Vector3 MoveTowards(const Vector3& aCurrent, const Vector3& aTarget, float aDistance);

		///	Length of the vector.
		/// 
		NODISC CONSTEXPR T Length() const;

		/// Square length of the vector, useful for comparisons
		/// 
		NODISC CONSTEXPR T LengthSqr() const;

		/// Computes a normalized vector.
		/// 
		/// \param Radius: Length of the normalized vector
		/// 
		/// \returns Normalized vector
		/// 
		NODISC CONSTEXPR Vector3<T> GetNormalized(T aRadius = static_cast<T>(1)) const;

		/// Computes a normalized vector.
		/// 
		/// \param Length: Optimization param for when length may already have been calculated
		/// \param Radius: Length of the normalized vector
		/// 
		/// \returns Normalized vector
		/// 
		NODISC CONSTEXPR Vector3<T> GetNormalized(T aLength, T aRadius) const;

		/// Normalizes this vector.
		/// 
		/// \param Radius: Length of the normalized vector
		/// 
		CONSTEXPR void Normalize(T aRadius = static_cast<T>(1));

		/// Dot product of two vectors.
		/// 
		NODISC CONSTEXPR T Dot(const Vector3& aVector) const;

		/// Cross product of two vectors.
		/// 
		/// \returns The perpendicular vector to the two vectors
		/// 
		NODISC CONSTEXPR Vector3 Cross(const Vector3& aVector) const;

		/// Projects this vector onto another.
		/// 
		/// \param Vector: vector to project onto
		/// 
		/// \returns Projected vector
		/// 
		NODISC CONSTEXPR Vector3 ProjectOnto(const Vector3& aVector) const;

		/// Reflects provided vector from this acting as a normal.
		/// 
		/// \param Vector: vector to reflect.
		/// 
		/// \returns Reflected vector
		/// 
		NODISC CONSTEXPR Vector3 Reflect(const Vector3& aVector) const;

		/// Computes the shortest angle between the two vectors.
		/// 
		/// \param Vector: vector to compare to.
		/// 
		/// \returns Angle in the range [-PI, PI) radians
		/// 
		NODISC CONSTEXPR T AngleTo(const Vector3& aVector) const;

		/// \returns Converts this 3D vector to a 2D one.
		/// 
		NODISC CONSTEXPR Vector2<T> XY() const;

		/// \returns Converts this 3D vector to a 2D one.
		/// 
		NODISC CONSTEXPR Vector2<T> XZ() const;

		/// \returns Converts this 3D vector to a 2D one.
		/// 
		NODISC CONSTEXPR Vector2<T> YZ() const;

		/// \returns Converts this 3D vector to a 4D one.
		/// 
		NODISC CONSTEXPR Vector4<T> XYZW(T aW = T{}) const;

		static const Vector3 Zero;
		static const Vector3 Up;
		static const Vector3 Down;
		static const Vector3 Right;
		static const Vector3 Left;
		static const Vector3 Forward;
		static const Vector3 Back;
	};

	template <typename T>
	const Vector3<T> Vector3<T>::Zero(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0));

	template <typename T>
	const Vector3<T> Vector3<T>::Up(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));

	template <typename T>
	const Vector3<T> Vector3<T>::Down(static_cast<T>(0), static_cast<T>(-1), static_cast<T>(0));

	template <typename T>
	const Vector3<T> Vector3<T>::Right(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0));

	template <typename T>
	const Vector3<T> Vector3<T>::Left(static_cast<T>(-1), static_cast<T>(0), static_cast<T>(0));

	template <typename T>
	const Vector3<T> Vector3<T>::Forward(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));

	template <typename T>
	const Vector3<T> Vector3<T>::Back(static_cast<T>(0), static_cast<T>(0), static_cast<T>(-1));

	template<typename T>
	CONSTEXPR Vector3<T>::Vector3(T aX, T aY, T aZ)
		: x(aX), y(aY), z(aZ) {}

	template<typename T>
	template<typename U>
	CONSTEXPR Vector3<T>::Vector3(const Vector3<U>& aVector)
		: x(static_cast<T>(aVector.x)), y(static_cast<T>(aVector.y)), z(static_cast<T>(aVector.z)) {}

	template<typename T>
	template<class OtherVector>
	CONSTEXPR Vector3<T>::operator OtherVector() const
	{
		return OtherVector{ x, y, z };
	}

	template<typename T>
	CONSTEXPR Vector3<T> Vector3<T>::Direction(const Vector3& aCurrent, const Vector3& aTarget)
	{
		return Vector3<T>(aTarget.x - aCurrent.x, aTarget.y - aCurrent.y, aTarget.z - aCurrent.z);
	}

	template<typename T>
	CONSTEXPR Vector3<T> Vector3<T>::Distance(const Vector3& aCurrent, const Vector3& aTarget)
	{
		return Direction(aCurrent, aTarget).Length();
	}

	template<typename T>
	CONSTEXPR Vector3<T> Vector3<T>::DistanceSqr(const Vector3& aCurrent, const Vector3& aTarget)
	{
		return Direction(aCurrent, aTarget).LengthSqr();
	}

	template<typename T>
	CONSTEXPR Vector3<T> Vector3<T>::Lerp(const Vector3& aCurrent, const Vector3& aTarget, float aPercentage)
	{
		const auto LerpFloat = [aPercentage](float aStart, float aEnd) { return aStart + aPercentage * (aEnd - aStart); };

		return Vector3<T>
		{
			static_cast<T>(LerpFloat(static_cast<float>(aCurrent.x), static_cast<float>(aTarget.x))),
			static_cast<T>(LerpFloat(static_cast<float>(aCurrent.y), static_cast<float>(aTarget.y))),
			static_cast<T>(LerpFloat(static_cast<float>(aCurrent.z), static_cast<float>(aTarget.z))),
		};
	}

	template<typename T>
	CONSTEXPR Vector3<T> Vector3<T>::Slerp(const Vector3& aCurrent, const Vector3& aTarget, float aPercentage)
	{
		const auto ClampFloat = [](float aValue, float aMin, float aMax) { return (aValue < aMin) ? aMin : ((aValue > aMax) ? aMax : aValue); };

		const float dot = ClampFloat(static_cast<float>(aCurrent.Dot(aTarget)), -1.0f, 1.0f);

		const Vector3<T> relativeVec = (aTarget - aCurrent * dot).GetNormalized();
		const float theta = std::acos(dot) * aPercentage;

		return aCurrent * std::cos(theta) + relativeVec * std::sin(theta);
	}

	template<typename T>
	CONSTEXPR Vector3<T> Vector3<T>::MoveTowards(const Vector3& aCurrent, const Vector3& aTarget, float aDistance)
	{
		return aCurrent + Vector3<T>::Direction(aCurrent, aTarget).GetNormalized(aDistance);
	}

	template<typename T>
	CONSTEXPR T Vector3<T>::Length() const
	{
		return static_cast<T>(std::sqrt(LengthSqr()));
	}

	template<typename T>
	CONSTEXPR T Vector3<T>::LengthSqr() const
	{
		return Dot(*this);
	}

	template<typename T>
	CONSTEXPR Vector3<T> Vector3<T>::GetNormalized(T aRadius) const
	{
		return GetNormalized(Length(), aRadius);
	}
	template<typename T>
	CONSTEXPR Vector3<T> Vector3<T>::GetNormalized(T aLength, T aRadius) const
	{
		assert(aLength > T{} && "Negative or zero length is an error");
		return (*this) * (aRadius / aLength);
	}

	template<typename T>
	CONSTEXPR void Vector3<T>::Normalize(T aRadius)
	{
		*this = GetNormalized(aRadius);
	}

	template<typename T>
	CONSTEXPR T Vector3<T>::Dot(const Vector3& aVector) const
	{
		return x * aVector.x + y * aVector.y + z * aVector.z;
	}

	template<typename T>
	CONSTEXPR Vector3<T> Vector3<T>::Cross(const Vector3& aVector) const
	{
		return Vector3<T>(
			y * aVector.z - z * aVector.y,
			z * aVector.x - x * aVector.z,
			x * aVector.y - y * aVector.x);
	}

	template<typename T>
	CONSTEXPR Vector3<T> Vector3<T>::ProjectOnto(const Vector3& aVector) const
	{
		assert(aVector != Vector3<T>() && "Cannot project onto a zero vector");
		return (Dot(aVector) / aVector.LengthSqr()) * aVector; // div by lengthsqr
	}

	template<typename T>
	CONSTEXPR Vector3<T> Vector3<T>::Reflect(const Vector3& aVector) const
	{
		return aVector - T{2} * ProjectOnto(aVector);
	}

	template<typename T>
	CONSTEXPR T Vector3<T>::AngleTo(const Vector3& aVector) const
	{
		return std::acos(Dot(aVector) / (Length() * aVector.Length()));
	}

	template<typename T>
	CONSTEXPR Vector2<T> Vector3<T>::XY() const
	{
		return Vector2<T>(x, y);
	}
	template<typename T>
	CONSTEXPR Vector2<T> Vector3<T>::XZ() const
	{
		return Vector2<T>(x, z);
	}
	template<typename T>
	CONSTEXPR Vector2<T> Vector3<T>::YZ() const
	{
		return Vector2<T>(y, z);
	}
	template<typename T>
	CONSTEXPR Vector4<T> Vector3<T>::XYZW(T aW) const
	{
		return Vector4<T>(x, y, z, aW);
	}

	// GLOBAL OPERATORS

	template<typename T>
	NODISC CONSTEXPR Vector3<T> operator-(const Vector3<T>& aVector)
	{
		return Vector3<T>(-aVector.x, -aVector.y, -aVector.z);
	}

	template<typename T>
	CONSTEXPR Vector3<T>& operator+=(Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		aLeft.x += aRight.x;
		aLeft.y += aRight.y;
		aLeft.z += aRight.z;

		return aLeft;
	}
	template<typename T>
	CONSTEXPR Vector3<T>& operator-=(Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		aLeft.x -= aRight.x;
		aLeft.y -= aRight.y;
		aLeft.z -= aRight.z;

		return aLeft;
	}

	template<typename T>
	CONSTEXPR Vector3<T>& operator*=(Vector3<T>& aLeft, T aRight)
	{
		aLeft.x *= aRight;
		aLeft.y *= aRight;
		aLeft.z *= aRight;

		return aLeft;
	}
	template<typename T>
	CONSTEXPR Vector3<T>& operator/=(Vector3<T>& aLeft, T aRight)
	{
		assert(aRight != 0 && "Cannot divide by zero");

		aLeft.x /= aRight;
		aLeft.y /= aRight;
		aLeft.z /= aRight;

		return aLeft;
	}

	template<typename T>
	CONSTEXPR Vector3<T>& operator*=(Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		aLeft.x *= aRight.x;
		aLeft.y *= aRight.y;
		aLeft.z *= aRight.z;

		return aLeft;
	}
	template<typename T>
	CONSTEXPR Vector3<T>& operator/=(Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && aRight.z != 0 && "Cannot divide by zero");

		aLeft.x /= aRight.x;
		aLeft.y /= aRight.y;
		aLeft.z /= aRight.z;

		return aLeft;
	}

	template<typename T>
	NODISC CONSTEXPR Vector3<T> operator+(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		return Vector3<T>(aLeft.x + aRight.x, aLeft.y + aRight.y, aLeft.z + aRight.z);
	}
	template<typename T>
	NODISC CONSTEXPR Vector3<T> operator-(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		return Vector3<T>(aLeft.x - aRight.x, aLeft.y - aRight.y, aLeft.z - aRight.z);
	}

	template<typename T>
	NODISC CONSTEXPR Vector3<T> operator*(const Vector3<T>& aLeft, T aRight)
	{
		return Vector3<T>(aLeft.x * aRight, aLeft.y * aRight, aLeft.z * aRight);
	}
	template<typename T>
	NODISC CONSTEXPR Vector3<T> operator/(const Vector3<T>& aLeft, T aRight)
	{
		assert(aRight != 0 && "Cannot divide by zero");
		return Vector3<T>(aLeft.x / aRight, aLeft.y / aRight, aLeft.z / aRight);
	}

	template<typename T>
	NODISC CONSTEXPR Vector3<T> operator*(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		return Vector3<T>(aLeft.x * aRight.x, aLeft.y * aRight.y, aLeft.z * aRight.z);
	}
	template<typename T>
	NODISC CONSTEXPR Vector3<T> operator*(T aLeft, const Vector3<T>& aRight)
	{
		return aRight * aLeft;
	}

	template<typename T>
	NODISC CONSTEXPR Vector3<T> operator/(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && aRight.z != 0 && "Cannot divide by zero");
		return Vector3<T>(aLeft.x / aRight.x, aLeft.y / aRight.y, aLeft.z / aRight.z);
	}
	template<typename T>
	NODISC CONSTEXPR Vector3<T> operator/(T aLeft, const Vector3<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && aRight.z != 0 && "Cannot divide by zero");
		return Vector3<T>(aLeft / aRight.x, aLeft / aRight.y, aLeft / aRight.z);
	}

	template<typename T>
	NODISC CONSTEXPR bool operator==(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		return (aLeft.x == aRight.x) && (aLeft.y == aRight.y) && (aLeft.z == aRight.z);
	}
	template<typename T>
	NODISC CONSTEXPR bool operator!=(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		return !(aLeft == aRight);
	}

	// using declarations

	using Vector3f = Vector3<float>;
	using Vector3d = Vector3<double>;
	using Vector3i = Vector3<int>;
	using Vector3u = Vector3<unsigned int>;
}
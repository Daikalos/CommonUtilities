#pragma once

#include <cmath>
#include <cassert>

namespace CommonUtilities
{
	template<typename T>
	class Vector3
	{
	public:
		T x;
		T y;
		T z;

		constexpr Vector3();
		constexpr ~Vector3();

		constexpr Vector3(T aX, T aY, T aZ);

		template <typename U>
		[[nodiscard]] constexpr explicit Vector3(const Vector3<U>& aVector);

		template<class OtherVector>
		[[nodiscard]] constexpr explicit operator OtherVector() const;

		/// \returns Directional vector pointing between aFrom and aTo.
		/// 
		[[nodiscard]] constexpr static Vector3 Direction(const Vector3& aFrom, const Vector3& aTo);

		///	Length of the vector.
		/// 
		[[nodiscard]] constexpr T Length() const;

		/// Square length of the vector, useful for comparisons
		/// 
		[[nodiscard]] constexpr T LengthSqr() const;

		/// Computes a normalized vector.
		/// 
		/// \param Radius: Length of the normalized vector
		/// 
		/// \returns Normalized vector
		/// 
		[[nodiscard]] constexpr Vector3<T> GetNormalized(T aRadius = static_cast<T>(1)) const;

		/// Computes a normalized vector.
		/// 
		/// \param Length: Optimization param for when length may already have been calculated
		/// \param Radius: Length of the normalized vector
		/// 
		/// \returns Normalized vector
		/// 
		[[nodiscard]] constexpr Vector3<T> GetNormalized(T aLength, T aRadius) const;

		/// Normalizes this vector.
		/// 
		/// \param Radius: Length of the normalized vector
		/// 
		constexpr void Normalize(T aRadius = static_cast<T>(1));

		/// Dot product of two vectors.
		/// 
		[[nodiscard]] constexpr T Dot(const Vector3& aVector) const;

		/// Cross product of two vectors.
		/// 
		/// \returns The perpendicular vector to the two vectors
		/// 
		[[nodiscard]] constexpr Vector3 Cross(const Vector3& aVector) const;

		/// Projects this vector onto another.
		/// 
		/// \param Vector: vector to project onto
		/// 
		/// \returns Projected vector
		/// 
		[[nodiscard]] constexpr Vector3 ProjectOnto(const Vector3& aVector) const;
	};

	template<typename T>
	constexpr Vector3<T>::Vector3()
		: x(), y(), z() {}

	template<typename T>
	constexpr Vector3<T>::~Vector3() = default;

	template<typename T>
	constexpr Vector3<T>::Vector3(T aX, T aY, T aZ)
		: x(aX), y(aY), z(aZ) {}

	template<typename T>
	template<typename U>
	constexpr Vector3<T>::Vector3(const Vector3<U>& aVector)
		: x(static_cast<T>(aVector.x)), y(static_cast<T>(aVector.y)), z(static_cast<T>(aVector.z)) {}

	template<typename T>
	template<class OtherVector>
	constexpr Vector3<T>::operator OtherVector() const
	{
		return OtherVector{ x, y, z };
	}

	template<typename T>
	constexpr Vector3<T> Vector3<T>::Direction(const Vector3& aFrom, const Vector3& aTo)
	{
		return Vector3<T>(aTo.x - aFrom.x, aTo.y - aFrom.y, aTo.z - aFrom.z);
	}

	template<typename T>
	constexpr T Vector3<T>::Length() const
	{
		return static_cast<T>(std::sqrt(LengthSqr()));
	}

	template<typename T>
	constexpr T Vector3<T>::LengthSqr() const
	{
		return Dot(*this);
	}

	template<typename T>
	constexpr Vector3<T> Vector3<T>::GetNormalized(T aRadius) const
	{
		return GetNormalized(Length(), aRadius);
	}
	template<typename T>
	constexpr Vector3<T> Vector3<T>::GetNormalized(T aLength, T aRadius) const
	{
		assert(aLength > T{} && "Negative or zero length is an error");
		return (*this) * (aRadius / aLength);
	}

	template<typename T>
	constexpr void Vector3<T>::Normalize(T aRadius)
	{
		*this = GetNormalized(aRadius);
	}

	template<typename T>
	constexpr T Vector3<T>::Dot(const Vector3& aVector) const
	{
		return x * aVector.x + y * aVector.y + z * aVector.z;
	}

	template<typename T>
	constexpr Vector3<T> Vector3<T>::Cross(const Vector3& aVector) const
	{
		return Vector3<T>(
			y * aVector.z - z * aVector.y,
			z * aVector.x - x * aVector.z,
			x * aVector.y - y * aVector.x);
	}

	template<typename T>
	constexpr Vector3<T> Vector3<T>::ProjectOnto(const Vector3& aVector) const
	{
		assert(aVector != Vector3<T>() && "Cannot project onto a zero vector");
		return (Dot(aVector) / aVector.LengthSqr()) * aVector; // div by lengthsqr
	}

	// GLOBAL OPERATORS

	template<typename T>
	constexpr Vector3<T> operator-(const Vector3<T>& aVector)
	{
		return Vector3<T>(-aVector.x, -aVector.y, -aVector.z);
	}

	template<typename T>
	constexpr Vector3<T>& operator+=(Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		aLeft.x += aRight.x;
		aLeft.y += aRight.y;
		aLeft.z += aRight.z;

		return aLeft;
	}
	template<typename T>
	constexpr Vector3<T>& operator-=(Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		aLeft.x -= aRight.x;
		aLeft.y -= aRight.y;
		aLeft.z -= aRight.z;

		return aLeft;
	}

	template<typename T>
	constexpr Vector3<T>& operator*=(Vector3<T>& aLeft, T aRight)
	{
		aLeft.x *= aRight;
		aLeft.y *= aRight;
		aLeft.z *= aRight;

		return aLeft;
	}
	template<typename T>
	constexpr Vector3<T>& operator/=(Vector3<T>& aLeft, T aRight)
	{
		assert(aRight != 0 && "Cannot divide by zero");

		aLeft.x /= aRight;
		aLeft.y /= aRight;
		aLeft.z /= aRight;

		return aLeft;
	}

	template<typename T>
	constexpr Vector3<T>& operator*=(Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		aLeft.x *= aRight.x;
		aLeft.y *= aRight.y;
		aLeft.z *= aRight.z;

		return aLeft;
	}
	template<typename T>
	constexpr Vector3<T>& operator/=(Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && aRight.z != 0 && "Cannot divide by zero");

		aLeft.x /= aRight.x;
		aLeft.y /= aRight.y;
		aLeft.z /= aRight.z;

		return aLeft;
	}

	template<typename T>
	constexpr Vector3<T> operator+(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		return Vector3<T>(aLeft.x + aRight.x, aLeft.y + aRight.y, aLeft.z + aRight.z);
	}
	template<typename T>
	constexpr Vector3<T> operator-(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		return Vector3<T>(aLeft.x - aRight.x, aLeft.y - aRight.y, aLeft.z - aRight.z);
	}

	template<typename T>
	constexpr Vector3<T> operator*(const Vector3<T>& aLeft, T aRight)
	{
		return Vector3<T>(aLeft.x * aRight, aLeft.y * aRight, aLeft.z * aRight);
	}
	template<typename T>
	constexpr Vector3<T> operator/(const Vector3<T>& aLeft, T aRight)
	{
		assert(aRight != 0 && "Cannot divide by zero");
		return Vector3<T>(aLeft.x / aRight, aLeft.y / aRight, aLeft.z / aRight);
	}

	template<typename T>
	constexpr Vector3<T> operator*(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		return Vector3<T>(aLeft.x * aRight.x, aLeft.y * aRight.y, aLeft.z * aRight.z);
	}
	template<typename T>
	constexpr Vector3<T> operator*(T aLeft, const Vector3<T>& aRight)
	{
		return aRight * aLeft;
	}

	template<typename T>
	constexpr Vector3<T> operator/(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && aRight.z != 0 && "Cannot divide by zero");
		return Vector3<T>(aLeft.x / aRight.x, aLeft.y / aRight.y, aLeft.z / aRight.z);
	}
	template<typename T>
	constexpr Vector3<T> operator/(T aLeft, const Vector3<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && aRight.z != 0 && "Cannot divide by zero");
		return Vector3<T>(aLeft / aRight.x, aLeft / aRight.y, aLeft / aRight.z);
	}

	template<typename T>
	constexpr bool operator==(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		return (aLeft.x == aRight.x) && (aLeft.y == aRight.y) && (aLeft.z == aRight.z);
	}
	template<typename T>
	constexpr bool operator!=(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		return !(aLeft == aRight);
	}

	// using declarations

	using Vector3f = Vector3<float>;
	using Vector3d = Vector3<double>;
	using Vector3i = Vector3<int>;
	using Vector3u = Vector3<unsigned int>;
}
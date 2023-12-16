#pragma once

#include <cmath>
#include <cassert>

namespace CommonUtilities
{
	template<typename T>
	class Vector4
	{
	public:
		T x;
		T y;
		T z;
		T w;

		constexpr Vector4();
		constexpr ~Vector4();

		constexpr Vector4(T aX, T aY, T aZ, T aW);

		template <typename U>
		[[nodiscard]] constexpr explicit Vector4(const Vector4<U>& aVector);

		template<class OtherVector>
		[[nodiscard]] constexpr explicit operator OtherVector() const;

		/// \returns Directional vector pointing between aFrom and aTo.
		/// 
		[[nodiscard]] constexpr static Vector4 Direction(const Vector4& aFrom, const Vector4& aTo);

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
		[[nodiscard]] constexpr Vector4<T> GetNormalized(T aRadius = static_cast<T>(1)) const;

		/// Computes a normalized vector.
		/// 
		/// \param Length: Optimization param for when length may already have been calculated
		/// \param Radius: Length of the normalized vector
		/// 
		/// \returns Normalized vector
		/// 
		[[nodiscard]] constexpr Vector4<T> GetNormalized(T aLength, T aRadius) const;

		/// Normalizes this vector
		/// 
		/// \param Radius: Length of the normalized vector
		/// 
		[[nodiscard]] constexpr void Normalize(T aRadius = static_cast<T>(1));

		/// Dot product of two vectors.
		/// 
		[[nodiscard]] constexpr T Dot(const Vector4& aVector) const;
	};

	template<typename T>
	constexpr Vector4<T>::Vector4()
		: x(), y(), z(), w() {}

	template<typename T>
	constexpr Vector4<T>::~Vector4() = default;

	template<typename T>
	constexpr Vector4<T>::Vector4(T aX, T aY, T aZ, T aW)
		: x(aX), y(aY), z(aZ), w(aW) {}

	template<typename T>
	template<typename U>
	constexpr Vector4<T>::Vector4(const Vector4<U>& aVector)
		: x(static_cast<T>(aVector.x)), y(static_cast<T>(aVector.y)), z(static_cast<T>(aVector.z)), w(static_cast<T>(aVector.w)) {}

	template<typename T>
	template<class OtherVector>
	constexpr Vector4<T>::operator OtherVector() const
	{
		return OtherVector{ x, y, z, w };
	}

	template<typename T>
	constexpr Vector4<T> Vector4<T>::Direction(const Vector4& aFrom, const Vector4& aTo)
	{
		return Vector4<T>(aTo.x - aFrom.x, aTo.y - aFrom.y, aTo.z - aFrom.z, aTo.w - aFrom.w);
	}

	template<typename T>
	constexpr T Vector4<T>::Length() const
	{
		return static_cast<T>(std::sqrt(LengthSqr()));
	}

	template<typename T>
	constexpr T Vector4<T>::LengthSqr() const
	{
		return Dot(*this);
	}

	template<typename T>
	constexpr Vector4<T> Vector4<T>::GetNormalized(T aRadius) const
	{
		return GetNormalized(Length(), aRadius);
	}
	template<typename T>
	constexpr Vector4<T> Vector4<T>::GetNormalized(T aLength, T aRadius) const
	{
		assert(aLength > T{} && "Negative or zero length is an error");
		return (*this) * (aRadius / aLength);
	}

	template<typename T>
	constexpr void Vector4<T>::Normalize(T aRadius)
	{
		*this = GetNormalized(aRadius);
	}

	template<typename T>
	constexpr T Vector4<T>::Dot(const Vector4& aVector) const
	{
		return x * aVector.x + y * aVector.y + z * aVector.z + w * aVector.w;
	}

	// GLOBAL OPERATORS

	template<typename T>
	constexpr Vector4<T> operator-(const Vector4<T>& aVector)
	{
		return Vector4<T>(-aVector.x, -aVector.y, -aVector.z, -aVector.w);
	}

	template<typename T>
	constexpr Vector4<T>& operator+=(Vector4<T>& aLeft, const Vector4<T>& aRight)
	{
		aLeft.x += aRight.x;
		aLeft.y += aRight.y;
		aLeft.z += aRight.z;
		aLeft.w += aRight.w;

		return aLeft;
	}
	template<typename T>
	constexpr Vector4<T>& operator-=(Vector4<T>& aLeft, const Vector4<T>& aRight)
	{
		aLeft.x -= aRight.x;
		aLeft.y -= aRight.y;
		aLeft.z -= aRight.z;
		aLeft.w -= aRight.w;

		return aLeft;
	}

	template<typename T>
	constexpr Vector4<T>& operator*=(Vector4<T>& aLeft, T aRight)
	{
		aLeft.x *= aRight;
		aLeft.y *= aRight;
		aLeft.z *= aRight;
		aLeft.w *= aRight;

		return aLeft;
	}
	template<typename T>
	constexpr Vector4<T>& operator/=(Vector4<T>& aLeft, T aRight)
	{
		assert(aRight != 0 && "Cannot divide by zero");

		aLeft.x /= aRight;
		aLeft.y /= aRight;
		aLeft.z /= aRight;
		aLeft.w /= aRight;

		return aLeft;
	}

	template<typename T>
	constexpr Vector4<T>& operator*=(Vector4<T>& aLeft, const Vector4<T>& aRight)
	{
		aLeft.x *= aRight.x;
		aLeft.y *= aRight.y;
		aLeft.z *= aRight.z;
		aLeft.w *= aRight.w;

		return aLeft;
	}
	template<typename T>
	constexpr Vector4<T>& operator/=(Vector4<T>& aLeft, const Vector4<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && aRight.z != 0 && aRight.w != 0 && "Cannot divide by zero");

		aLeft.x /= aRight.x;
		aLeft.y /= aRight.y;
		aLeft.z /= aRight.z;
		aLeft.w /= aRight.w;

		return aLeft;
	}

	template<typename T>
	constexpr Vector4<T> operator+(const Vector4<T>& aLeft, const Vector4<T>& aRight)
	{
		return Vector4<T>(aLeft.x + aRight.x, aLeft.y + aRight.y, aLeft.z + aRight.z, aLeft.w + aRight.w);
	}
	template<typename T>
	constexpr Vector4<T> operator-(const Vector4<T>& aLeft, const Vector4<T>& aRight)
	{
		return Vector4<T>(aLeft.x - aRight.x, aLeft.y - aRight.y, aLeft.z - aRight.z, aLeft.w - aRight.w);
	}

	template<typename T>
	constexpr Vector4<T> operator*(const Vector4<T>& aLeft, T aRight)
	{
		return Vector4<T>(aLeft.x * aRight, aLeft.y * aRight, aLeft.z * aRight, aLeft.w * aRight);
	}
	template<typename T>
	constexpr Vector4<T> operator/(const Vector4<T>& aLeft, T aRight)
	{
		assert(aRight != 0 && "Cannot divide by zero");
		return Vector4<T>(aLeft.x / aRight, aLeft.y / aRight, aLeft.z / aRight, aLeft.w / aRight);
	}

	template<typename T>
	constexpr Vector4<T> operator*(const Vector4<T>& aLeft, const Vector4<T>& aRight)
	{
		return Vector4<T>(aLeft.x * aRight.x, aLeft.y * aRight.y, aLeft.z * aRight.z, aLeft.w * aRight.w);
	}
	template<typename T>
	constexpr Vector4<T> operator*(T aLeft, const Vector4<T>& aRight)
	{
		return aRight * aLeft;
	}

	template<typename T>
	constexpr Vector4<T> operator/(const Vector4<T>& aLeft, const Vector4<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && aRight.z != 0 && aRight.w != 0 && "Cannot divide by zero");
		return Vector4<T>(aLeft.x / aRight.x, aLeft.y / aRight.y, aLeft.z / aRight.z, aLeft.w / aRight.w);
	}
	template<typename T>
	constexpr Vector4<T> operator/(T aLeft, const Vector4<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && aRight.z != 0 && aRight.w != 0 && "Cannot divide by zero");
		return Vector4<T>(aLeft / aRight.x, aLeft / aRight.y, aLeft / aRight.z, aLeft / aRight.w);
	}

	template<typename T>
	constexpr bool operator==(const Vector4<T>& aLeft, const Vector4<T>& aRight)
	{
		return (aLeft.x == aRight.x) && (aLeft.y == aRight.y) && (aLeft.z == aRight.z) && (aLeft.w == aRight.w);
	}
	template<typename T>
	constexpr bool operator!=(const Vector4<T>& aLeft, const Vector4<T>& aRight)
	{
		return !(aLeft == aRight);
	}

	// using declarations

	using Vector4f = Vector4<float>;
	using Vector4d = Vector4<double>;
	using Vector4i = Vector4<int>;
	using Vector4u = Vector4<unsigned int>;
}
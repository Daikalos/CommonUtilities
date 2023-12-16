#pragma once

#include <cmath>
#include <cassert>

namespace CommonUtilities
{
	template<typename T>
	class Vector2
	{
	public:
		T x;
		T y;

		constexpr Vector2();
		constexpr ~Vector2();

		constexpr Vector2(T aX, T aY);

		template <typename U>
		constexpr explicit Vector2(const Vector2<U>& aVector);

		template<class OtherVector>
		[[nodiscard]] constexpr explicit operator OtherVector() const;

		/// \returns Directional vector pointing between aFrom and aTo.
		/// 
		[[nodiscard]] constexpr static Vector2 Direction(const Vector2& aFrom, const Vector2& aTo);

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
		[[nodiscard]] constexpr Vector2<T> GetNormalized(T aRadius = static_cast<T>(1)) const;

		/// Computes a normalized vector.
		/// 
		/// \param Length: Optimization param for when length may already have been calculated
		/// \param Radius: Length of the normalized vector
		/// 
		/// \returns Normalized vector
		/// 
		[[nodiscard]] constexpr Vector2<T> GetNormalized(T aLength, T aRadius) const;

		/// Normalizes this vector.
		/// 
		/// \param Radius: Length of the normalized vector
		/// 
		constexpr void Normalize(T aRadius = static_cast<T>(1));

		/// Dot product of two vectors.
		/// 
		[[nodiscard]] constexpr T Dot(const Vector2& aVector) const;

		/// Projects this vector onto another.
		/// 
		/// \param Vector: vector to project onto
		/// 
		/// \returns Projected vector
		/// 
		[[nodiscard]] constexpr Vector2 ProjectOnto(const Vector2& aVector) const;
	};

	template<typename T>
	constexpr Vector2<T>::Vector2()
		: x(), y() {}

	template<typename T>
	constexpr Vector2<T>::~Vector2() = default;

	template<typename T>
	constexpr Vector2<T>::Vector2(T aX, T aY)
		: x(aX), y(aY) {}

	template<typename T>
	template<typename U>
	constexpr Vector2<T>::Vector2(const Vector2<U>& aVector)
		: x(static_cast<T>(aVector.x)), y(static_cast<T>(aVector.y)) {}

	template<typename T>
	template<class OtherVector>
	constexpr Vector2<T>::operator OtherVector() const
	{
		return OtherVector{ x, y };
	}

	template<typename T>
	constexpr Vector2<T> Vector2<T>::Direction(const Vector2& aFrom, const Vector2& aTo)
	{
		return Vector2<T>(aTo.x - aFrom.x, aTo.y - aFrom.y);
	}

	template<typename T>
	constexpr T Vector2<T>::Length() const
	{
		return static_cast<T>(std::sqrt(LengthSqr()));
	}

	template<typename T>
	constexpr T Vector2<T>::LengthSqr() const
	{
		return Dot(*this);
	}

	template<typename T>
	constexpr Vector2<T> Vector2<T>::GetNormalized(T aRadius) const
	{
		return GetNormalized(Length(), aRadius);
	}
	template<typename T>
	constexpr Vector2<T> Vector2<T>::GetNormalized(T aLength, T aRadius) const
	{
		assert(aLength > T{} && "Negative or zero length is an error");
		return (*this) * (aRadius / aLength);
	}

	template<typename T>
	constexpr void Vector2<T>::Normalize(T aRadius)
	{
		*this = GetNormalized(aRadius);
	}

	template<typename T>
	constexpr T Vector2<T>::Dot(const Vector2& aVector) const
	{
		return x * aVector.x + y * aVector.y;
	}

	template<typename T>
	constexpr Vector2<T> Vector2<T>::ProjectOnto(const Vector2& aVector) const
	{
		assert(aVector != Vector2<T>() && "Cannot project onto a zero vector");
		return (Dot(aVector) / aVector.LengthSqr()) * aVector;
	}

	// GLOBAL OPERATORS

	template<typename T>
	constexpr Vector2<T> operator-(const Vector2<T>& aVector)
	{
		return Vector2<T>(-aVector.x, -aVector.y);
	}

	template<typename T>
	constexpr Vector2<T>& operator+=(Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		aLeft.x += aRight.x;
		aLeft.y += aRight.y;

		return aLeft;
	}
	template<typename T>
	constexpr Vector2<T>& operator-=(Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		aLeft.x -= aRight.x;
		aLeft.y -= aRight.y;

		return aLeft;
	}

	template<typename T>
	constexpr Vector2<T>& operator*=(Vector2<T>& aLeft, T aRight)
	{
		aLeft.x *= aRight;
		aLeft.y *= aRight;

		return aLeft;
	}
	template<typename T>
	constexpr Vector2<T>& operator/=(Vector2<T>& aLeft, T aRight)
	{
		assert(aRight != 0 && "Cannot divide by zero");

		aLeft.x /= aRight;
		aLeft.y /= aRight;

		return aLeft;
	}

	template<typename T>
	constexpr Vector2<T>& operator*=(Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		aLeft.x *= aRight.x;
		aLeft.y *= aRight.y;

		return aLeft;
	}
	template<typename T>
	constexpr Vector2<T>& operator/=(Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && "Cannot divide by zero");

		aLeft.x /= aRight.x;
		aLeft.y /= aRight.y;

		return aLeft;
	}

	template<typename T>
	constexpr Vector2<T> operator+(const Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		return Vector2<T>(aLeft.x + aRight.x, aLeft.y + aRight.y);
	}
	template<typename T>
	constexpr Vector2<T> operator-(const Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		return Vector2<T>(aLeft.x - aRight.x, aLeft.y - aRight.y);
	}

	template<typename T>
	constexpr Vector2<T> operator*(const Vector2<T>& aLeft, T aRight)
	{
		return Vector2<T>(aLeft.x * aRight, aLeft.y * aRight);
	}
	template<typename T>
	constexpr Vector2<T> operator/(const Vector2<T>& aLeft, T aRight)
	{
		assert(aRight != 0 && "Cannot divide by zero");
		return Vector2<T>(aLeft.x / aRight, aLeft.y / aRight);
	}

	template<typename T>
	constexpr Vector2<T> operator*(const Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		return Vector2<T>(aLeft.x * aRight.x, aLeft.y * aRight.y);
	}
	template<typename T>
	constexpr Vector2<T> operator*(T aLeft, const Vector2<T>& aRight)
	{
		return aRight * aLeft;
	}

	template<typename T>
	constexpr Vector2<T> operator/(const Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && "Cannot divide by zero");
		return Vector2<T>(aLeft.x / aRight.x, aLeft.y / aRight.y);
	}
	template<typename T>
	constexpr Vector2<T> operator/(T aLeft, const Vector2<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && "Cannot divide by zero");
		return Vector2<T>(aLeft / aRight.x, aLeft / aRight.y);
	}

	template<typename T>
	constexpr bool operator==(const Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		return (aLeft.x == aRight.x) && (aLeft.y == aRight.y);
	}
	template<typename T>
	constexpr bool operator!=(const Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		return !(aLeft == aRight);
	}

	// using declarations

	using Vector2f = Vector2<float>;
	using Vector2d = Vector2<double>;
	using Vector2i = Vector2<int>;
	using Vector2u = Vector2<unsigned int>;
}
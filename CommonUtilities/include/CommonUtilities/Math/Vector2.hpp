#pragma once

#include <cmath>
#include <cassert>
#include <bit>

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T>
	class Vector3;

	template<typename T>
	class Vector2
	{
	public:
		T x;
		T y;

		CONSTEXPR Vector2();
		CONSTEXPR ~Vector2();

		CONSTEXPR Vector2(T aX, T aY);

		template <typename U>
		CONSTEXPR explicit Vector2(const Vector2<U>& aVector);

		template<class OtherVector>
		NODISC CONSTEXPR explicit operator OtherVector() const;

		/// \returns Directional vector pointing from current to target.
		/// 
		NODISC CONSTEXPR static Vector2 Direction(const Vector2& aCurrent, const Vector2& aTarget);

		/// \returns Distance from current to target.
		/// 
		NODISC CONSTEXPR static Vector2 Distance(const Vector2& aCurrent, const Vector2& aTarget);

		/// \returns Distance squared from current to target.
		/// 
		NODISC CONSTEXPR static Vector2 DistanceSqr(const Vector2& aCurrent, const Vector2& aTarget);

		/// \returns Lerped vector between current and target.
		/// 
		NODISC CONSTEXPR static Vector2 Lerp(const Vector2& aCurrent, const Vector2& aTarget, float aPercentage);

		/// \returns Slerped vector between current and target.
		/// 
		NODISC CONSTEXPR static Vector2 Slerp(const Vector2& aCurrent, const Vector2& aTarget, float aPercentage);

		/// \returns Moved vector going from current towards target.
		/// 
		NODISC CONSTEXPR static Vector2 MoveTowards(const Vector2& aCurrent, const Vector2& aTarget, float aDistance);

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
		NODISC CONSTEXPR Vector2<T> GetNormalized(T aRadius = static_cast<T>(1)) const;

		/// Computes a normalized vector.
		/// 
		/// \param Length: Optimization param for when length may already have been calculated
		/// \param Radius: Length of the normalized vector
		/// 
		/// \returns Normalized vector
		/// 
		NODISC CONSTEXPR Vector2<T> GetNormalized(T aLength, T aRadius) const;

		/// Computes a normalized vector using the inverse square root optimization. Use this
		/// when performance matters more than accuracy.
		/// 
		/// \returns Normalized vector
		/// 
		NODISC CONSTEXPR Vector2<T> GetNormalizedFast() const;

		/// Normalizes this vector.
		/// 
		/// \param Radius: Length of the normalized vector
		/// 
		CONSTEXPR void Normalize(T aRadius = static_cast<T>(1));

		/// Normalizes this vector using the inverse square root optimization. Use this
		/// when performance matters more than accuracy.
		/// 
		CONSTEXPR void NormalizeFast();

		/// Dot product of two vectors.
		/// 
		NODISC CONSTEXPR T Dot(const Vector2& aVector) const;

		/// Projects this vector onto another.
		/// 
		/// \param Vector: vector to project onto
		/// 
		/// \returns Projected vector
		/// 
		NODISC CONSTEXPR Vector2 ProjectOnto(const Vector2& aVector) const;

		/// Reflects provided vector from this acting as a normal.
		/// 
		/// \param Vector: vector to reflect.
		/// 
		/// \returns Reflected vector
		/// 
		NODISC CONSTEXPR Vector2 Reflect(const Vector2& aVector) const;

		/// \returns Converts this 2D vector to a 3D one.
		/// 
		NODISC CONSTEXPR Vector3<T> XYZ(T aZ = T{}) const;

		/// \returns Converts this 2D vector to a 3D one.
		/// 
		NODISC CONSTEXPR Vector3<T> XZY(T aY = T{}) const;

		/// \returns Converts this 2D vector to a 3D one.
		/// 
		NODISC CONSTEXPR Vector3<T> YZX(T aX = T{}) const;
	};

	template<typename T>
	CONSTEXPR Vector2<T>::Vector2()
		: x(), y() {}

	template<typename T>
	CONSTEXPR Vector2<T>::~Vector2() = default;

	template<typename T>
	CONSTEXPR Vector2<T>::Vector2(T aX, T aY)
		: x(aX), y(aY) {}

	template<typename T>
	template<typename U>
	CONSTEXPR Vector2<T>::Vector2(const Vector2<U>& aVector)
		: x(static_cast<T>(aVector.x)), y(static_cast<T>(aVector.y)) {}

	template<typename T>
	template<class OtherVector>
	CONSTEXPR Vector2<T>::operator OtherVector() const
	{
		return OtherVector{ x, y };
	}

	template<typename T>
	CONSTEXPR Vector2<T> Vector2<T>::Direction(const Vector2& aCurrent, const Vector2& aTarget)
	{
		return Vector2<T>(aTarget.x - aCurrent.x, aTarget.y - aCurrent.y);
	}

	template<typename T>
	CONSTEXPR Vector2<T> Vector2<T>::Distance(const Vector2& aCurrent, const Vector2& aTarget)
	{
		return Direction(aCurrent, aTarget).Length();
	}

	template<typename T>
	CONSTEXPR Vector2<T> Vector2<T>::DistanceSqr(const Vector2& aCurrent, const Vector2& aTarget)
	{
		return Direction(aCurrent, aTarget).LengthSqr();
	}

	template<typename T>
	CONSTEXPR Vector2<T> Vector2<T>::Lerp(const Vector2& aCurrent, const Vector2& aTarget, float aPercentage)
	{
		const auto LerpFloat = [aPercentage](float aStart, float aEnd) { return aStart + aPercentage * (aEnd - aStart); };

		return Vector2<T>
		{
			static_cast<T>(LerpFloat(static_cast<float>(aCurrent.x), static_cast<float>(aTarget.x))),
			static_cast<T>(LerpFloat(static_cast<float>(aCurrent.y), static_cast<float>(aTarget.y)))
		};
	}

	template<typename T>
	CONSTEXPR Vector2<T> Vector2<T>::Slerp(const Vector2& aCurrent, const Vector2& aTarget, float aPercentage)
	{
		const auto ClampFloat = [](float aValue, float aMin, float aMax) { return (aValue < aMin) ? aMin : ((aValue > aMax) ? aMax : aValue); };

		const float dot = ClampFloat(static_cast<float>(aCurrent.Dot(aTarget)), -1.0f, 1.0f);

		const Vector2<T> relativeVec = (aTarget - aCurrent * dot).GetNormalized();
		const float theta = std::acos(dot) * aPercentage;

		return aCurrent * std::cos(theta) + relativeVec * std::sin(theta);
	}

	template<typename T>
	CONSTEXPR Vector2<T> Vector2<T>::MoveTowards(const Vector2& aCurrent, const Vector2& aTarget, float aDistance)
	{
		return aCurrent + Vector2<T>::Direction(aCurrent, aTarget).GetNormalized() * aDistance;
	}

	template<typename T>
	CONSTEXPR T Vector2<T>::Length() const
	{
		return static_cast<T>(std::sqrt(LengthSqr()));
	}

	template<typename T>
	CONSTEXPR T Vector2<T>::LengthSqr() const
	{
		return Dot(*this);
	}

	template<typename T>
	CONSTEXPR Vector2<T> Vector2<T>::GetNormalized(T aRadius) const
	{
		return GetNormalized(Length(), aRadius);
	}
	template<typename T>
	CONSTEXPR Vector2<T> Vector2<T>::GetNormalized(T aLength, T aRadius) const
	{
		assert(aLength > T{} && "Negative or zero length is an error");
		return (*this) * (aRadius / aLength);
	}
	template<typename T>
	CONSTEXPR Vector2<T> Vector2<T>::GetNormalizedFast() const
	{
		// return this vector length squared multiplied inverse squared root for optimization

		const float lengthSqr = static_cast<float>(LengthSqr());

		assert(lengthSqr > T{} && "Negative or zero length is an error");

		const float v = std::bit_cast<float>(0x5f3759df - (std::bit_cast<std::uint32_t>(lengthSqr) >> 1));
		const float invRoot = v * (1.5f - (lengthSqr * 0.5f * v * v));

		return (*this) * static_cast<T>(invRoot);
	}

	template<typename T>
	CONSTEXPR void Vector2<T>::Normalize(T aRadius)
	{
		*this = GetNormalized(aRadius);
	}
	template<typename T>
	CONSTEXPR void Vector2<T>::NormalizeFast()
	{
		*this = GetNormalizedFast();
	}

	template<typename T>
	CONSTEXPR T Vector2<T>::Dot(const Vector2& aVector) const
	{
		return x * aVector.x + y * aVector.y;
	}

	template<typename T>
	CONSTEXPR Vector2<T> Vector2<T>::ProjectOnto(const Vector2& aVector) const
	{
		assert(aVector != Vector2<T>() && "Cannot project onto a zero vector");
		return (Dot(aVector) / aVector.LengthSqr()) * aVector;
	}

	template<typename T>
	CONSTEXPR Vector2<T> Vector2<T>::Reflect(const Vector2& aVector) const
	{
		return aVector - T{2} * ProjectOnto(aVector);
	}

	template<typename T>
	CONSTEXPR Vector3<T> Vector2<T>::XYZ(T aZ) const
	{
		return Vector3<T>(x, y, aZ);
	}

	template<typename T>
	CONSTEXPR Vector3<T> Vector2<T>::XZY(T aY) const
	{
		return Vector3<T>(x, aY, y);
	}

	template<typename T>
	CONSTEXPR Vector3<T> Vector2<T>::YZX(T aX) const
	{
		return Vector3<T>(aX, x, y);
	}

	// GLOBAL OPERATORS

	template<typename T>
	NODISC CONSTEXPR Vector2<T> operator-(const Vector2<T>& aVector)
	{
		return Vector2<T>(-aVector.x, -aVector.y);
	}

	template<typename T>
	CONSTEXPR Vector2<T>& operator+=(Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		aLeft.x += aRight.x;
		aLeft.y += aRight.y;

		return aLeft;
	}
	template<typename T>
	CONSTEXPR Vector2<T>& operator-=(Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		aLeft.x -= aRight.x;
		aLeft.y -= aRight.y;

		return aLeft;
	}

	template<typename T>
	CONSTEXPR Vector2<T>& operator*=(Vector2<T>& aLeft, T aRight)
	{
		aLeft.x *= aRight;
		aLeft.y *= aRight;

		return aLeft;
	}
	template<typename T>
	CONSTEXPR Vector2<T>& operator/=(Vector2<T>& aLeft, T aRight)
	{
		assert(aRight != 0 && "Cannot divide by zero");

		aLeft.x /= aRight;
		aLeft.y /= aRight;

		return aLeft;
	}

	template<typename T>
	CONSTEXPR Vector2<T>& operator*=(Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		aLeft.x *= aRight.x;
		aLeft.y *= aRight.y;

		return aLeft;
	}
	template<typename T>
	CONSTEXPR Vector2<T>& operator/=(Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && "Cannot divide by zero");

		aLeft.x /= aRight.x;
		aLeft.y /= aRight.y;

		return aLeft;
	}

	template<typename T>
	NODISC CONSTEXPR Vector2<T> operator+(const Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		return Vector2<T>(aLeft.x + aRight.x, aLeft.y + aRight.y);
	}
	template<typename T>
	NODISC CONSTEXPR Vector2<T> operator-(const Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		return Vector2<T>(aLeft.x - aRight.x, aLeft.y - aRight.y);
	}

	template<typename T>
	NODISC CONSTEXPR Vector2<T> operator*(const Vector2<T>& aLeft, T aRight)
	{
		return Vector2<T>(aLeft.x * aRight, aLeft.y * aRight);
	}
	template<typename T>
	NODISC CONSTEXPR Vector2<T> operator/(const Vector2<T>& aLeft, T aRight)
	{
		assert(aRight != 0 && "Cannot divide by zero");
		return Vector2<T>(aLeft.x / aRight, aLeft.y / aRight);
	}

	template<typename T>
	NODISC CONSTEXPR Vector2<T> operator*(const Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		return Vector2<T>(aLeft.x * aRight.x, aLeft.y * aRight.y);
	}
	template<typename T>
	NODISC CONSTEXPR Vector2<T> operator*(T aLeft, const Vector2<T>& aRight)
	{
		return aRight * aLeft;
	}

	template<typename T>
	NODISC CONSTEXPR Vector2<T> operator/(const Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && "Cannot divide by zero");
		return Vector2<T>(aLeft.x / aRight.x, aLeft.y / aRight.y);
	}
	template<typename T>
	NODISC CONSTEXPR Vector2<T> operator/(T aLeft, const Vector2<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && "Cannot divide by zero");
		return Vector2<T>(aLeft / aRight.x, aLeft / aRight.y);
	}

	template<typename T>
	NODISC CONSTEXPR bool operator==(const Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		return (aLeft.x == aRight.x) && (aLeft.y == aRight.y);
	}
	template<typename T>
	NODISC CONSTEXPR bool operator!=(const Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		return !(aLeft == aRight);
	}

	// using declarations

	using Vector2f = Vector2<float>;
	using Vector2d = Vector2<double>;
	using Vector2i = Vector2<int>;
	using Vector2u = Vector2<unsigned int>;
}
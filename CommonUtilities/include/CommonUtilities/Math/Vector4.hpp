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
	class Vector3;

	template<typename T>
	class Vector4
	{
	public:
		T x{};
		T y{};
		T z{};
		T w{};

		constexpr Vector4() = default;
		constexpr ~Vector4() = default;

		constexpr Vector4(T aValue);
		constexpr Vector4(T aX, T aY, T aZ, T aW);

		template <typename U>
		constexpr explicit Vector4(const Vector2<U>& aVector);

		template <typename U>
		constexpr explicit Vector4(const Vector3<U>& aVector);

		template <typename U>
		constexpr explicit Vector4(const Vector4<U>& aVector);

		template<class OtherVector>
		NODISC constexpr explicit operator OtherVector() const;

		///	Length of the vector.
		/// 
		NODISC constexpr T Length() const;

		/// Square length of the vector, useful for comparisons
		/// 
		NODISC constexpr T LengthSqr() const;

		/// Computes a normalized vector.
		/// 
		/// \param Radius: Length of the normalized vector
		/// 
		/// \returns Normalized vector
		/// 
		NODISC constexpr Vector4<T> GetNormalized(T aRadius = T(1)) const;

		/// Computes a normalized vector.
		/// 
		/// \param Length: Optimization param for when length may already have been calculated
		/// \param Radius: Length of the normalized vector
		/// 
		/// \returns Normalized vector
		/// 
		NODISC constexpr Vector4<T> GetNormalized(T aLength, T aRadius) const;

		/// Normalizes this vector
		/// 
		/// \param Radius: Length of the normalized vector
		/// 
		constexpr void Normalize(T aRadius = T(1)) &;

		/// Dot product of two vectors.
		/// 
		NODISC constexpr T Dot(const Vector4& aVector) const;

		/// Computes the fractional part of each member
		/// 
		/// \returns Vector with only the fractional values left
		/// 
		constexpr Vector4<T> GetFrac() const requires IsFloatingPoint<T>;

		/// Computes the ceil of each member
		/// 
		/// \returns Vector whose values have been rounded up
		/// 
		constexpr Vector4<T> GetCeil() const requires IsFloatingPoint<T>;

		/// Computes the floor of each member
		/// 
		/// \returns Vector whose values have been rounded down
		/// 
		constexpr Vector4<T> GetFloor() const requires IsFloatingPoint<T>;

		/// Computes the trunc of each member
		/// 
		/// \returns Vector whose values have been truncated
		/// 
		constexpr Vector4<T> GetTrunc() const requires IsFloatingPoint<T>;

		/// Computes the round of each member
		/// 
		/// \returns Vector whose values have been rounded to nearest
		/// 
		constexpr Vector4<T> GetRound() const requires IsFloatingPoint<T>;

		/// Rounds the vector values up
		/// 
		constexpr void Ceil() requires IsFloatingPoint<T>;

		/// Rounds the vector values down
		/// 
		constexpr void Floor() requires IsFloatingPoint<T>;

		/// Truncates the vector values
		/// 
		constexpr void Trunc() requires IsFloatingPoint<T>;

		/// Rounds the vector values
		/// 
		constexpr void Round() requires IsFloatingPoint<T>;

		/// \returns Converts this 4D vector to a 2D one.
		/// 
		NODISC constexpr Vector2<T> XY() const;

		/// \returns Converts this 4D vector to a 3D one.
		/// 
		NODISC constexpr Vector3<T> XYZ() const;

		/// \returns Directional vector pointing from current to target.
		/// 
		NODISC constexpr static Vector4 Direction(const Vector4& aCurrent, const Vector4& aTarget);

		/// \returns Distance from current to target.
		/// 
		NODISC constexpr static T Distance(const Vector4& aCurrent, const Vector4& aTarget);

		/// \returns Distance squared from current to target.
		/// 
		NODISC constexpr static T DistanceSqr(const Vector4& aCurrent, const Vector4& aTarget);

		/// \returns Dot product of the two vectors
		/// 
		NODISC constexpr static T Dot(const Vector4& aLeft, const Vector4& aRight);

		/// \returns Lerped vector between current and target.
		/// 
		NODISC constexpr static Vector4 Lerp(const Vector4& aCurrent, const Vector4& aTarget, T aPercentage);

		/// \returns Lerped vector between current and target.
		/// 
		NODISC constexpr static Vector4 CLerp(const Vector4& aCurrent, const Vector4& aTarget, T aPercentage);

		/// \returns Moved vector going from current towards target.
		/// 
		NODISC constexpr static Vector4 MoveTowards(const Vector4& aCurrent, const Vector4& aTarget, T aDistance);

		/// \returns Whether this equals another vector within a tolerance
		/// 
		NODISC constexpr static bool Equal(const Vector4<T>& aLeft, const Vector4<T>& aRight, T aTolerance = au::EPSILON_V<T>) requires IsFloatingPoint<T>;
	};

	template<typename T>
	constexpr Vector4<T>::Vector4(T aValue)
		: x(aValue), y(aValue), z(aValue), w(aValue) {}

	template<typename T>
	constexpr Vector4<T>::Vector4(T aX, T aY, T aZ, T aW)
		: x(aX), y(aY), z(aZ), w(aW) {}

	template<typename T>
	template<typename U>
	constexpr Vector4<T>::Vector4(const Vector2<U>& aVector)
		: x(static_cast<T>(aVector.x)), y(static_cast<T>(aVector.y)), z(T(0)), w(T(0)) {}

	template<typename T>
	template<typename U>
	constexpr Vector4<T>::Vector4(const Vector3<U>& aVector)
		: x(static_cast<T>(aVector.x)), y(static_cast<T>(aVector.y)), z(static_cast<T>(aVector.z)), w(T(0)) {}

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
	constexpr void Vector4<T>::Normalize(T aRadius) &
	{
		*this = GetNormalized(aRadius);
	}

	template<typename T>
	constexpr T Vector4<T>::Dot(const Vector4& aVector) const
	{
		return x * aVector.x + y * aVector.y + z * aVector.z + w * aVector.w;
	}

	template<typename T>
	constexpr Vector4<T> Vector4<T>::GetFrac() const requires IsFloatingPoint<T>
	{
		return (*this - GetFloor());
	}
	template<typename T>
	constexpr Vector4<T> Vector4<T>::GetCeil() const requires IsFloatingPoint<T>
	{
		return Vector4<T>(std::ceil(x), std::ceil(y), std::ceil(z), std::ceil(w));
	}
	template<typename T>
	constexpr Vector4<T> Vector4<T>::GetFloor() const requires IsFloatingPoint<T>
	{
		return Vector4<T>(std::floor(x), std::floor(y), std::floor(z), std::floor(w));
	}
	template<typename T>
	constexpr Vector4<T> Vector4<T>::GetTrunc() const requires IsFloatingPoint<T>
	{
		return Vector4<T>(std::trunc(x), std::trunc(y), std::trunc(z), std::trunc(w));
	}
	template<typename T>
	constexpr Vector4<T> Vector4<T>::GetRound() const requires IsFloatingPoint<T>
	{
		return Vector4<T>(std::round(x), std::round(y), std::round(z), std::round(w));
	}

	template<typename T>
	constexpr void Vector4<T>::Ceil() requires IsFloatingPoint<T>
	{
		*this = GetCeil();
	}
	template<typename T>
	constexpr void Vector4<T>::Floor() requires IsFloatingPoint<T>
	{
		*this = GetFloor();
	}
	template<typename T>
	constexpr void Vector4<T>::Trunc() requires IsFloatingPoint<T>
	{
		*this = GetTrunc();
	}
	template<typename T>
	constexpr void Vector4<T>::Round() requires IsFloatingPoint<T>
	{
		*this = GetRound();
	}

	template<typename T>
	constexpr Vector2<T> Vector4<T>::XY() const
	{
		return Vector2<T>(x, y);
	}
	template<typename T>
	constexpr Vector3<T> Vector4<T>::XYZ() const
	{
		return Vector3<T>(x, y, z);
	}

	template<typename T>
	constexpr Vector4<T> Vector4<T>::Direction(const Vector4& aCurrent, const Vector4& aTarget)
	{
		return Vector4<T>(
			aTarget.x - aCurrent.x, 
			aTarget.y - aCurrent.y, 
			aTarget.z - aCurrent.z,
			aTarget.w - aCurrent.w);
	}

	template<typename T>
	constexpr T Vector4<T>::Distance(const Vector4& aCurrent, const Vector4& aTarget)
	{
		return Direction(aCurrent, aTarget).Length();
	}

	template<typename T>
	constexpr T Vector4<T>::DistanceSqr(const Vector4& aCurrent, const Vector4& aTarget)
	{
		return Direction(aCurrent, aTarget).LengthSqr();
	}

	template<typename T>
	constexpr T Vector4<T>::Dot(const Vector4& aLeft, const Vector4& aRight)
	{
		return aLeft.Dot(aRight);
	}

	template<typename T>
	constexpr Vector4<T> Vector4<T>::Lerp(const Vector4& aCurrent, const Vector4& aTarget, T aPercentage)
	{
		static const auto Lerp = [](T aStart, T aEnd, T aPerc) { return aStart + aPerc * (aEnd - aStart); };

		return Vector4<T>
		{
			Lerp(aCurrent.x, aTarget.x, aPercentage),
			Lerp(aCurrent.y, aTarget.y, aPercentage),
			Lerp(aCurrent.z, aTarget.z, aPercentage),
			Lerp(aCurrent.w, aTarget.w, aPercentage)
		};
	}

	template<typename T>
	constexpr Vector4<T> Vector4<T>::CLerp(const Vector4& aCurrent, const Vector4& aTarget, T aPercentage)
	{
		static const auto Clamp = [](T aValue, T aMin, T aMax) { return (aValue < aMin) ? aMin : ((aValue > aMax) ? aMax : aValue); };
		static const auto Lerp = [](T aStart, T aEnd, T aPerc) { return aStart + aPerc * (aEnd - aStart); };

		return Vector4<T>
		{
			Clamp(Lerp(aCurrent.x, aTarget.x, aPercentage), (std::min)(aCurrent.x, aTarget.x), (std::max)(aCurrent.x, aTarget.x)),
			Clamp(Lerp(aCurrent.y, aTarget.y, aPercentage), (std::min)(aCurrent.y, aTarget.y), (std::max)(aCurrent.y, aTarget.y)),
			Clamp(Lerp(aCurrent.z, aTarget.z, aPercentage), (std::min)(aCurrent.z, aTarget.z), (std::max)(aCurrent.z, aTarget.z)),
			Clamp(Lerp(aCurrent.w, aTarget.w, aPercentage), (std::min)(aCurrent.w, aTarget.w), (std::max)(aCurrent.w, aTarget.w))
		};
	}

	template<typename T>
	constexpr Vector4<T> Vector4<T>::MoveTowards(const Vector4& aCurrent, const Vector4& aTarget, T aDistance)
	{
		if (Vector4<T> dir = Vector4<T>::Direction(aCurrent, aTarget); dir != Vector4<T>::Zero)
		{
			const T lenSqr	= dir.LengthSqr();
			const T distSqr = aDistance * aDistance;

			if (distSqr > lenSqr)
			{
				return aTarget;
			}
			else if (distSqr < 0.0f)
			{
				return aCurrent;
			}

			return aCurrent + dir.GetNormalized(std::sqrt(distSqr)) * aDistance;
		}

		return aCurrent;
	}

	template<typename T>
	constexpr bool Vector4<T>::Equal(const Vector4<T>& aLeft, const Vector4<T>& aRight, T aTolerance) requires IsFloatingPoint<T>
	{
		return	au::Equal(aLeft.x, aRight.x, aTolerance) &&
				au::Equal(aLeft.y, aRight.y, aTolerance) &&
				au::Equal(aLeft.z, aRight.z, aTolerance) &&
				au::Equal(aLeft.w, aRight.w, aTolerance);
	}

	// GLOBAL OPERATORS

	template<typename T>
	NODISC constexpr Vector4<T> operator-(const Vector4<T>& aVector)
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
	constexpr Vector4<T>& operator%=(Vector4<T>& aLeft, const Vector4<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && aRight.z != 0 && aRight.w != 0 && "Cannot modulo by zero");

		aLeft.x %= aRight.x;
		aLeft.y %= aRight.y;
		aLeft.z %= aRight.z;
		aLeft.w %= aRight.w;

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
	constexpr Vector4<T>& operator%=(Vector4<T>& aLeft, T aRight)
	{
		assert(aRight != 0 && "Cannot modulo by zero");

		aLeft.x %= aRight;
		aLeft.y %= aRight;
		aLeft.z %= aRight;
		aLeft.w %= aRight;

		return aLeft;
	}

	template<typename T>
	NODISC constexpr Vector4<T> operator+(const Vector4<T>& aLeft, const Vector4<T>& aRight)
	{
		return Vector4<T>(aLeft.x + aRight.x, aLeft.y + aRight.y, aLeft.z + aRight.z, aLeft.w + aRight.w);
	}
	template<typename T>
	NODISC constexpr Vector4<T> operator-(const Vector4<T>& aLeft, const Vector4<T>& aRight)
	{
		return Vector4<T>(aLeft.x - aRight.x, aLeft.y - aRight.y, aLeft.z - aRight.z, aLeft.w - aRight.w);
	}
	template<typename T>
	NODISC constexpr Vector4<T> operator*(const Vector4<T>& aLeft, const Vector4<T>& aRight)
	{
		return Vector4<T>(aLeft.x * aRight.x, aLeft.y * aRight.y, aLeft.z * aRight.z, aLeft.w * aRight.w);
	}
	template<typename T>
	NODISC constexpr Vector4<T> operator/(const Vector4<T>& aLeft, const Vector4<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && aRight.z != 0 && aRight.w != 0 && "Cannot divide by zero");
		return Vector4<T>(aLeft.x / aRight.x, aLeft.y / aRight.y, aLeft.z / aRight.z, aLeft.w / aRight.w);
	}
	template<typename T>
	NODISC constexpr Vector4<T> operator%(const Vector4<T>& aLeft, const Vector4<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && aRight.z != 0 && aRight.w != 0 && "Cannot modulo by zero");
		return Vector4<T>(aLeft.x % aRight.x, aLeft.y % aRight.y, aLeft.z % aRight.z, aLeft.w % aRight.w);
	}

	template<typename T>
	NODISC constexpr Vector4<T> operator*(const Vector4<T>& aLeft, T aRight)
	{
		return Vector4<T>(aLeft.x * aRight, aLeft.y * aRight, aLeft.z * aRight, aLeft.w * aRight);
	}
	template<typename T>
	NODISC constexpr Vector4<T> operator/(const Vector4<T>& aLeft, T aRight)
	{
		assert(aRight != 0 && "Cannot divide by zero");
		return Vector4<T>(aLeft.x / aRight, aLeft.y / aRight, aLeft.z / aRight, aLeft.w / aRight);
	}
	template<typename T>
	NODISC constexpr Vector4<T> operator%(const Vector4<T>& aLeft, T aRight)
	{
		assert(aRight != 0 && "Cannot modulo by zero");
		return Vector4<T>(aLeft.x % aRight, aLeft.y % aRight, aLeft.z % aRight, aLeft.w % aRight);
	}

	template<typename T>
	NODISC constexpr Vector4<T> operator*(T aLeft, const Vector4<T>& aRight)
	{
		return aRight * aLeft;
	}
	template<typename T>
	NODISC constexpr Vector4<T> operator/(T aLeft, const Vector4<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && aRight.z != 0 && aRight.w != 0 && "Cannot divide by zero");
		return Vector4<T>(aLeft / aRight.x, aLeft / aRight.y, aLeft / aRight.z, aLeft / aRight.w);
	}
	template<typename T>
	NODISC constexpr Vector4<T> operator%(T aLeft, const Vector4<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && aRight.z != 0 && aRight.w != 0 && "Cannot modulo by zero");
		return Vector4<T>(aLeft % aRight.x, aLeft % aRight.y, aLeft % aRight.z, aLeft % aRight.w);
	}

	template<typename T>
	NODISC constexpr bool operator==(const Vector4<T>& aLeft, const Vector4<T>& aRight)
	{
		return (aLeft.x == aRight.x) && (aLeft.y == aRight.y) && (aLeft.z == aRight.z) && (aLeft.w == aRight.w);
	}
	template<typename T>
	NODISC constexpr bool operator!=(const Vector4<T>& aLeft, const Vector4<T>& aRight)
	{
		return !(aLeft == aRight);
	}

	// using declarations

	using Vector4f = Vector4<float>;
	using Vector4d = Vector4<double>;
	using Vector4i = Vector4<int>;
	using Vector4u = Vector4<unsigned>;
}
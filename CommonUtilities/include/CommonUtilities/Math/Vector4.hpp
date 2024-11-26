#pragma once

#include <cmath>
#include <cassert>
#include <immintrin.h>
#include <array>
#include <tuple>

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

		constexpr explicit Vector4(T aValue);
		constexpr Vector4(T aX, T aY, T aZ, T aW);
		constexpr Vector4(const std::array<T, 4>& aArray);
		constexpr Vector4(__m128 aRegister) requires (std::is_same_v<T, float>);

		constexpr explicit Vector4(const Vector3<T>& aVector, T aW);
		constexpr explicit Vector4(T aX, const Vector3<T>& aVector);

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

		/// Computes a normalized vector if length of vector is a non-negative number above 0.
		/// 
		/// \param Radius: Length of the normalized vector
		/// 
		/// \returns Normalized vector
		/// 
		NODISC constexpr Vector4<T> GetNormalizedSafe(T aRadius = T(1)) const;

		/// Computes a normalized vector if length of vector is a non-negative number above 0.
		/// 
		/// \param Length: Optimization param for when length may already have been calculated
		/// \param Radius: Length of the normalized vector
		/// 
		/// \returns Normalized vector
		/// 
		NODISC constexpr Vector4<T> GetNormalizedSafe(T aLength, T aRadius) const;

		/// Computes a normalized vector.
		/// 
		/// \param Radius: Length of the normalized vector
		/// 
		/// \returns Normalized vector and current length
		/// 
		NODISC constexpr std::pair<Vector4<T>, T> GetNormalizedWithLength(T aRadius = T(1)) const;

		/// Normalizes this vector
		/// 
		/// \param Radius: Length of the normalized vector
		/// 
		constexpr void Normalize(T aRadius = T(1)) &;

		/// Normalizes this vector
		/// 
		/// \param Radius: Length of the normalized vector
		/// 
		constexpr void NormalizeSafe(T aRadius = T(1)) &;

		/// Dot product of two vectors.
		/// 
		NODISC constexpr T Dot(const Vector4& aVector) const;

		/// Computes the abs of each member
		/// 
		/// \returns Vector whose values have been made absolute
		/// 
		constexpr Vector4<T> GetAbs() const;

		/// Computes the fractional part of each member
		/// 
		/// \returns Vector with only the fractional values left
		/// 
		constexpr Vector4<T> GetFrac() const requires IsFloatingPointType<T>;

		/// Computes the ceil of each member
		/// 
		/// \returns Vector whose values have been rounded up
		/// 
		constexpr Vector4<T> GetCeil() const requires IsFloatingPointType<T>;

		/// Computes the floor of each member
		/// 
		/// \returns Vector whose values have been rounded down
		/// 
		constexpr Vector4<T> GetFloor() const requires IsFloatingPointType<T>;

		/// Computes the trunc of each member
		/// 
		/// \returns Vector whose values have been truncated
		/// 
		constexpr Vector4<T> GetTrunc() const requires IsFloatingPointType<T>;

		/// Computes the round of each member
		/// 
		/// \returns Vector whose values have been rounded to nearest
		/// 
		constexpr Vector4<T> GetRound() const requires IsFloatingPointType<T>;

		/// Rounds the vector values up
		/// 
		constexpr void Ceil() requires IsFloatingPointType<T>;

		/// Rounds the vector values down
		/// 
		constexpr void Floor() requires IsFloatingPointType<T>;

		/// Truncates the vector values
		/// 
		constexpr void Trunc() requires IsFloatingPointType<T>;

		/// Rounds the vector values
		/// 
		constexpr void Round() requires IsFloatingPointType<T>;

		/// \returns Converts this 4D vector to a 2D one.
		/// 
		NODISC constexpr Vector2<T> XY() const;

		/// \returns Converts this 4D vector to a 3D one.
		/// 
		NODISC constexpr Vector3<T> XYZ() const;

		/// \returns Converts this 4D vector to SIMD.
		/// 
		NODISC constexpr __m128 ToSIMD() const requires (std::is_same_v<T, float>);

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
		NODISC constexpr static Vector4 Lerp(const Vector4& aCurrent, const Vector4& aTarget, float aPercentage);

		/// \returns Lerped vector between current and target.
		/// 
		NODISC constexpr static Vector4 CLerp(const Vector4& aCurrent, const Vector4& aTarget, float aPercentage);

		/// \returns Moved vector going from current towards target.
		/// 
		NODISC constexpr static Vector4 MoveTowards(const Vector4& aCurrent, const Vector4& aTarget, T aDistance);

		/// \returns Whether this equals another vector within a tolerance
		/// 
		NODISC constexpr static bool Equal(const Vector4<T>& aLeft, const Vector4<T>& aRight, T aTolerance = EPSILON_V<T>) requires IsFloatingPointType<T>;
	};

	template<typename T>
	constexpr Vector4<T>::Vector4(T aValue)
		: x(aValue), y(aValue), z(aValue), w(aValue) {}

	template<typename T>
	constexpr Vector4<T>::Vector4(T aX, T aY, T aZ, T aW)
		: x(aX), y(aY), z(aZ), w(aW) {}

	template<typename T>
	constexpr Vector4<T>::Vector4(const std::array<T, 4>& aArray)
		: Vector4(aArray[0], aArray[1], aArray[2], aArray[4]) {}

	template<typename T>
	constexpr Vector4<T>::Vector4(__m128 aRegister) requires (std::is_same_v<T, float>)
	{
		alignas(16) std::array<float, 4> values{};
		_mm_store_ps(values.data(), aRegister);
		x = values[0], y = values[1], z = values[2], w = values[3];
	}

	template<typename T>
	constexpr Vector4<T>::Vector4(const Vector3<T>& aVector, T aW)
		: Vector4(aVector.x, aVector.y, aVector.z, aW) {}

	template<typename T>
	constexpr Vector4<T>::Vector4(T aX, const Vector3<T>& aVector)
		: Vector4(aX, aVector.x, aVector.y, aVector.z) {}

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
	constexpr Vector4<T> Vector4<T>::GetNormalizedSafe(T aRadius) const
	{
		if (const T lenSqr = LengthSqr(); lenSqr >= EPSILON_V<T> *EPSILON_V<T>)
			return GetNormalized((T)std::sqrt(lenSqr), aRadius);

		return *this;
	}
	template<typename T>
	constexpr Vector4<T> Vector4<T>::GetNormalizedSafe(T aLength, T aRadius) const
	{
		if (aLength >= EPSILON_V<T>)
			return GetNormalized(aLength, aRadius);

		return *this;
	}
	template<typename T>
	constexpr std::pair<Vector4<T>, T> Vector4<T>::GetNormalizedWithLength(T aRadius) const
	{
		const T length = Length();
		return std::make_pair(GetNormalized(length, aRadius), length);
	}

	template<typename T>
	constexpr void Vector4<T>::Normalize(T aRadius) &
	{
		*this = GetNormalized(aRadius);
	}

	template<typename T>
	constexpr void Vector4<T>::NormalizeSafe(T aRadius) &
	{
		*this = GetNormalizedSafe(aRadius);
	}

	template<typename T>
	constexpr T Vector4<T>::Dot(const Vector4& aVector) const
	{
		return x * aVector.x + y * aVector.y + z * aVector.z + w * aVector.w;
	}

	template<typename T>
	constexpr Vector4<T> Vector4<T>::GetAbs() const
	{
		return Vector4<T>(std::abs(x), std::abs(y), std::abs(z), std::abs(w));
	}
	template<typename T>
	constexpr Vector4<T> Vector4<T>::GetFrac() const requires IsFloatingPointType<T>
	{
		return (*this - GetFloor());
	}
	template<typename T>
	constexpr Vector4<T> Vector4<T>::GetCeil() const requires IsFloatingPointType<T>
	{
		return Vector4<T>(std::ceil(x), std::ceil(y), std::ceil(z), std::ceil(w));
	}
	template<typename T>
	constexpr Vector4<T> Vector4<T>::GetFloor() const requires IsFloatingPointType<T>
	{
		return Vector4<T>(std::floor(x), std::floor(y), std::floor(z), std::floor(w));
	}
	template<typename T>
	constexpr Vector4<T> Vector4<T>::GetTrunc() const requires IsFloatingPointType<T>
	{
		return Vector4<T>(std::trunc(x), std::trunc(y), std::trunc(z), std::trunc(w));
	}
	template<typename T>
	constexpr Vector4<T> Vector4<T>::GetRound() const requires IsFloatingPointType<T>
	{
		return Vector4<T>(std::round(x), std::round(y), std::round(z), std::round(w));
	}

	template<typename T>
	constexpr void Vector4<T>::Ceil() requires IsFloatingPointType<T>
	{
		*this = GetCeil();
	}
	template<typename T>
	constexpr void Vector4<T>::Floor() requires IsFloatingPointType<T>
	{
		*this = GetFloor();
	}
	template<typename T>
	constexpr void Vector4<T>::Trunc() requires IsFloatingPointType<T>
	{
		*this = GetTrunc();
	}
	template<typename T>
	constexpr void Vector4<T>::Round() requires IsFloatingPointType<T>
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
	constexpr __m128 Vector4<T>::ToSIMD() const requires (std::is_same_v<T, float>)
	{
		alignas(16) const std::array<float, 4> values{ x, y, z, w };
		return _mm_load_ps(values.data());
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
	constexpr Vector4<T> Vector4<T>::Lerp(const Vector4& aCurrent, const Vector4& aTarget, float aPercentage)
	{
		using CommonUtilities::Lerp;

		return Vector4<T>
		{
			Lerp(aCurrent.x, aTarget.x, aPercentage),
			Lerp(aCurrent.y, aTarget.y, aPercentage),
			Lerp(aCurrent.z, aTarget.z, aPercentage),
			Lerp(aCurrent.w, aTarget.w, aPercentage)
		};
	}

	template<typename T>
	constexpr Vector4<T> Vector4<T>::CLerp(const Vector4& aCurrent, const Vector4& aTarget, float aPercentage)
	{
		using CommonUtilities::Clamp;
		using CommonUtilities::Lerp;

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
	constexpr bool Vector4<T>::Equal(const Vector4<T>& aLeft, const Vector4<T>& aRight, T aTolerance) requires IsFloatingPointType<T>
	{
		using CommonUtilities::Equal; // koenig lookup if you wanna know

		return	Equal(aLeft.x, aRight.x, aTolerance) &&
				Equal(aLeft.y, aRight.y, aTolerance) &&
				Equal(aLeft.z, aRight.z, aTolerance) &&
				Equal(aLeft.w, aRight.w, aTolerance);
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

	template<typename T>
	NODISC constexpr Vector4<T> Lerp(const Vector4<T>& aStart, const Vector4<T>& aEnd, float aPercentage)
	{
		return Vector4<T>::Lerp(aStart, aEnd, aPercentage);
	}

	// using declarations

	using Vector4f = Vector4<float>;
	using Vector4d = Vector4<double>;
	using Vector4i = Vector4<int>;
	using Vector4u = Vector4<unsigned>;
}
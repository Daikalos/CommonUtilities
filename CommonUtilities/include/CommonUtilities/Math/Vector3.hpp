#pragma once

#include <cmath>
#include <cassert>
#include <xmmintrin.h>
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

		constexpr Vector3() = default;
		constexpr ~Vector3() = default;

		constexpr Vector3(T aValue);
		constexpr Vector3(T aX, T aY, T aZ);

		template <typename U>
		constexpr explicit Vector3(const Vector2<U>& aVector);

		template <typename U>
		constexpr explicit Vector3(const Vector4<U>& aVector);

		template <typename U>
		constexpr explicit Vector3(const Vector3<U>& aVector);

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
		NODISC constexpr Vector3<T> GetNormalized(T aRadius = T(1)) const;

		/// Computes a normalized vector.
		/// 
		/// \param Length: Optimization param for when length may already have been calculated
		/// \param Radius: Length of the normalized vector
		/// 
		/// \returns Normalized vector
		/// 
		NODISC constexpr Vector3<T> GetNormalized(T aLength, T aRadius) const;

		/// Normalizes this vector.
		/// 
		/// \param Radius: Length of the normalized vector
		/// 
		constexpr void Normalize(T aRadius = T(1)) &;

		/// Dot product of two vectors.
		/// 
		NODISC constexpr T Dot(const Vector3& aVector) const;

		/// Cross product of two vectors.
		/// 
		/// \returns The perpendicular vector to the two vectors
		/// 
		NODISC constexpr Vector3 Cross(const Vector3& aVector) const;

		/// Projects this vector onto another.
		/// 
		/// \param Vector: vector to project onto
		/// 
		/// \returns Projected vector
		/// 
		NODISC constexpr Vector3 ProjectOnto(const Vector3& aVector) const;

		/// Reflects provided vector from this acting as a normal.
		/// 
		/// \param Vector: vector to reflect.
		/// 
		/// \returns Reflected vector
		/// 
		NODISC constexpr Vector3 Reflect(const Vector3& aVector) const;

		/// Computes the shortest angle between the two vectors.
		/// 
		/// \param Vector: vector to compare to.
		/// 
		/// \returns Angle in the range [-PI, PI) radians
		/// 
		NODISC constexpr T AngleTo(const Vector3& aVector) const;

		/// Computes the fractional part of each member
		/// 
		/// \returns Vector with only the fractional values left
		/// 
		constexpr Vector3<T> GetFrac() const requires IsFloatingPointType<T>;

		/// Computes the ceil of each member
		/// 
		/// \returns Vector whose values have been rounded up
		/// 
		constexpr Vector3<T> GetCeil() const requires IsFloatingPointType<T>;

		/// Computes the floor of each member
		/// 
		/// \returns Vector whose values have been rounded down
		/// 
		constexpr Vector3<T> GetFloor() const requires IsFloatingPointType<T>;

		/// Computes the trunc of each member
		/// 
		/// \returns Vector whose values have been truncated
		/// 
		constexpr Vector3<T> GetTrunc() const requires IsFloatingPointType<T>;

		/// Computes the round of each member
		/// 
		/// \returns Vector whose values have been rounded to nearest
		/// 
		constexpr Vector3<T> GetRound() const requires IsFloatingPointType<T>;

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

		/// \returns Converts this 3D vector to a 2D one.
		/// 
		NODISC constexpr Vector2<T> XY() const;

		/// \returns Converts this 3D vector to a 2D one.
		/// 
		NODISC constexpr Vector2<T> XZ() const;

		/// \returns Converts this 3D vector to a 2D one.
		/// 
		NODISC constexpr Vector2<T> YZ() const;

		/// \returns Converts this 3D vector to a 4D one.
		/// 
		NODISC constexpr Vector4<T> XYZW(T aW = T{}) const;

		/// \returns Converts this 3D vector to SIMD.
		/// 
		NODISC constexpr __m128 ToSIMD() const requires (std::is_same_v<T, float>);

		/// \returns Directional vector pointing from current to target.
		/// 
		NODISC constexpr static Vector3 Direction(const Vector3& aCurrent, const Vector3& aTarget);

		/// \returns Distance from current to target.
		/// 
		NODISC constexpr static T Distance(const Vector3& aCurrent, const Vector3& aTarget);

		/// \returns Distance squared from current to target.
		/// 
		NODISC constexpr static T DistanceSqr(const Vector3& aCurrent, const Vector3& aTarget);

		/// \returns Dot product of the two vectors
		/// 
		NODISC constexpr static T Dot(const Vector3& aLeft, const Vector3& aRight);

		/// \returns Lerped vector between current and target.
		/// 
		NODISC constexpr static Vector3 Lerp(const Vector3& aCurrent, const Vector3& aTarget, T aPercentage);

		/// \returns Clamped lerped vector between current and target.
		/// 
		NODISC constexpr static Vector3 CLerp(const Vector3& aCurrent, const Vector3& aTarget, T aPercentage);

		/// \returns Moved vector going from current towards target.
		/// 
		NODISC constexpr static Vector3 MoveTowards(const Vector3& aCurrent, const Vector3& aTarget, T aDistance);

		/// \returns Whether this equals another vector within a tolerance
		/// 
		NODISC constexpr static bool Equal(const Vector3& aLeft, const Vector3& aRight, T aTolerance = EPSILON_V<T>) requires IsFloatingPointType<T>;

		static const Vector3 Zero;
		static const Vector3 One;
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
	const Vector3<T> Vector3<T>::One(static_cast<T>(1), static_cast<T>(1), static_cast<T>(1));

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
	constexpr Vector3<T>::Vector3(T aValue)
		: x(aValue), y(aValue), z(aValue) {}

	template<typename T>
	constexpr Vector3<T>::Vector3(T aX, T aY, T aZ)
		: x(aX), y(aY), z(aZ) {}

	template<typename T>
	template<typename U>
	constexpr Vector3<T>::Vector3(const Vector2<U>& aVector)
		: x(static_cast<T>(aVector.x)), y(static_cast<T>(aVector.y)), z(T(0)) {}

	template<typename T>
	template<typename U>
	constexpr Vector3<T>::Vector3(const Vector4<U>& aVector)
		: x(static_cast<T>(aVector.x)), y(static_cast<T>(aVector.y)), z(static_cast<T>(aVector.z)) {}

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
	constexpr void Vector3<T>::Normalize(T aRadius) &
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

	template<typename T>
	constexpr Vector3<T> Vector3<T>::Reflect(const Vector3& aVector) const
	{
		return aVector - T{2} * ProjectOnto(aVector);
	}

	template<typename T>
	constexpr T Vector3<T>::AngleTo(const Vector3& aVector) const
	{
		return std::acos(Dot(aVector) / (Length() * aVector.Length()));
	}

	template<typename T>
	constexpr Vector3<T> Vector3<T>::GetFrac() const requires IsFloatingPointType<T>
	{
		return (*this - GetFloor());
	}
	template<typename T>
	constexpr Vector3<T> Vector3<T>::GetCeil() const requires IsFloatingPointType<T>
	{
		return Vector3<T>(std::ceil(x), std::ceil(y), std::ceil(z));
	}
	template<typename T>
	constexpr Vector3<T> Vector3<T>::GetFloor() const requires IsFloatingPointType<T>
	{
		return Vector3<T>(std::floor(x), std::floor(y), std::floor(z));
	}
	template<typename T>
	constexpr Vector3<T> Vector3<T>::GetTrunc() const requires IsFloatingPointType<T>
	{
		return Vector3<T>(std::trunc(x), std::trunc(y), std::trunc(z));
	}
	template<typename T>
	constexpr Vector3<T> Vector3<T>::GetRound() const requires IsFloatingPointType<T>
	{
		return Vector3<T>(std::round(x), std::round(y), std::round(z));
	}

	template<typename T>
	constexpr void Vector3<T>::Ceil() requires IsFloatingPointType<T>
	{
		*this = GetCeil();
	}
	template<typename T>
	constexpr void Vector3<T>::Floor() requires IsFloatingPointType<T>
	{
		*this = GetFloor();
	}
	template<typename T>
	constexpr void Vector3<T>::Trunc() requires IsFloatingPointType<T>
	{
		*this = GetTrunc();
	}
	template<typename T>
	constexpr void Vector3<T>::Round() requires IsFloatingPointType<T>
	{
		*this = GetRound();
	}

	template<typename T>
	constexpr Vector2<T> Vector3<T>::XY() const
	{
		return Vector2<T>(x, y);
	}
	template<typename T>
	constexpr Vector2<T> Vector3<T>::XZ() const
	{
		return Vector2<T>(x, z);
	}
	template<typename T>
	constexpr Vector2<T> Vector3<T>::YZ() const
	{
		return Vector2<T>(y, z);
	}
	template<typename T>
	constexpr Vector4<T> Vector3<T>::XYZW(T aW) const
	{
		return Vector4<T>(x, y, z, aW);
	}

	template<typename T>
	constexpr __m128 Vector3<T>::ToSIMD() const requires (std::is_same_v<T, float>)
	{
		return __m128{ x, y, z, T(0) };
	}

	template<typename T>
	constexpr Vector3<T> Vector3<T>::Direction(const Vector3& aCurrent, const Vector3& aTarget)
	{
		return Vector3<T>(aTarget.x - aCurrent.x, aTarget.y - aCurrent.y, aTarget.z - aCurrent.z);
	}

	template<typename T>
	constexpr T Vector3<T>::Distance(const Vector3& aCurrent, const Vector3& aTarget)
	{
		return Direction(aCurrent, aTarget).Length();
	}

	template<typename T>
	constexpr T Vector3<T>::DistanceSqr(const Vector3& aCurrent, const Vector3& aTarget)
	{
		return Direction(aCurrent, aTarget).LengthSqr();
	}

	template<typename T>
	constexpr T Vector3<T>::Dot(const Vector3& aLeft, const Vector3& aRight)
	{
		return aLeft.Dot(aRight);
	}

	template<typename T>
	constexpr Vector3<T> Vector3<T>::Lerp(const Vector3& aCurrent, const Vector3& aTarget, T aPercentage)
	{
		const auto Lerp = [](T aStart, T aEnd, T aPerc) { return aStart + aPerc * (aEnd - aStart); };

		return Vector3<T>
		{
			Lerp(aCurrent.x, aTarget.x, aPercentage),
			Lerp(aCurrent.y, aTarget.y, aPercentage),
			Lerp(aCurrent.z, aTarget.z, aPercentage)
		};
	}

	template<typename T>
	constexpr Vector3<T> Vector3<T>::CLerp(const Vector3& aCurrent, const Vector3& aTarget, T aPercentage)
	{
		const auto Clamp = [](T aValue, T aMin, T aMax) { return (aValue < aMin) ? aMin : ((aValue > aMax) ? aMax : aValue); };
		const auto Lerp = [](T aStart, T aEnd, T aPerc) { return aStart + aPerc * (aEnd - aStart); };

		return Vector3<T>
		{
			Clamp(Lerp(aCurrent.x, aTarget.x, aPercentage), (std::min)(aCurrent.x, aTarget.x), (std::max)(aCurrent.x, aTarget.x)),
			Clamp(Lerp(aCurrent.y, aTarget.y, aPercentage), (std::min)(aCurrent.y, aTarget.y), (std::max)(aCurrent.y, aTarget.y)),
			Clamp(Lerp(aCurrent.z, aTarget.z, aPercentage), (std::min)(aCurrent.z, aTarget.z), (std::max)(aCurrent.z, aTarget.z))
		};
	}

	template<typename T>
	constexpr Vector3<T> Vector3<T>::MoveTowards(const Vector3& aCurrent, const Vector3& aTarget, T aDistance)
	{
		if (Vector3<T> dir = Vector2<T>::Direction(aCurrent, aTarget); dir != Vector3<T>::Zero)
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
	constexpr bool Vector3<T>::Equal(const Vector3& aLeft, const Vector3& aRight, T aTolerance) requires IsFloatingPointType<T>
	{
		return	Equal(aLeft.x, aRight.x, aTolerance) &&
				Equal(aLeft.y, aRight.y, aTolerance) &&
				Equal(aLeft.z, aRight.z, aTolerance);
	}

	// GLOBAL OPERATORS

	template<typename T>
	NODISC constexpr Vector3<T> operator-(const Vector3<T>& aVector)
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
	constexpr Vector3<T>& operator%=(Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && aRight.z != 0 && "Cannot modulo by zero");

		aLeft.x %= aRight.x;
		aLeft.y %= aRight.y;
		aLeft.z %= aRight.z;

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
	constexpr Vector3<T>& operator%=(Vector3<T>& aLeft, T aRight)
	{
		assert(aRight != 0 && "Cannot modulo by zero");

		aLeft.x %= aRight;
		aLeft.y %= aRight;
		aLeft.z %= aRight;

		return aLeft;
	}

	template<typename T>
	NODISC constexpr Vector3<T> operator+(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		return Vector3<T>(aLeft.x + aRight.x, aLeft.y + aRight.y, aLeft.z + aRight.z);
	}
	template<typename T>
	NODISC constexpr Vector3<T> operator-(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		return Vector3<T>(aLeft.x - aRight.x, aLeft.y - aRight.y, aLeft.z - aRight.z);
	}
	template<typename T>
	NODISC constexpr Vector3<T> operator*(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		return Vector3<T>(aLeft.x * aRight.x, aLeft.y * aRight.y, aLeft.z * aRight.z);
	}
	template<typename T>
	NODISC constexpr Vector3<T> operator/(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && aRight.z != 0 && "Cannot divide by zero");
		return Vector3<T>(aLeft.x / aRight.x, aLeft.y / aRight.y, aLeft.z / aRight.z);
	}
	template<typename T>
	NODISC constexpr Vector3<T> operator%(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && aRight.z != 0 && "Cannot modulo by zero");
		return Vector3<T>(aLeft.x % aRight.x, aLeft.y % aRight.y, aLeft.z % aRight.z);
	}

	template<typename T>
	NODISC constexpr Vector3<T> operator*(const Vector3<T>& aLeft, T aRight)
	{
		return Vector3<T>(aLeft.x * aRight, aLeft.y * aRight, aLeft.z * aRight);
	}
	template<typename T>
	NODISC constexpr Vector3<T> operator/(const Vector3<T>& aLeft, T aRight)
	{
		assert(aRight != 0 && "Cannot divide by zero");
		return Vector3<T>(aLeft.x / aRight, aLeft.y / aRight, aLeft.z / aRight);
	}
	template<typename T>
	NODISC constexpr Vector3<T> operator%(const Vector3<T>& aLeft, T aRight)
	{
		assert(aRight != 0 && "Cannot modulo by zero");
		return Vector3<T>(aLeft.x % aRight, aLeft.y % aRight, aLeft.z % aRight);
	}

	template<typename T>
	NODISC constexpr Vector3<T> operator*(T aLeft, const Vector3<T>& aRight)
	{
		return aRight * aLeft;
	}
	template<typename T>
	NODISC constexpr Vector3<T> operator/(T aLeft, const Vector3<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && aRight.z != 0 && "Cannot divide by zero");
		return Vector3<T>(aLeft / aRight.x, aLeft / aRight.y, aLeft / aRight.z);
	}
	template<typename T>
	NODISC constexpr Vector3<T> operator%(T aLeft, const Vector3<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && aRight.z != 0 && "Cannot modulo by zero");
		return Vector3<T>(aLeft % aRight.x, aLeft % aRight.y, aLeft % aRight.z);
	}

	template<typename T>
	NODISC constexpr bool operator==(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		return (aLeft.x == aRight.x) && (aLeft.y == aRight.y) && (aLeft.z == aRight.z);
	}
	template<typename T>
	NODISC constexpr bool operator!=(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		return !(aLeft == aRight);
	}

	// using declarations

	using Vector3f = Vector3<float>;
	using Vector3d = Vector3<double>;
	using Vector3i = Vector3<int>;
	using Vector3u = Vector3<unsigned>;
}
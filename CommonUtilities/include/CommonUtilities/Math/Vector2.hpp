#pragma once

#include <cmath>
#include <cassert>
#include <bit>

#include <CommonUtilities/Utility/ArithmeticUtils.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T>
	class Vector3;

	template<typename T>
	class Vector4;

	template<typename T>
	class Vector2
	{
	public:
		T x{};
		T y{};

		constexpr Vector2() = default;
		constexpr ~Vector2() = default;

		constexpr Vector2(T aValue);
		constexpr Vector2(T aX, T aY);

		template <typename U>
		constexpr explicit Vector2(const Vector3<U>& aVector);

		template <typename U>
		constexpr explicit Vector2(const Vector4<U>& aVector);

		template <typename U>
		constexpr explicit Vector2(const Vector2<U>& aVector);

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
		NODISC constexpr Vector2<T> GetNormalized(T aRadius = T(1)) const;

		/// Computes a normalized vector.
		/// 
		/// \param Length: Optimization param for when length may already have been calculated
		/// \param Radius: Length of the normalized vector
		/// 
		/// \returns Normalized vector
		/// 
		NODISC constexpr Vector2<T> GetNormalized(T aLength, T aRadius) const;

		/// Normalizes this vector.
		/// 
		/// \param Radius: Length of the normalized vector
		/// 
		constexpr void Normalize(T aRadius = T(1)) &;

		/// Dot product of two vectors.
		/// 
		NODISC constexpr T Dot(const Vector2& aVector) const;

		/// Projects this vector onto another.
		/// 
		/// \param Vector: vector to project onto
		/// 
		/// \returns Projected vector
		/// 
		NODISC constexpr Vector2 ProjectOnto(const Vector2& aVector) const;

		/// Reflects provided vector from this acting as a normal.
		/// 
		/// \param Vector: vector to reflect.
		/// 
		/// \returns Reflected vector
		/// 
		NODISC constexpr Vector2 Reflect(const Vector2& aVector) const;

		/// \returns Converts this 2D vector to a 3D one.
		/// 
		NODISC constexpr Vector3<T> XYZ(T aZ = T{}) const;

		/// \returns Converts this 2D vector to a 3D one.
		/// 
		NODISC constexpr Vector3<T> XZY(T aY = T{}) const;

		/// \returns Converts this 2D vector to a 3D one.
		/// 
		NODISC constexpr Vector3<T> YZX(T aX = T{}) const;

		/// \returns Converts this 2D vector to a 4D one.
		/// 
		NODISC constexpr Vector4<T> XYZW(T aZ = T{}, T aW = T{}) const;

		/// \returns Directional vector pointing from current to target.
		/// 
		NODISC constexpr static Vector2 Direction(const Vector2& aCurrent, const Vector2& aTarget);

		/// \returns Distance from current to target.
		/// 
		NODISC constexpr static T Distance(const Vector2& aCurrent, const Vector2& aTarget);

		/// \returns Distance squared from current to target.
		/// 
		NODISC constexpr static T DistanceSqr(const Vector2& aCurrent, const Vector2& aTarget);

		/// \returns Lerped vector between current and target.
		/// 
		NODISC constexpr static Vector2 Lerp(const Vector2& aCurrent, const Vector2& aTarget, T aPercentage);

		/// \returns Clamped lerped vector between current and target.
		/// 
		NODISC constexpr static Vector2 CLerp(const Vector2& aCurrent, const Vector2& aTarget, T aPercentage);

		/// \returns Moved vector going from current towards target.
		/// 
		NODISC constexpr static Vector2 MoveTowards(const Vector2& aCurrent, const Vector2& aTarget, T aDistance);

		/// \returns Whether left equals right vector within a tolerance
		/// 
		NODISC constexpr static bool Equal(const Vector2& aLeft, const Vector2& aRight, T aTolerance = au::EPSILON_V<T>);

		static const Vector2 Zero;
		static const Vector2 One;
		static const Vector2 Right;
		static const Vector2 Left;
		static const Vector2 Up;
		static const Vector2 Down;
	};

	template <typename T>
	const Vector2<T> Vector2<T>::Zero(static_cast<T>(0), static_cast<T>(0));

	template <typename T>
	const Vector2<T> Vector2<T>::One(static_cast<T>(1), static_cast<T>(1));

	template <typename T>
	const Vector2<T> Vector2<T>::Right(static_cast<T>(1), static_cast<T>(0));

	template <typename T>
	const Vector2<T> Vector2<T>::Left(static_cast<T>(-1), static_cast<T>(0));

	template <typename T>
	const Vector2<T> Vector2<T>::Up(static_cast<T>(0), static_cast<T>(1));

	template <typename T>
	const Vector2<T> Vector2<T>::Down(static_cast<T>(0), static_cast<T>(-1));

	template<typename T>
	constexpr Vector2<T>::Vector2(T aValue)
		: x(aValue), y(aValue) {}

	template<typename T>
	constexpr Vector2<T>::Vector2(T aX, T aY)
		: x(aX), y(aY) {}

	template<typename T>
	template<typename U>
	constexpr Vector2<T>::Vector2(const Vector3<U>& aVector)
		: x(static_cast<T>(aVector.x)), y(static_cast<T>(aVector.y)) {}

	template<typename T>
	template<typename U>
	constexpr Vector2<T>::Vector2(const Vector4<U>& aVector)
		: x(static_cast<T>(aVector.x)), y(static_cast<T>(aVector.y)) {}

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
	constexpr void Vector2<T>::Normalize(T aRadius) &
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

	template<typename T>
	constexpr Vector2<T> Vector2<T>::Reflect(const Vector2& aVector) const
	{
		return aVector - T{2} * ProjectOnto(aVector);
	}

	template<typename T>
	constexpr Vector3<T> Vector2<T>::XYZ(T aZ) const
	{
		return Vector3<T>(x, y, aZ);
	}
	template<typename T>
	constexpr Vector3<T> Vector2<T>::XZY(T aY) const
	{
		return Vector3<T>(x, aY, y);
	}
	template<typename T>
	constexpr Vector3<T> Vector2<T>::YZX(T aX) const
	{
		return Vector3<T>(aX, x, y);
	}
	template<typename T>
	constexpr Vector4<T> Vector2<T>::XYZW(T aZ, T aW) const
	{
		return Vector4<T>(x, y, aZ, aW);
	}

	template<typename T>
	constexpr Vector2<T> Vector2<T>::Direction(const Vector2& aCurrent, const Vector2& aTarget)
	{
		return Vector2<T>(aTarget.x - aCurrent.x, aTarget.y - aCurrent.y);
	}

	template<typename T>
	constexpr T Vector2<T>::Distance(const Vector2& aCurrent, const Vector2& aTarget)
	{
		return Direction(aCurrent, aTarget).Length();
	}

	template<typename T>
	constexpr T Vector2<T>::DistanceSqr(const Vector2& aCurrent, const Vector2& aTarget)
	{
		return Direction(aCurrent, aTarget).LengthSqr();
	}

	template<typename T>
	constexpr Vector2<T> Vector2<T>::Lerp(const Vector2& aCurrent, const Vector2& aTarget, T aPercentage)
	{
		const auto Lerp = [](T aStart, T aEnd, T aPerc) { return aStart + aPerc * (aEnd - aStart); };

		return Vector2<T>
		{
			Lerp(aCurrent.x, aTarget.x, aPercentage),
			Lerp(aCurrent.y, aTarget.y, aPercentage)
		};
	}

	template<typename T>
	constexpr Vector2<T> Vector2<T>::CLerp(const Vector2& aCurrent, const Vector2& aTarget, T aPercentage)
	{
		const auto Clamp = [](T aValue, T aMin, T aMax) { return (aValue < aMin) ? aMin : ((aValue > aMax) ? aMax : aValue); };
		const auto Lerp = [](T aStart, T aEnd, T aPerc) { return aStart + aPerc * (aEnd - aStart); };

		return Vector2<T>
		{
			Clamp(Lerp(aCurrent.x, aTarget.x, aPercentage), (std::min)(aCurrent.x, aTarget.x), (std::max)(aCurrent.x, aTarget.x)),
			Clamp(Lerp(aCurrent.y, aTarget.y, aPercentage), (std::min)(aCurrent.y, aTarget.y), (std::max)(aCurrent.y, aTarget.y))
		};
	}

	template<typename T>
	constexpr Vector2<T> Vector2<T>::MoveTowards(const Vector2& aCurrent, const Vector2& aTarget, T aDistance)
	{
		if (Vector2<T> dir = Vector2<T>::Direction(aCurrent, aTarget); dir != Vector2<T>::Zero)
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
	constexpr bool Vector2<T>::Equal(const Vector2& aLeft, const Vector2& aRight, T aTolerance)
	{
		return	au::Equal(aLeft.x, aRight.x, aTolerance) &&
				au::Equal(aLeft.y, aRight.y, aTolerance);
	}

	// GLOBAL OPERATORS

	template<typename T>
	NODISC constexpr Vector2<T> operator-(const Vector2<T>& aVector)
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
	NODISC constexpr Vector2<T> operator+(const Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		return Vector2<T>(aLeft.x + aRight.x, aLeft.y + aRight.y);
	}
	template<typename T>
	NODISC constexpr Vector2<T> operator-(const Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		return Vector2<T>(aLeft.x - aRight.x, aLeft.y - aRight.y);
	}

	template<typename T>
	NODISC constexpr Vector2<T> operator*(const Vector2<T>& aLeft, T aRight)
	{
		return Vector2<T>(aLeft.x * aRight, aLeft.y * aRight);
	}
	template<typename T>
	NODISC constexpr Vector2<T> operator/(const Vector2<T>& aLeft, T aRight)
	{
		assert(aRight != 0 && "Cannot divide by zero");
		return Vector2<T>(aLeft.x / aRight, aLeft.y / aRight);
	}

	template<typename T>
	NODISC constexpr Vector2<T> operator*(const Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		return Vector2<T>(aLeft.x * aRight.x, aLeft.y * aRight.y);
	}
	template<typename T>
	NODISC constexpr Vector2<T> operator*(T aLeft, const Vector2<T>& aRight)
	{
		return aRight * aLeft;
	}

	template<typename T>
	NODISC constexpr Vector2<T> operator/(const Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && "Cannot divide by zero");
		return Vector2<T>(aLeft.x / aRight.x, aLeft.y / aRight.y);
	}
	template<typename T>
	NODISC constexpr Vector2<T> operator/(T aLeft, const Vector2<T>& aRight)
	{
		assert(aRight.x != 0 && aRight.y != 0 && "Cannot divide by zero");
		return Vector2<T>(aLeft / aRight.x, aLeft / aRight.y);
	}

	template<typename T>
	NODISC constexpr bool operator==(const Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		return (aLeft.x == aRight.x) && (aLeft.y == aRight.y);
	}
	template<typename T>
	NODISC constexpr bool operator!=(const Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		return !(aLeft == aRight);
	}

	// using declarations

	using Vector2f = Vector2<float>;
	using Vector2d = Vector2<double>;
	using Vector2i = Vector2<int>;
	using Vector2u = Vector2<unsigned>;
}
#pragma once

#include <array>
#include <cmath>
#include <cassert>
#include <stdexcept>

#include "Vector2.hpp"
#include "Vector3.hpp"

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T>
	class Matrix4x4;

	template<typename T>
	class Matrix3x3
	{
	public:
		CONSTEXPR Matrix3x3();
		CONSTEXPR ~Matrix3x3();

		CONSTEXPR Matrix3x3(
			T a00, T a10, T a20,
			T a01, T a11, T a21,
			T a02, T a12, T a22);

		CONSTEXPR Matrix3x3(const Matrix4x4<T>& aMatrix);

		NODISC CONSTEXPR T& operator[](int aIndex);
		NODISC CONSTEXPR const T& operator[](int aIndex) const;

		NODISC CONSTEXPR T& operator()(int aRow, int aColumn);
		NODISC CONSTEXPR const T& operator()(int aRow, int aColumn) const;

		NODISC CONSTEXPR const T* GetData() const noexcept;

		NODISC CONSTEXPR auto GetTranspose() const -> Matrix3x3;

		NODISC CONSTEXPR Vector2<T> GetTranslation() const;
		NODISC CONSTEXPR T GetRotation() const;
		NODISC CONSTEXPR Vector2<T> GetScale() const;

		NODISC CONSTEXPR Vector2<T> GetUp() const;
		NODISC CONSTEXPR Vector2<T> GetRight() const;

		CONSTEXPR void SetTranslation(const Vector2<T>& aTranslation);
		CONSTEXPR void SetRotation(T aRotation);
		CONSTEXPR void SetScale(const Vector2<T>& aScale);

		NODISC CONSTEXPR auto Inverse() const -> Matrix3x3;
		NODISC CONSTEXPR auto FastInverse() const -> Matrix3x3;

		CONSTEXPR auto Translate(const Vector2<T>& aTranslation) -> Matrix3x3&;
		CONSTEXPR auto Scale(const Vector2<T>& someFactors) -> Matrix3x3&;
		CONSTEXPR auto Rotate(T aRotation) -> Matrix3x3&;

		NODISC CONSTEXPR Vector2<T> TransformPoint(const Vector2<T>& aPoint) const;
		NODISC CONSTEXPR Vector3<T> TransformPoint(const Vector3<T>& aPoint) const;

		CONSTEXPR auto Add(const Matrix3x3& aRight) -> Matrix3x3&;
		CONSTEXPR auto Subtract(const Matrix3x3& aRight) -> Matrix3x3&;
		CONSTEXPR auto Combine(const Matrix3x3& aRight) -> Matrix3x3&;

		NODISC CONSTEXPR static auto CreateTRS(const Vector2<T>& aPosition, T aRotation, const Vector2<T>& aScale, const Vector2<T>& aOrigin = Vector2<T>()) -> Matrix3x3;

		NODISC CONSTEXPR static auto CreateRotationAroundX(T aRadians) -> Matrix3x3;
		NODISC CONSTEXPR static auto CreateRotationAroundY(T aRadians) -> Matrix3x3;
		NODISC CONSTEXPR static auto CreateRotationAroundZ(T aRadians) -> Matrix3x3;

		static CONSTEXPR int ROWS		= 3;
		static CONSTEXPR int COLUMNS	= 3;
		static CONSTEXPR int COUNT		= ROWS * COLUMNS;

		static const Matrix3x3 IDENTITY;

	private:
		std::array<T, COUNT> myMatrix
		{ 
			1, 0, 0,
			0, 1, 0,
			0, 0, 1 
		};
	};


	template<typename T>
	CONSTEXPR Matrix3x3<T>::Matrix3x3() = default;

	template<typename T>
	CONSTEXPR Matrix3x3<T>::~Matrix3x3() = default;

	template<typename T>
	CONSTEXPR Matrix3x3<T>::Matrix3x3(
		T a00, T a10, T a20, 
		T a01, T a11, T a21, 
		T a02, T a12, T a22)
		: myMatrix{ a00, a10, a20, 
					a01, a11, a21, 
					a02, a12, a22 }
	{

	}

	template<typename T>
	CONSTEXPR Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& aMatrix)
		: myMatrix{ 
			aMatrix[0], aMatrix[1], aMatrix[2],
			aMatrix[4], aMatrix[5], aMatrix[6],
			aMatrix[8], aMatrix[9], aMatrix[10] }
	{

	}

	template<typename T>
	CONSTEXPR T& Matrix3x3<T>::operator[](int aIndex)
	{
		return myMatrix[aIndex];
	}
	template<typename T>
	CONSTEXPR const T& Matrix3x3<T>::operator[](int aIndex) const
	{
		return myMatrix[aIndex];
	}

	template<typename T>
	CONSTEXPR T& Matrix3x3<T>::operator()(int aRow, int aColumn)
	{
		return myMatrix[(aColumn - 1) + (aRow - 1) * COLUMNS];
	}
	template<typename T>
	CONSTEXPR const T& Matrix3x3<T>::operator()(int aRow, int aColumn) const
	{
		return myMatrix[(aColumn - 1) + (aRow - 1) * COLUMNS];
	}

	template<typename T>
	CONSTEXPR const T* Matrix3x3<T>::GetData() const noexcept
	{
		return myMatrix.data();
	}

	template<typename T>
	CONSTEXPR auto Matrix3x3<T>::GetTranspose() const -> Matrix3x3
	{
		return Matrix3x3<T> 
		{ 
			myMatrix[0], myMatrix[3], myMatrix[6],
			myMatrix[1], myMatrix[4], myMatrix[7],
			myMatrix[2], myMatrix[5], myMatrix[8] 
		};
	}

	template<typename T>
	CONSTEXPR Vector2<T> Matrix3x3<T>::GetTranslation() const
	{
		return Vector2<T>{ myMatrix[6], myMatrix[7] };
	}
	template<typename T>
	CONSTEXPR T Matrix3x3<T>::GetRotation() const
	{
		return std::atan2f(myMatrix[1], myMatrix[0]);
	}
	template<typename T>
	CONSTEXPR Vector2<T> Matrix3x3<T>::GetScale() const
	{
		return Vector2<T>
		{
			Vector2<T>{ myMatrix[0], myMatrix[1] }.Length(),
			Vector2<T>{ myMatrix[3], myMatrix[4] }.Length()
		};
	}

	template<typename T>
	CONSTEXPR Vector2<T> Matrix3x3<T>::GetUp() const
	{
		return Vector2<T>{ myMatrix[3], myMatrix[4] };
	}
	template<typename T>
	CONSTEXPR Vector2<T> Matrix3x3<T>::GetRight() const
	{
		return Vector2<T>{ myMatrix[0], myMatrix[1] };
	}

	template<typename T>
	CONSTEXPR void Matrix3x3<T>::SetTranslation(const Vector2<T>& aTranslation)
	{
		myMatrix[6] = aTranslation.x;
		myMatrix[7] = aTranslation.y;
	}
	template<typename T>
	CONSTEXPR void Matrix3x3<T>::SetRotation(T aRotation)
	{
		const Vector2<T> scale = GetScale();
		
		const T c = std::cos(aRotation);
		const T s = std::sin(aRotation);
		const T sxc = scale.x * c;
		const T syc = scale.y * c;
		const T sxs = scale.x * s;
		const T sys = scale.y * s;

		myMatrix[0] =  sxc; myMatrix[1] = sxs;
		myMatrix[3] = -sys; myMatrix[4] = syc;
	}
	template<typename T>
	CONSTEXPR void Matrix3x3<T>::SetScale(const Vector2<T>& aScale)
	{
		Vector2<T> scaleX{ myMatrix[0], myMatrix[1] };
		Vector2<T> scaleY{ myMatrix[3], myMatrix[4] };

		scaleX.Normalize(aScale.x);
		scaleY.Normalize(aScale.y);

		myMatrix[0] = scaleX.x; myMatrix[1] = scaleX.y;
		myMatrix[3] = scaleY.x; myMatrix[4] = scaleY.y;
	}

	template<typename T>
	CONSTEXPR auto Matrix3x3<T>::Inverse() const -> Matrix3x3
	{
		throw std::runtime_error("Not yet implemented");
		return Matrix3x3();
	}
	template<typename T>
	CONSTEXPR auto Matrix3x3<T>::FastInverse() const -> Matrix3x3
	{
		const Vector2<T> s = GetScale();
		assert(s.x != 0 && s.y != 0 && "Cannot divide by zero");

		const Vector2<T> is = static_cast<T>(1) / s;
		const Vector2<T> ip	= -GetTranslation();

		Matrix3x3 inverseMatrix
		{
			myMatrix[0],	myMatrix[3],	0,
			myMatrix[1],	myMatrix[4],	0,
			0,				0,				1
		};

		inverseMatrix.SetTranslation(inverseMatrix.TransformPoint(ip));

		const Matrix3x3 scalingInverse =
		{
			is.x * is.x,	0,				0,
			0,				is.y * is.y,	0,
			0,				0,				1
		};

		return inverseMatrix.Combine(scalingInverse);
	}

	template<typename T>
	CONSTEXPR auto Matrix3x3<T>::Translate(const Vector2<T>& aTranslation) -> Matrix3x3&
	{
		myMatrix[6] += aTranslation.x;
		myMatrix[7] += aTranslation.y;

		return *this;
	}
	template<typename T>
	CONSTEXPR auto Matrix3x3<T>::Scale(const Vector2<T>& someFactors) -> Matrix3x3&
	{
		const Matrix3x3<T> scaling
		{
			someFactors.x,	0,				0,
			0,				someFactors.y,	0,
			0,				0,				1,
		};

		return Combine(scaling);
	}
	template<typename T>
	CONSTEXPR auto Matrix3x3<T>::Rotate(T aRotation) -> Matrix3x3&
	{
		return Combine(CreateRotationAroundZ(aRotation));
	}

	template<typename T>
	CONSTEXPR Vector2<T> Matrix3x3<T>::TransformPoint(const Vector2<T>& aPoint) const
	{
		return Vector2<T>
		{
			aPoint.x * myMatrix[0] + aPoint.y * myMatrix[3] + myMatrix[6],
			aPoint.x * myMatrix[1] + aPoint.y * myMatrix[4] + myMatrix[7]
		};
	}
	template<typename T>
	CONSTEXPR Vector3<T> Matrix3x3<T>::TransformPoint(const Vector3<T>& aPoint) const
	{
		return Vector3<T>
		{ 
			aPoint.x * myMatrix[0] + aPoint.y * myMatrix[3] + aPoint.z * myMatrix[6],
			aPoint.x * myMatrix[1] + aPoint.y * myMatrix[4] + aPoint.z * myMatrix[7],
			aPoint.x * myMatrix[2] + aPoint.y * myMatrix[5] + aPoint.z * myMatrix[8] 
		};
	}

	template<typename T>
	CONSTEXPR auto Matrix3x3<T>::Add(const Matrix3x3& aRight) -> Matrix3x3&
	{
		for (int i = 0; i < COUNT; ++i)
		{
			myMatrix[i] += aRight[i];
		}

		return *this;
	}

	template<typename T>
	CONSTEXPR auto Matrix3x3<T>::Subtract(const Matrix3x3& aRight) -> Matrix3x3&
	{
		for (int i = 0; i < COUNT; ++i)
		{
			myMatrix[i] -= aRight[i];
		}

		return *this;
	}

	template<typename T>
	CONSTEXPR auto Matrix3x3<T>::Combine(const Matrix3x3& aRight) -> Matrix3x3&
	{
		const T* a = myMatrix.data();
		const T* b = aRight.myMatrix.data();

		*this = Matrix3x3
		{
			(a[0] * b[0] + a[1] * b[3] + a[2] * b[6]), (a[0] * b[1] + a[1] * b[4] + a[2] * b[7]), (a[0] * b[2] + a[1] * b[5] + a[2] * b[8]),
			(a[3] * b[0] + a[4] * b[3] + a[5] * b[6]), (a[3] * b[1] + a[4] * b[4] + a[5] * b[7]), (a[3] * b[2] + a[4] * b[5] + a[5] * b[8]),
			(a[6] * b[0] + a[7] * b[3] + a[8] * b[6]), (a[6] * b[1] + a[7] * b[4] + a[8] * b[7]), (a[6] * b[2] + a[7] * b[5] + a[8] * b[8])
		};

		return *this;
	}

	template<typename T>
	CONSTEXPR auto Matrix3x3<T>::CreateTRS(const Vector2<T>& aPosition, T aRotation, const Vector2<T>& aScale, const Vector2<T>& aOrigin) -> Matrix3x3
	{
		const T c	= std::cos(aRotation);
		const T s	= std::sin(aRotation);
		const T sxc = aScale.x * c;
		const T syc = aScale.y * c;
		const T sxs = aScale.x * s;
		const T sys = aScale.y * s;
		const T tx	= -aOrigin.x * sxc - aOrigin.y * sys + aPosition.x;
		const T ty	= aOrigin.x * sxs - aOrigin.y * syc + aPosition.y;

		return Matrix3x3
		{
			 sxc,	sxs,	0,
			-sys,	syc,	0,
			 tx,	ty,		1
		};
	}

	template<typename T>
	CONSTEXPR auto Matrix3x3<T>::CreateRotationAroundX(T aRadians) -> Matrix3x3
	{
		const T c = std::cos(aRadians);
		const T s = std::sin(aRadians);

		return Matrix3x3<T> 
		{
			1,  0,  0,
			0,  c,  s,
			0, -s,  c
		};
	}
	template<typename T>
	CONSTEXPR auto Matrix3x3<T>::CreateRotationAroundY(T aRadians) -> Matrix3x3
	{
		const T c = std::cos(aRadians);
		const T s = std::sin(aRadians);

		return Matrix3x3<T> 
		{
			c,  0, -s,
			0,  1,  0,
			s,  0,  c
		};
	}
	template<typename T>
	CONSTEXPR auto Matrix3x3<T>::CreateRotationAroundZ(T aRadians) -> Matrix3x3
	{
		const T c = std::cos(aRadians);
		const T s = std::sin(aRadians);

		return Matrix3x3<T> 
		{
			 c,  s,  0,
			-s,  c,  0,
			 0,  0,  1
		};
	}

	// GLOBAL OPERATORS

	template<typename T>
	NODISC CONSTEXPR Matrix3x3<T> operator+(const Matrix3x3<T>& aLeft, const Matrix3x3<T>& aRight)
	{
		return Matrix3x3<T>(aLeft).Add(aRight);
	}
	template<typename T>
	NODISC CONSTEXPR Matrix3x3<T> operator-(const Matrix3x3<T>& aLeft, const Matrix3x3<T>& aRight)
	{
		return Matrix3x3<T>(aLeft).Subtract(aRight);
	}
	template<typename T>
	NODISC CONSTEXPR Matrix3x3<T> operator*(const Matrix3x3<T>& aLeft, const Matrix3x3<T>& aRight)
	{
		return Matrix3x3<T>(aLeft).Combine(aRight);
	}

	template<typename T>
	CONSTEXPR Matrix3x3<T>& operator+=(Matrix3x3<T>& aLeft, const Matrix3x3<T>& aRight)
	{
		return aLeft.Add(aRight);
	}
	template<typename T>
	CONSTEXPR Matrix3x3<T>& operator-=(Matrix3x3<T>& aLeft, const Matrix3x3<T>& aRight)
	{
		return aLeft.Subtract(aRight);
	}
	template<typename T>
	CONSTEXPR Matrix3x3<T>& operator*=(Matrix3x3<T>& aLeft, const Matrix3x3<T>& aRight)
	{
		return aLeft.Combine(aRight);
	}

	template<typename T>
	NODISC CONSTEXPR Vector3<T> operator*(const Matrix3x3<T>& aLeft, const Vector3<T>& aRight)
	{
		return aLeft.TransformPoint(aRight);
	}
	template<typename T>
	NODISC CONSTEXPR Vector3<T> operator*(const Vector3<T>& aLeft, const Matrix3x3<T>& aRight)
	{
		return aRight * aLeft;
	}

	template<typename T>
	NODISC CONSTEXPR Vector2<T> operator*(const Matrix3x3<T>& aLeft, const Vector2<T>& aRight)
	{
		return aLeft.TransformPoint(aRight);
	}
	template<typename T>
	NODISC CONSTEXPR Vector2<T> operator*(const Vector2<T>& aLeft, const Matrix3x3<T>& aRight)
	{
		return aRight * aLeft;
	}

	template<typename T>
	NODISC CONSTEXPR bool operator==(const Matrix3x3<T>& aLeft, const Matrix3x3<T>& aRight)
	{
		for (int i = 0; i < Matrix3x3<T>::COUNT; ++i)
		{
			if (aLeft[i] != aRight[i])
			{
				return false;
			}
		}

		return true;
	}
	template<typename T>
	NODISC CONSTEXPR bool operator!=(const Matrix3x3<T>& aLeft, const Matrix3x3<T>& aRight)
	{
		return !(aLeft == aRight);
	}

	template<typename T>
	inline const Matrix3x3<T> Matrix3x3<T>::IDENTITY;

	// using declarations

	using Mat3f = Matrix3x3<float>;
	using Mat3d = Matrix3x3<double>;
	using Mat3i = Matrix3x3<int>;
	using Mat3u = Matrix3x3<unsigned int>;
}
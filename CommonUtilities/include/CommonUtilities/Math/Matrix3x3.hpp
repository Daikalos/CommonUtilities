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
		static constexpr int ROWS		= 3;
		static constexpr int COLUMNS	= 3;
		static constexpr int COUNT		= ROWS * COLUMNS;

		constexpr Matrix3x3() = default;
		constexpr ~Matrix3x3() = default;

		constexpr Matrix3x3(
			T a00, T a10, T a20,
			T a01, T a11, T a21,
			T a02, T a12, T a22);

		constexpr Matrix3x3(const std::array<T, COUNT>& aArray);

		constexpr Matrix3x3(const T(&aArray)[COUNT]);

		constexpr Matrix3x3(const Matrix4x4<T>& aMatrix);

		NODISC constexpr T& operator[](int aIndex);
		NODISC constexpr const T& operator[](int aIndex) const;

		NODISC constexpr T& operator()(int aRow, int aColumn);
		NODISC constexpr const T& operator()(int aRow, int aColumn) const;

		NODISC constexpr const T* GetData() const noexcept;
		NODISC constexpr T* GetData() noexcept;

		NODISC constexpr auto GetTranspose() const -> Matrix3x3;
		NODISC constexpr auto GetRotationMatrix() const -> Matrix3x3;

		NODISC constexpr Vector2<T> GetTranslation() const;
		NODISC constexpr T GetRotation() const;
		NODISC constexpr Vector2<T> GetScale() const;

		NODISC constexpr Vector2<T> GetUp() const;
		NODISC constexpr Vector2<T> GetRight() const;

		constexpr void SetTranslation(const Vector2<T>& aTranslation);
		constexpr void SetRotation(T aRotation);
		constexpr void SetScale(const Vector2<T>& aScale);

		NODISC constexpr auto GetInverse() const -> Matrix3x3;
		NODISC constexpr auto GetFastInverse() const -> Matrix3x3;

		constexpr auto Translate(const Vector2<T>& aTranslation) -> Matrix3x3&;
		constexpr auto Scale(const Vector2<T>& someFactors) -> Matrix3x3&;
		constexpr auto Rotate(T aRotation) -> Matrix3x3&;

		NODISC constexpr Vector2<T> TransformPoint(const Vector2<T>& aPoint) const;
		NODISC constexpr Vector3<T> TransformPoint(const Vector3<T>& aPoint) const;

		constexpr auto Add(const Matrix3x3& aRight) -> Matrix3x3&;
		constexpr auto Subtract(const Matrix3x3& aRight) -> Matrix3x3&;
		constexpr auto Combine(const Matrix3x3& aRight) -> Matrix3x3&;

		NODISC constexpr static auto CreateTRS(
			const Vector2<T>& aPosition, 
			T aRotation, 
			const Vector2<T>& aScale, 
			const Vector2<T>& aScaleMultiplier = Vector2<T>(1, 1),
			const Vector2<T>& aOrigin = Vector2<T>()) -> Matrix3x3;

		NODISC constexpr static auto CreateRotationAroundX(T aRadians) -> Matrix3x3;
		NODISC constexpr static auto CreateRotationAroundY(T aRadians) -> Matrix3x3;
		NODISC constexpr static auto CreateRotationAroundZ(T aRadians) -> Matrix3x3;

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
	constexpr Matrix3x3<T>::Matrix3x3(
		T a00, T a10, T a20, 
		T a01, T a11, T a21, 
		T a02, T a12, T a22)
		: myMatrix{ a00, a10, a20, 
					a01, a11, a21, 
					a02, a12, a22 }
	{

	}

	template<typename T>
	constexpr Matrix3x3<T>::Matrix3x3(const std::array<T, COUNT>& aArray)
		: myMatrix{ aArray }
	{

	}

	template<typename T>
	constexpr Matrix3x3<T>::Matrix3x3(const T(&aArray)[COUNT])
		: myMatrix{ std::to_array(aArray) }
	{

	}

	template<typename T>
	constexpr Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& aMatrix)
		: myMatrix{ 
			aMatrix[0], aMatrix[1], aMatrix[2],
			aMatrix[4], aMatrix[5], aMatrix[6],
			aMatrix[8], aMatrix[9], aMatrix[10] }
	{

	}

	template<typename T>
	constexpr T& Matrix3x3<T>::operator[](int aIndex)
	{
		return myMatrix[aIndex];
	}
	template<typename T>
	constexpr const T& Matrix3x3<T>::operator[](int aIndex) const
	{
		return myMatrix[aIndex];
	}

	template<typename T>
	constexpr T& Matrix3x3<T>::operator()(int aRow, int aColumn)
	{
		return myMatrix[(aColumn - 1) + (aRow - 1) * COLUMNS];
	}
	template<typename T>
	constexpr const T& Matrix3x3<T>::operator()(int aRow, int aColumn) const
	{
		return myMatrix[(aColumn - 1) + (aRow - 1) * COLUMNS];
	}

	template<typename T>
	constexpr const T* Matrix3x3<T>::GetData() const noexcept
	{
		return myMatrix.data();
	}
	template<typename T>
	constexpr T* Matrix3x3<T>::GetData() noexcept
	{
		return myMatrix.data();
	}

	template<typename T>
	constexpr auto Matrix3x3<T>::GetTranspose() const -> Matrix3x3
	{
		return Matrix3x3<T> 
		{ 
			myMatrix[0], myMatrix[3], myMatrix[6],
			myMatrix[1], myMatrix[4], myMatrix[7],
			myMatrix[2], myMatrix[5], myMatrix[8] 
		};
	}
	template<typename T>
	constexpr auto Matrix3x3<T>::GetRotationMatrix() const -> Matrix3x3
	{
		Matrix3x3<T> rotationMatrix
		{
			myMatrix[0],	myMatrix[3],	0,
			myMatrix[1],	myMatrix[4],	0,
			0,				0,				1
		};

		rotationMatrix.SetScale(Vector3f(1.0f, 1.0f, 1.0f)); // remove scaling

		return rotationMatrix;
	}

	template<typename T>
	constexpr Vector2<T> Matrix3x3<T>::GetTranslation() const
	{
		return Vector2<T>{ myMatrix[6], myMatrix[7] };
	}
	template<typename T>
	constexpr T Matrix3x3<T>::GetRotation() const
	{
		return std::atan2f(myMatrix[1], myMatrix[0]);
	}
	template<typename T>
	constexpr Vector2<T> Matrix3x3<T>::GetScale() const
	{
		return Vector2<T>
		{
			Vector2<T>{ myMatrix[0], myMatrix[1] }.Length(),
			Vector2<T>{ myMatrix[3], myMatrix[4] }.Length()
		};
	}

	template<typename T>
	constexpr Vector2<T> Matrix3x3<T>::GetUp() const
	{
		return Vector2<T>{ myMatrix[3], myMatrix[4] };
	}
	template<typename T>
	constexpr Vector2<T> Matrix3x3<T>::GetRight() const
	{
		return Vector2<T>{ myMatrix[0], myMatrix[1] };
	}

	template<typename T>
	constexpr void Matrix3x3<T>::SetTranslation(const Vector2<T>& aTranslation)
	{
		myMatrix[6] = aTranslation.x;
		myMatrix[7] = aTranslation.y;
	}
	template<typename T>
	constexpr void Matrix3x3<T>::SetRotation(T aRotation)
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
	constexpr void Matrix3x3<T>::SetScale(const Vector2<T>& aScale)
	{
		Vector2<T> scaleX{ myMatrix[0], myMatrix[1] };
		Vector2<T> scaleY{ myMatrix[3], myMatrix[4] };

		scaleX.Normalize(aScale.x);
		scaleY.Normalize(aScale.y);

		myMatrix[0] = scaleX.x; myMatrix[1] = scaleX.y;
		myMatrix[3] = scaleY.x; myMatrix[4] = scaleY.y;
	}

	template<typename T>
	constexpr auto Matrix3x3<T>::GetInverse() const -> Matrix3x3
	{
		const T* m = myMatrix.data();

		T det = m[0] * (m[4] * m[8] - m[5] * m[7]) - 
				m[1] * (m[3] * m[8] - m[5] * m[6]) + 
				m[2] * (m[3] * m[7] - m[4] * m[6]);

		return (det != T(0)) ? Matrix3x3
		{
		    (m[4] * m[8] - m[5] * m[7]) / det,
		   -(m[1] * m[8] - m[2] * m[7]) / det,
		    (m[1] * m[5] - m[2] * m[4]) / det,
		   -(m[3] * m[8] - m[5] * m[6]) / det,
		    (m[0] * m[8] - m[2] * m[6]) / det,
		   -(m[0] * m[5] - m[2] * m[3]) / det,
		    (m[3] * m[7] - m[4] * m[6]) / det,
		   -(m[0] * m[7] - m[1] * m[6]) / det,
		    (m[0] * m[4] - m[1] * m[3]) / det,
		} ? Matrix3x3{};
	}
	template<typename T>
	constexpr auto Matrix3x3<T>::GetFastInverse() const -> Matrix3x3
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
	constexpr auto Matrix3x3<T>::Translate(const Vector2<T>& aTranslation) -> Matrix3x3&
	{
		myMatrix[6] += aTranslation.x;
		myMatrix[7] += aTranslation.y;

		return *this;
	}
	template<typename T>
	constexpr auto Matrix3x3<T>::Scale(const Vector2<T>& someFactors) -> Matrix3x3&
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
	constexpr auto Matrix3x3<T>::Rotate(T aRotation) -> Matrix3x3&
	{
		return Combine(CreateRotationAroundZ(aRotation));
	}

	template<typename T>
	constexpr Vector2<T> Matrix3x3<T>::TransformPoint(const Vector2<T>& aPoint) const
	{
		return Vector2<T>
		{
			aPoint.x * myMatrix[0] + aPoint.y * myMatrix[3] + myMatrix[6],
			aPoint.x * myMatrix[1] + aPoint.y * myMatrix[4] + myMatrix[7]
		};
	}
	template<typename T>
	constexpr Vector3<T> Matrix3x3<T>::TransformPoint(const Vector3<T>& aPoint) const
	{
		return Vector3<T>
		{ 
			aPoint.x * myMatrix[0] + aPoint.y * myMatrix[3] + aPoint.z * myMatrix[6],
			aPoint.x * myMatrix[1] + aPoint.y * myMatrix[4] + aPoint.z * myMatrix[7],
			aPoint.x * myMatrix[2] + aPoint.y * myMatrix[5] + aPoint.z * myMatrix[8] 
		};
	}

	template<typename T>
	constexpr auto Matrix3x3<T>::Add(const Matrix3x3& aRight) -> Matrix3x3&
	{
		for (int i = 0; i < COUNT; ++i)
		{
			myMatrix[i] += aRight[i];
		}

		return *this;
	}

	template<typename T>
	constexpr auto Matrix3x3<T>::Subtract(const Matrix3x3& aRight) -> Matrix3x3&
	{
		for (int i = 0; i < COUNT; ++i)
		{
			myMatrix[i] -= aRight[i];
		}

		return *this;
	}

	template<typename T>
	constexpr auto Matrix3x3<T>::Combine(const Matrix3x3& aRight) -> Matrix3x3&
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
	constexpr auto Matrix3x3<T>::CreateTRS(
		const Vector2<T>& aPosition, 
		T aRotation, 
		const Vector2<T>& aScale, 
		const Vector2<T>& aScaleMultiplier, 
		const Vector2<T>& aOrigin) -> Matrix3x3
	{
		const T c	= std::cos(aRotation);
		const T s	= std::sin(aRotation);
		const T sxc = aScale.x * aScaleMultiplier.x * c;
		const T syc = aScale.y * aScaleMultiplier.y * c;
		const T sxs = aScale.x * aScaleMultiplier.x * s;
		const T sys = aScale.y * aScaleMultiplier.y * s;
		const T tx	= -aOrigin.x * sxc - aOrigin.y * sys + aPosition.x;
		const T ty	= -aOrigin.x * sxs + aOrigin.y * syc + aPosition.y;

		return Matrix3x3
		{
			 sxc,	sxs,	0,
			-sys,	syc,	0,
			 tx,	ty,		1
		};
	}

	template<typename T>
	constexpr auto Matrix3x3<T>::CreateRotationAroundX(T aRadians) -> Matrix3x3
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
	constexpr auto Matrix3x3<T>::CreateRotationAroundY(T aRadians) -> Matrix3x3
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
	constexpr auto Matrix3x3<T>::CreateRotationAroundZ(T aRadians) -> Matrix3x3
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
	NODISC constexpr Matrix3x3<T> operator+(const Matrix3x3<T>& aLeft, const Matrix3x3<T>& aRight)
	{
		return Matrix3x3<T>(aLeft).Add(aRight);
	}
	template<typename T>
	NODISC constexpr Matrix3x3<T> operator-(const Matrix3x3<T>& aLeft, const Matrix3x3<T>& aRight)
	{
		return Matrix3x3<T>(aLeft).Subtract(aRight);
	}
	template<typename T>
	NODISC constexpr Matrix3x3<T> operator*(const Matrix3x3<T>& aLeft, const Matrix3x3<T>& aRight)
	{
		return Matrix3x3<T>(aLeft).Combine(aRight);
	}

	template<typename T>
	constexpr Matrix3x3<T>& operator+=(Matrix3x3<T>& aLeft, const Matrix3x3<T>& aRight)
	{
		return aLeft.Add(aRight);
	}
	template<typename T>
	constexpr Matrix3x3<T>& operator-=(Matrix3x3<T>& aLeft, const Matrix3x3<T>& aRight)
	{
		return aLeft.Subtract(aRight);
	}
	template<typename T>
	constexpr Matrix3x3<T>& operator*=(Matrix3x3<T>& aLeft, const Matrix3x3<T>& aRight)
	{
		return aLeft.Combine(aRight);
	}

	template<typename T>
	NODISC constexpr Vector3<T> operator*(const Matrix3x3<T>& aLeft, const Vector3<T>& aRight)
	{
		return aLeft.TransformPoint(aRight);
	}
	template<typename T>
	NODISC constexpr Vector3<T> operator*(const Vector3<T>& aLeft, const Matrix3x3<T>& aRight)
	{
		return aRight * aLeft;
	}

	template<typename T>
	NODISC constexpr Vector2<T> operator*(const Matrix3x3<T>& aLeft, const Vector2<T>& aRight)
	{
		return aLeft.TransformPoint(aRight);
	}
	template<typename T>
	NODISC constexpr Vector2<T> operator*(const Vector2<T>& aLeft, const Matrix3x3<T>& aRight)
	{
		return aRight * aLeft;
	}

	template<typename T>
	NODISC constexpr bool operator==(const Matrix3x3<T>& aLeft, const Matrix3x3<T>& aRight)
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
	NODISC constexpr bool operator!=(const Matrix3x3<T>& aLeft, const Matrix3x3<T>& aRight)
	{
		return !(aLeft == aRight);
	}

	template<typename T>
	inline const Matrix3x3<T> Matrix3x3<T>::IDENTITY;

	// using declarations

	using Mat3f = Matrix3x3<float>;
	using Mat3d = Matrix3x3<double>;
	using Mat3i = Matrix3x3<int>;
	using Mat3u = Matrix3x3<unsigned>;
}
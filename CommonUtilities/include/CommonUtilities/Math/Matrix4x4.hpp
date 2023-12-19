#pragma once

#include <array>
#include <cmath>

#include "Vector3.hpp"
#include "Vector4.hpp"

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T>
	class Matrix4x4
	{
	public:
		CONSTEXPR Matrix4x4();
		CONSTEXPR ~Matrix4x4();

		CONSTEXPR Matrix4x4(
			T a00, T a10, T a20, T a30,
			T a01, T a11, T a21, T a31,
			T a02, T a12, T a22, T a32,
			T a03, T a13, T a23, T a33);

		NODISC CONSTEXPR T& operator[](int aIndex);
		NODISC CONSTEXPR const T& operator[](int aIndex) const;

		NODISC CONSTEXPR T& operator()(int aRow, int aColumn);
		NODISC CONSTEXPR const T& operator()(int aRow, int aColumn) const;

		NODISC CONSTEXPR const T* GetData() const noexcept;

		NODISC CONSTEXPR auto GetTranspose() const -> Matrix4x4;

		NODISC CONSTEXPR Vector3<T> GetTranslation() const;
		NODISC CONSTEXPR Vector3<T> GetRotation() const;
		NODISC CONSTEXPR Vector3<T> GetScale() const;

		NODISC CONSTEXPR Vector3<T> GetForward() const;
		NODISC CONSTEXPR Vector3<T> GetUp() const;
		NODISC CONSTEXPR Vector3<T> GetRight() const;

		CONSTEXPR void SetTranslation(const Vector3<T>& aTranslation);
		CONSTEXPR void SetRotation(const Vector3<T>& aRotation);
		CONSTEXPR void SetScale(const Vector3<T>& aScale);

		NODISC CONSTEXPR auto Inverse() const -> Matrix4x4;
		NODISC CONSTEXPR auto FastInverse() const -> Matrix4x4;

		CONSTEXPR auto Translate(const Vector3<T>& aTranslation) -> Matrix4x4&;
		CONSTEXPR auto Scale(const Vector3<T>& someFactors) -> Matrix4x4&;
		CONSTEXPR auto Rotate(T aRoll, T aYaw, T aPitch) -> Matrix4x4&;

		CONSTEXPR auto RotateRoll(T aRoll) -> Matrix4x4&;
		CONSTEXPR auto RotateYaw(T aYaw) -> Matrix4x4&;
		CONSTEXPR auto RotatePitch(T aPitch) -> Matrix4x4&;

		NODISC CONSTEXPR Vector3<T> TransformPoint(const Vector3<T>& aPoint) const;
		NODISC CONSTEXPR Vector4<T> TransformPoint(const Vector4<T>& aPoint) const;

		CONSTEXPR auto Add(const Matrix4x4& aRight) -> Matrix4x4&;
		CONSTEXPR auto Subtract(const Matrix4x4& aRight) -> Matrix4x4&;
		CONSTEXPR auto Combine(const Matrix4x4& aRight) -> Matrix4x4&;

		NODISC CONSTEXPR static auto CreateProjection(T aFov, T aAspectRatio, T aNearClip, T aFarClip) -> Matrix4x4;
		NODISC CONSTEXPR static auto CreateTRS(const Vector3<T>& aPosition, const Vector3<T>& aRotation, const Vector3<T>& aScale) -> Matrix4x4;

		NODISC CONSTEXPR static auto CreateRotationAroundX(T aRadians) -> Matrix4x4;
		NODISC CONSTEXPR static auto CreateRotationAroundY(T aRadians) -> Matrix4x4;
		NODISC CONSTEXPR static auto CreateRotationAroundZ(T aRadians) -> Matrix4x4;

		static CONSTEXPR int ROWS		= 4;
		static CONSTEXPR int COLUMNS	= 4;
		static CONSTEXPR int COUNT		= ROWS * COLUMNS;

		static const Matrix4x4 IDENTITY;

	private:
		std::array<T, COUNT> myMatrix
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	};

	template<typename T>
	CONSTEXPR Matrix4x4<T>::Matrix4x4() = default;

	template<typename T>
	CONSTEXPR Matrix4x4<T>::~Matrix4x4() = default;

	template<typename T>
	CONSTEXPR Matrix4x4<T>::Matrix4x4(
		T a00, T a10, T a20, T a30, 
		T a01, T a11, T a21, T a31, 
		T a02, T a12, T a22, T a32, 
		T a03, T a13, T a23, T a33) 
		: myMatrix{ a00, a10, a20, a30,
					a01, a11, a21, a31,
					a02, a12, a22, a32, 
					a03, a13, a23, a33 }
	{

	}

	template<typename T>
	CONSTEXPR T& Matrix4x4<T>::operator[](int aIndex)
	{
		return myMatrix[aIndex];
	}
	template<typename T>
	CONSTEXPR const T& Matrix4x4<T>::operator[](int aIndex) const
	{
		return myMatrix[aIndex];
	}

	template<typename T>
	CONSTEXPR T& Matrix4x4<T>::operator()(int aRow, int aColumn)
	{
		return myMatrix[(aColumn - 1) + (aRow - 1) * COLUMNS];
	}
	template<typename T>
	CONSTEXPR const T& Matrix4x4<T>::operator()(int aRow, int aColumn) const
	{
		return myMatrix[(aColumn - 1) + (aRow - 1) * COLUMNS];
	}

	template<typename T>
	inline CONSTEXPR const T* Matrix4x4<T>::GetData() const noexcept
	{
		return myMatrix.data();
	}

	template<typename T>
	CONSTEXPR auto Matrix4x4<T>::GetTranspose() const -> Matrix4x4
	{
		return Matrix4x4<T> 
		{
			myMatrix[0], myMatrix[4], myMatrix[8], myMatrix[12],
			myMatrix[1], myMatrix[5], myMatrix[9], myMatrix[13],
			myMatrix[2], myMatrix[6], myMatrix[10], myMatrix[14],
			myMatrix[3], myMatrix[7], myMatrix[11], myMatrix[15]
		};
	}

	template<typename T>
	CONSTEXPR Vector3<T> Matrix4x4<T>::GetTranslation() const
	{
		return Vector3<T>(myMatrix[12], myMatrix[13], myMatrix[14]);
	}
	template<typename T>
	CONSTEXPR Vector3<T> Matrix4x4<T>::GetRotation() const
	{
		return Vector3<T>
		{
			std::atan2f(myMatrix[6], myMatrix[10]),
			std::atan2f(-myMatrix[2], std::sqrtf(myMatrix[6] * myMatrix[6] + myMatrix[10] * myMatrix[10])),
			std::atan2f(myMatrix[1], myMatrix[0])
		};
	}
	template<typename T>
	CONSTEXPR Vector3<T> Matrix4x4<T>::GetScale() const
	{
		return Vector3<T>
		{
			Vector3<T>{ myMatrix[0], myMatrix[1], myMatrix[2 ] }.Length(),
			Vector3<T>{ myMatrix[4], myMatrix[5], myMatrix[6 ] }.Length(),
			Vector3<T>{ myMatrix[8], myMatrix[9], myMatrix[10] }.Length()
		};
	}

	template<typename T>
	NODISC CONSTEXPR Vector3<T> Matrix4x4<T>::GetForward() const
	{
		return Vector3<T>{ myMatrix[8], myMatrix[9], myMatrix[10] };
	}
	template<typename T>
	NODISC CONSTEXPR Vector3<T> Matrix4x4<T>::GetUp() const 
	{
		return Vector3<T>{ myMatrix[4], myMatrix[5], myMatrix[6] };
	}
	template<typename T>
	NODISC CONSTEXPR Vector3<T> Matrix4x4<T>::GetRight() const 
	{
		return Vector3<T>{ myMatrix[0], myMatrix[1], myMatrix[2] };
	}

	template<typename T>
	CONSTEXPR void Matrix4x4<T>::SetTranslation(const Vector3<T>& aTranslation)
	{
		myMatrix[12] = aTranslation.x;
		myMatrix[13] = aTranslation.y;
		myMatrix[14] = aTranslation.z;
	}
	template<typename T>
	CONSTEXPR void Matrix4x4<T>::SetRotation(const Vector3<T>& aRotation)
	{
		*this = CreateTRS(GetTranslation(), aRotation, GetScale());
	}
	template<typename T>
	CONSTEXPR void Matrix4x4<T>::SetScale(const Vector3<T>& aScale)
	{
		Vector3<T> scaleX{ myMatrix[0], myMatrix[1], myMatrix[2 ] };
		Vector3<T> scaleY{ myMatrix[3], myMatrix[4], myMatrix[6 ] };
		Vector3<T> scaleZ{ myMatrix[3], myMatrix[4], myMatrix[10] };

		scaleX.Normalize(aScale.x);
		scaleY.Normalize(aScale.y);
		scaleZ.Normalize(aScale.z);

		myMatrix[0] = scaleX.x; myMatrix[1] = scaleX.y; myMatrix[2 ] = scaleX.z;
		myMatrix[4] = scaleY.x; myMatrix[5] = scaleY.y; myMatrix[6 ] = scaleY.z;
		myMatrix[8] = scaleZ.x; myMatrix[9] = scaleZ.y; myMatrix[10] = scaleZ.z;
	}

	template<typename T>
	CONSTEXPR auto Matrix4x4<T>::Inverse() const -> Matrix4x4
	{
		return Matrix4x4();
	}
	template<typename T>
	CONSTEXPR auto Matrix4x4<T>::FastInverse() const -> Matrix4x4
	{
		const Vector3<T> s = GetScale();
		assert(s.x != 0 && s.y != 0 && s.z != 0 && "Cannot divide by zero");

		const Vector3<T> is = static_cast<T>(1) / s;
		const Vector3<T> ip = -GetTranslation();

		Matrix4x4 inverseMatrix
		{
			myMatrix[0],	myMatrix[4],	myMatrix[8],	0,
			myMatrix[1],	myMatrix[5],	myMatrix[9],	0,
			myMatrix[2],	myMatrix[6],	myMatrix[10],	0,
			0,				0,				0,				1
		};

		inverseMatrix.SetTranslation(inverseMatrix.TransformPoint(ip));

		const Matrix4x4 scalingInverse =
		{
			is.x * is.x,	0,				0,				0,
			0,				is.y * is.y,	0,				0,
			0,				0,				is.z * is.z,	0,
			0,				0,				0,				1
		};

		return inverseMatrix.Combine(scalingInverse);
	}

	template<typename T>
	CONSTEXPR auto Matrix4x4<T>::Translate(const Vector3<T>& aTranslation) -> Matrix4x4&
	{
		myMatrix[12] += aTranslation.x;
		myMatrix[13] += aTranslation.y;
		myMatrix[14] += aTranslation.z;

		return *this;
	}
	template<typename T>
	CONSTEXPR auto Matrix4x4<T>::Scale(const Vector3<T>& someFactors) -> Matrix4x4&
	{
		const Matrix4x4 scaling
		{
			someFactors.x,	0,				0,				0,
			0,				someFactors.y,	0,				0,
			0,				0,				someFactors.z,	0,
			0,				0,				0,				1
		};

		return Combine(scaling);
	}
	template<typename T>
	CONSTEXPR auto Matrix4x4<T>::Rotate(T aRoll, T aYaw, T aPitch) -> Matrix4x4&
	{
		return Combine(CreateRotationAroundZ(aRoll) * CreateRotationAroundY(aYaw) * CreateRotationAroundX(aPitch));
	}

	template<typename T>
	CONSTEXPR auto Matrix4x4<T>::RotateRoll(T aRoll) -> Matrix4x4&
	{
		return Combine(CreateRotationAroundZ(aRoll));
	}
	template<typename T>
	CONSTEXPR auto Matrix4x4<T>::RotateYaw(T aYaw) -> Matrix4x4&
	{
		return Combine(CreateRotationAroundY(aYaw));
	}
	template<typename T>
	CONSTEXPR auto Matrix4x4<T>::RotatePitch(T aPitch) -> Matrix4x4&
	{
		return Combine(CreateRotationAroundX(aPitch));
	}

	template<typename T>
	CONSTEXPR Vector3<T> Matrix4x4<T>::TransformPoint(const Vector3<T>& aPoint) const
	{
		return Vector3<T>
		{
			aPoint.x * myMatrix[0] + aPoint.y * myMatrix[4] + aPoint.z * myMatrix[8 ] + myMatrix[12],
			aPoint.x * myMatrix[1] + aPoint.y * myMatrix[5] + aPoint.z * myMatrix[9 ] + myMatrix[13],
			aPoint.x * myMatrix[2] + aPoint.y * myMatrix[6] + aPoint.z * myMatrix[10] + myMatrix[14]
		};
	}
	template<typename T>
	CONSTEXPR Vector4<T> Matrix4x4<T>::TransformPoint(const Vector4<T>& aPoint) const
	{
		return Vector4<T> 
		{	
			aPoint.x * myMatrix[0] + aPoint.y * myMatrix[4] + aPoint.z * myMatrix[8 ] + aPoint.w * myMatrix[12],
			aPoint.x * myMatrix[1] + aPoint.y * myMatrix[5] + aPoint.z * myMatrix[9 ] + aPoint.w * myMatrix[13],
			aPoint.x * myMatrix[2] + aPoint.y * myMatrix[6] + aPoint.z * myMatrix[10] + aPoint.w * myMatrix[14],
			aPoint.x * myMatrix[3] + aPoint.y * myMatrix[7] + aPoint.z * myMatrix[11] + aPoint.w * myMatrix[15] 
		};
	}

	template<typename T>
	CONSTEXPR auto Matrix4x4<T>::Add(const Matrix4x4& aRight) -> Matrix4x4&
	{
		for (int i = 0; i < COUNT; ++i)
		{
			myMatrix[i] += aRight[i];
		}

		return *this;
	}

	template<typename T>
	CONSTEXPR auto Matrix4x4<T>::Subtract(const Matrix4x4& aRight) -> Matrix4x4&
	{
		for (int i = 0; i < COUNT; ++i)
		{
			myMatrix[i] -= aRight[i];
		}

		return *this;
	}

	template<typename T>
	CONSTEXPR auto Matrix4x4<T>::Combine(const Matrix4x4& aRight) -> Matrix4x4&
	{
		const T* a = myMatrix.data();
		const T* b = aRight.myMatrix.data();

		*this = Matrix4x4<T>
		{
			(a[0 ] * b[0] + a[1 ] * b[4] + a[2 ] * b[8] + a[3 ] * b[12]), (a[0 ] * b[1] + a[1 ] * b[5] + a[2 ] * b[9] + a[3 ] * b[13]), (a[0 ] * b[2] + a[1 ] * b[6] + a[2 ] * b[10] + a[3 ] * b[14]), (a[0 ] * b[3] + a[1 ] * b[7] + a[2 ] * b[11] + a[3 ] * b[15]),
			(a[4 ] * b[0] + a[5 ] * b[4] + a[6 ] * b[8] + a[7 ] * b[12]), (a[4 ] * b[1] + a[5 ] * b[5] + a[6 ] * b[9] + a[7 ] * b[13]), (a[4 ] * b[2] + a[5 ] * b[6] + a[6 ] * b[10] + a[7 ] * b[14]), (a[4 ] * b[3] + a[5 ] * b[7] + a[6 ] * b[11] + a[7 ] * b[15]),
			(a[8 ] * b[0] + a[9 ] * b[4] + a[10] * b[8] + a[11] * b[12]), (a[8 ] * b[1] + a[9 ] * b[5] + a[10] * b[9] + a[11] * b[13]), (a[8 ] * b[2] + a[9 ] * b[6] + a[10] * b[10] + a[11] * b[14]), (a[8 ] * b[3] + a[9 ] * b[7] + a[10] * b[11] + a[11] * b[15]),
			(a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12]), (a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13]), (a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14]), (a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15])
		};

		return *this;
	}

	template<typename T>
	CONSTEXPR auto Matrix4x4<T>::CreateProjection(T aFov, T aAspectRatio, T aNearClip, T aFarClip) -> Matrix4x4
	{
		float xScale = T(1) / std::tan(aFov / T(2));
		float yScale = xScale * aAspectRatio;

		return Matrix4x4
		{
			xScale, 0,		0,													0,
			0,		yScale,	0,													0,
			0,		0,		aFarClip / (aFarClip - aNearClip),					1,
			0,		0,		(-aNearClip * aFarClip) / (aFarClip - aNearClip),	0
		};
	}
	template<typename T>
	CONSTEXPR auto Matrix4x4<T>::CreateTRS(const Vector3<T>& aPosition, const Vector3<T>& aRotation, const Vector3<T>& aScale) -> Matrix4x4
	{
		Matrix4x4 trsMatrix = CreateRotationAroundX(aRotation.x).Combine(CreateRotationAroundY(aRotation.y)).Combine(CreateRotationAroundZ(aRotation.z));

		trsMatrix.SetTranslation(aPosition);

		const Matrix4x4 scalingMatrix
		{
			aScale.x,	0,			0,			0,
			0,			aScale.y,	0,			0,
			0,			0,			aScale.z,	0,
			0,			0,			0,			1
		};

		trsMatrix.Combine(scalingMatrix);

		return trsMatrix;
	}

	template<typename T>
	CONSTEXPR auto Matrix4x4<T>::CreateRotationAroundX(T aRadians) -> Matrix4x4
	{
		const T c = std::cos(aRadians);
		const T s = std::sin(aRadians);

		return Matrix4x4<T> 
		{
			1,  0,  0,  0,
			0,  c,  s,  0,
			0, -s,  c,  0,
			0,  0,  0,  1
		};
	}
	template<typename T>
	CONSTEXPR auto Matrix4x4<T>::CreateRotationAroundY(T aRadians) -> Matrix4x4
	{
		const T c = std::cos(aRadians);
		const T s = std::sin(aRadians);

		return Matrix4x4<T> 
		{
			c,  0, -s,  0,
			0,  1,  0,  0,
			s,  0,  c,  0,
			0,  0,  0,  1
		};
	}
	template<typename T>
	CONSTEXPR auto Matrix4x4<T>::CreateRotationAroundZ(T aRadians) -> Matrix4x4
	{
		const T c = std::cos(aRadians);
		const T s = std::sin(aRadians);

		return Matrix4x4<T> 
		{
			 c,  s,  0,  0,
			-s,  c,  0,  0,
			 0,  0,  1,  0,
			 0,  0,  0,  1
		};
	}

	// GLOBAL OPERATORS

	template<typename T>
	CONSTEXPR Matrix4x4<T> operator+(const Matrix4x4<T>& aLeft, const Matrix4x4<T>& aRight)
	{
		return Matrix4x4<T>(aLeft).Add(aRight);
	}
	template<typename T>
	CONSTEXPR Matrix4x4<T> operator-(const Matrix4x4<T>& aLeft, const Matrix4x4<T>& aRight)
	{
		return Matrix4x4<T>(aLeft).Subtract(aRight);
	}
	template<typename T>
	CONSTEXPR Matrix4x4<T> operator*(const Matrix4x4<T>& aLeft, const Matrix4x4<T>& aRight)
	{
		return Matrix4x4<T>(aLeft).Combine(aRight);
	}

	template<typename T>
	CONSTEXPR Matrix4x4<T>& operator+=(Matrix4x4<T>& aLeft, const Matrix4x4<T>& aRight)
	{
		return aLeft.Add(aRight);
	}
	template<typename T>
	CONSTEXPR Matrix4x4<T>& operator-=(Matrix4x4<T>& aLeft, const Matrix4x4<T>& aRight)
	{
		return aLeft.Subtract(aRight);
	}
	template<typename T>
	CONSTEXPR Matrix4x4<T>& operator*=(Matrix4x4<T>& aLeft, const Matrix4x4<T>& aRight)
	{
		return aLeft.Combine(aRight);
	}

	template<typename T>
	CONSTEXPR Vector4<T> operator*(const Vector4<T>& aLeft, const Matrix4x4<T>& aRight)
	{
		return aRight.TransformPoint(aLeft);
	}

	template<typename T>
	CONSTEXPR bool operator==(const Matrix4x4<T>& aLeft, const Matrix4x4<T>& aRight)
	{
		for (int i = 0; i < Matrix4x4<T>::COUNT; ++i)
		{
			if (aLeft[i] != aRight[i])
			{
				return false;
			}
		}

		return true;
	}
	template<typename T>
	CONSTEXPR bool operator!=(const Matrix4x4<T>& aLeft, const Matrix4x4<T>& aRight)
	{
		return !(aLeft == aRight);
	}

	template<typename T>
	inline const Matrix4x4<T> Matrix4x4<T>::IDENTITY;

	// using declarations

	using Mat4f = Matrix4x4<float>;
	using Mat4d = Matrix4x4<double>;
	using Mat4i = Matrix4x4<int>;
	using Mat4u = Matrix4x4<unsigned int>;
}
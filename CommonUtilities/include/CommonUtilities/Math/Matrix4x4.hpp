#pragma once

#include <array>
#include <cmath>
#include <cassert>
#include <stdexcept>

#include "Vector3.hpp"
#include "Vector4.hpp"

#include <CommonUtilities/Config.h>
#include <CommonUtilities/Utility/ArithmeticUtils.hpp>

#include "Quaternion.hpp"

namespace CommonUtilities
{
	template<typename T>
	class Matrix3x3;

	template<typename T>
	class Matrix4x4
	{
	public:
		static constexpr int ROWS		= 4;
		static constexpr int COLUMNS	= 4;
		static constexpr int COUNT		= ROWS * COLUMNS;

		constexpr Matrix4x4() = default;
		constexpr ~Matrix4x4() = default;

		constexpr Matrix4x4(
			T a00, T a10, T a20, T a30,
			T a01, T a11, T a21, T a31,
			T a02, T a12, T a22, T a32,
			T a03, T a13, T a23, T a33);

		constexpr Matrix4x4(const std::array<T, COUNT>& aArray);

		constexpr Matrix4x4(const T(&aArray)[COUNT]);

		constexpr Matrix4x4(const Matrix3x3<T>& aMatrix);

		template<class OtherMatrix>
		NODISC constexpr explicit operator OtherMatrix() const;

		NODISC constexpr T& operator[](int aIndex);
		NODISC constexpr const T& operator[](int aIndex) const;

		NODISC constexpr T& operator()(int aRow, int aColumn);
		NODISC constexpr const T& operator()(int aRow, int aColumn) const;

		NODISC constexpr const T* GetData() const noexcept;
		NODISC constexpr T* GetData() noexcept;

		NODISC constexpr auto GetTranspose() const -> Matrix4x4;
		NODISC constexpr auto GetRotationMatrix() const -> Matrix4x4;

		NODISC constexpr Vector3<T> GetTranslation() const;
		NODISC constexpr Vector3<T> GetRotation() const;
		NODISC constexpr Vector3<T> GetScale() const;

		NODISC constexpr Vector3<T> GetForward() const;
		NODISC constexpr Vector3<T> GetUp() const;
		NODISC constexpr Vector3<T> GetRight() const;

		constexpr void DecomposeTransform(Vector3<T>& outPosition, Vector3<T>& outRotation, Vector3<T>& outScale);

		constexpr void SetTranslation(const Vector3<T>& aTranslation);
		constexpr void SetRotation(const Vector3<T>& aPitchYawRoll);
		constexpr void SetRotation(T aYaw, T aPitch, T aRoll);
		constexpr void SetRotation(const Quaternion<T>& aQuaternion);
		constexpr void SetScale(const Vector3<T>& aScale);

		NODISC constexpr auto GetInverse() const -> Matrix4x4;
		NODISC constexpr auto GetFastInverse() const -> Matrix4x4;

		constexpr auto Translate(const Vector3<T>& aTranslation) -> Matrix4x4&;
		constexpr auto Scale(const Vector3<T>& someFactors) -> Matrix4x4&;
		constexpr auto Rotate(const Vector3<T>& aPitchYawRoll) -> Matrix4x4&;
		constexpr auto Rotate(T aYaw, T aPitch, T aRoll) -> Matrix4x4&;
		constexpr auto Rotate(const Quaternion<T>& aQuaternion) -> Matrix4x4&;

		constexpr auto RotateRoll(T aRoll) -> Matrix4x4&;
		constexpr auto RotateYaw(T aYaw) -> Matrix4x4&;
		constexpr auto RotatePitch(T aPitch) -> Matrix4x4&;

		NODISC constexpr Vector3<T> TransformPoint(const Vector3<T>& aPoint) const;
		NODISC constexpr Vector4<T> TransformPoint(const Vector4<T>& aPoint) const;

		constexpr auto Add(const Matrix4x4& aRight) -> Matrix4x4&;
		constexpr auto Subtract(const Matrix4x4& aRight) -> Matrix4x4&;
		constexpr auto Combine(const Matrix4x4& aRight) -> Matrix4x4&;

		NODISC constexpr static auto CreateOrthographic(T aWidth, T aHeight, T aDepth) -> Matrix4x4;
		NODISC constexpr static auto CreateOrthographic(T aLeft, T aRight, T aTop, T aBottom, T aNear, T aFar) -> Matrix4x4;
		NODISC constexpr static auto CreatePerspective(T aHorizontalFOVDeg, T aAspectRatio, T aNearClip, T aFarClip) -> Matrix4x4;

		NODISC constexpr static auto CreateTRS(const Vector3<T>& aPosition, const Vector3<T>& aPitchYawRoll, const Vector3<T>& aScale) -> Matrix4x4;
		NODISC constexpr static auto CreateTRS(const Vector3<T>& aPosition, const Quaternion<T>& aQuaternion, const Vector3<T>& aScale) -> Matrix4x4;

		NODISC constexpr static auto CreateRotationAroundX(T aRadians) -> Matrix4x4;
		NODISC constexpr static auto CreateRotationAroundY(T aRadians) -> Matrix4x4;
		NODISC constexpr static auto CreateRotationAroundZ(T aRadians) -> Matrix4x4;

		NODISC constexpr static auto CreateLookAt(const Vector3<T>& aEyePosition, const Vector3<T>& aDirection, const Vector3<T>& aUp = Vector3<T>(0, 1, 0));

		NODISC constexpr static auto CreateRotationMatrixFromNormalizedQuaternion(const Quaternion<T>& aQuaternion) -> Matrix4x4;

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
	constexpr Matrix4x4<T>::Matrix4x4(
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
	inline constexpr Matrix4x4<T>::Matrix4x4(const std::array<T, COUNT>& aArray)
		: myMatrix{ aArray }
	{

	}

	template<typename T>
	inline constexpr Matrix4x4<T>::Matrix4x4(const T(&aArray)[COUNT])
		: myMatrix{ std::to_array(aArray) }
	{

	}

	template<typename T>
	inline constexpr Matrix4x4<T>::Matrix4x4(const Matrix3x3<T>& aMatrix)
		: myMatrix{
			aMatrix[0],		aMatrix[1],		0,		aMatrix[2],
			aMatrix[3],		aMatrix[4],		0,		aMatrix[5],
			0,				0,				1,		0,
			aMatrix[6],		aMatrix[7],		0,		aMatrix[8]}
	{

	}

	template<typename T>
	template<class OtherMatrix>
	constexpr Matrix4x4<T>::operator OtherMatrix() const
	{
		return OtherMatrix{ GetData() };
	}

	template<typename T>
	constexpr T& Matrix4x4<T>::operator[](int aIndex)
	{
		return myMatrix[aIndex];
	}
	template<typename T>
	constexpr const T& Matrix4x4<T>::operator[](int aIndex) const
	{
		return myMatrix[aIndex];
	}

	template<typename T>
	constexpr T& Matrix4x4<T>::operator()(int aRow, int aColumn)
	{
		return myMatrix[(aColumn - 1) + (aRow - 1) * COLUMNS];
	}
	template<typename T>
	constexpr const T& Matrix4x4<T>::operator()(int aRow, int aColumn) const
	{
		return myMatrix[(aColumn - 1) + (aRow - 1) * COLUMNS];
	}

	template<typename T>
	constexpr const T* Matrix4x4<T>::GetData() const noexcept
	{
		return myMatrix.data();
	}
	template<typename T>
	constexpr T* Matrix4x4<T>::GetData() noexcept
	{
		return myMatrix.data();
	}

	template<typename T>
	constexpr auto Matrix4x4<T>::GetTranspose() const -> Matrix4x4
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
	constexpr auto Matrix4x4<T>::GetRotationMatrix() const -> Matrix4x4
	{
		Matrix4x4<T> rotationMatrix
		{
			myMatrix[0],	myMatrix[1],	myMatrix[2],	0,
			myMatrix[4],	myMatrix[5],	myMatrix[6],	0,
			myMatrix[8],	myMatrix[9],	myMatrix[10],	0,
			0,				0,				0,				1
		};

		rotationMatrix.SetScale(Vector3f(1.0f, 1.0f, 1.0f)); // remove scaling

		return rotationMatrix;
	}

	template<typename T>
	constexpr Vector3<T> Matrix4x4<T>::GetTranslation() const
	{
		return Vector3<T>(myMatrix[12], myMatrix[13], myMatrix[14]);
	}
	template<typename T>
	constexpr Vector3<T> Matrix4x4<T>::GetRotation() const
	{
		cu::Vector3<T> forward	= GetForward().GetNormalized();
		cu::Vector3<T> right	= GetRight().GetNormalized();
		cu::Vector3<T> up		= GetUp().GetNormalized();

		return Vector3<T>
		{
			std::atan2f(up.z, forward.z),
			std::atan2f(-right.z, std::sqrtf(up.z * up.z + forward.z * forward.z)),
			std::atan2f(right.y, right.x)
		};
	}
	template<typename T>
	constexpr Vector3<T> Matrix4x4<T>::GetScale() const
	{
		return Vector3<T>
		{
			GetRight().Length(), GetUp().Length(), GetForward().Length()
		};
	}

	template<typename T>
	constexpr Vector3<T> Matrix4x4<T>::GetForward() const
	{
		return Vector3<T>{ myMatrix[8], myMatrix[9], myMatrix[10] };
	}
	template<typename T>
	constexpr Vector3<T> Matrix4x4<T>::GetUp() const 
	{
		return Vector3<T>{ myMatrix[4], myMatrix[5], myMatrix[6] };
	}
	template<typename T>
	constexpr Vector3<T> Matrix4x4<T>::GetRight() const 
	{
		return Vector3<T>{ myMatrix[0], myMatrix[1], myMatrix[2] };
	}

	template<typename T>
	constexpr void Matrix4x4<T>::DecomposeTransform(Vector3<T>& outPosition, Vector3<T>& outRotation, Vector3<T>& outScale)
	{
		outPosition = GetTranslation();

		cu::Vector3<T> right	= GetRight();
		cu::Vector3<T> up		= GetUp();
		cu::Vector3<T> forward	= GetForward();

		outScale.x = right.Length();
		outScale.y = up.Length();
		outScale.z = forward.Length();

		right   = right.GetNormalized(outScale.x, T(1)); // normalize using already calculated length
		up	    = up.GetNormalized(outScale.y, T(1));
		forward = forward.GetNormalized(outScale.z, T(1));

		outRotation.x = std::atan2f(up.z, forward.z);
		outRotation.y = std::atan2f(-right.z, std::sqrtf(up.z * up.z + forward.z * forward.z));
		outRotation.z = std::atan2f(right.y, right.x);
	}

	template<typename T>
	constexpr void Matrix4x4<T>::SetTranslation(const Vector3<T>& aTranslation)
	{
		myMatrix[12] = aTranslation.x;
		myMatrix[13] = aTranslation.y;
		myMatrix[14] = aTranslation.z;
	}
	template<typename T>
	constexpr void Matrix4x4<T>::SetRotation(const Vector3<T>& aPitchYawRoll)
	{
		*this = CreateTRS(GetTranslation(), aPitchYawRoll, GetScale());
	}
	template<typename T>
	constexpr void Matrix4x4<T>::SetRotation(T aYaw, T aPitch, T aRoll)
	{
		SetRotation(Vector3<T>(aYaw, aPitch, aRoll));
	}
	template<typename T>
	constexpr void Matrix4x4<T>::SetRotation(const Quaternion<T>& aQuaternion)
	{
		*this = CreateTRS(GetTranslation(), aQuaternion, GetScale());
	}
	template<typename T>
	constexpr void Matrix4x4<T>::SetScale(const Vector3<T>& aScale)
	{
		Vector3<T> scaleX{ myMatrix[0], myMatrix[1], myMatrix[2 ] };
		Vector3<T> scaleY{ myMatrix[4], myMatrix[5], myMatrix[6 ] };
		Vector3<T> scaleZ{ myMatrix[8], myMatrix[9], myMatrix[10] };

		scaleX.Normalize(aScale.x);
		scaleY.Normalize(aScale.y);
		scaleZ.Normalize(aScale.z);

		myMatrix[0] = scaleX.x; myMatrix[1] = scaleX.y; myMatrix[2 ] = scaleX.z;
		myMatrix[4] = scaleY.x; myMatrix[5] = scaleY.y; myMatrix[6 ] = scaleY.z;
		myMatrix[8] = scaleZ.x; myMatrix[9] = scaleZ.y; myMatrix[10] = scaleZ.z;
	}

	template<typename T>
	constexpr auto Matrix4x4<T>::GetInverse() const -> Matrix4x4
	{
		throw std::runtime_error("Not yet implemented");
		return Matrix4x4();
	}
	template<typename T>
	constexpr auto Matrix4x4<T>::GetFastInverse() const -> Matrix4x4
	{
		const Vector3<T> s = GetScale();
		assert(s.x != 0 && s.y != 0 && s.z != 0 && "Cannot divide by zero");

		const Vector3<T> is = T(1) / s;
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
	constexpr auto Matrix4x4<T>::Translate(const Vector3<T>& aTranslation) -> Matrix4x4&
	{
		myMatrix[12] += aTranslation.x;
		myMatrix[13] += aTranslation.y;
		myMatrix[14] += aTranslation.z;

		return *this;
	}
	template<typename T>
	constexpr auto Matrix4x4<T>::Scale(const Vector3<T>& someFactors) -> Matrix4x4&
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
	constexpr auto Matrix4x4<T>::Rotate(const Vector3<T>& aPitchYawRoll) -> Matrix4x4&
	{
		return Rotate(Quaternion<T>(aPitchYawRoll.y, aPitchYawRoll.x, aPitchYawRoll.z));
	}
	template<typename T>
	constexpr auto Matrix4x4<T>::Rotate(T aYaw, T aPitch, T aRoll) -> Matrix4x4&
	{
		return Rotate(Vector3<T>(aYaw, aPitch, aRoll));
	}
	template<typename T>
	constexpr auto Matrix4x4<T>::Rotate(const Quaternion<T>& aQuaternion) -> Matrix4x4&
	{
		return Combine(CreateRotationMatrixFromNormalizedQuaternion(aQuaternion));
	}

	template<typename T>
	constexpr auto Matrix4x4<T>::RotateRoll(T aRoll) -> Matrix4x4&
	{
		return Combine(CreateRotationAroundZ(aRoll));
	}
	template<typename T>
	constexpr auto Matrix4x4<T>::RotateYaw(T aYaw) -> Matrix4x4&
	{
		return Combine(CreateRotationAroundY(aYaw));
	}
	template<typename T>
	constexpr auto Matrix4x4<T>::RotatePitch(T aPitch) -> Matrix4x4&
	{
		return Combine(CreateRotationAroundX(aPitch));
	}

	template<typename T>
	constexpr Vector3<T> Matrix4x4<T>::TransformPoint(const Vector3<T>& aPoint) const
	{
		return Vector3<T>
		{
			aPoint.x * myMatrix[0] + aPoint.y * myMatrix[4] + aPoint.z * myMatrix[8 ] + myMatrix[12],
			aPoint.x * myMatrix[1] + aPoint.y * myMatrix[5] + aPoint.z * myMatrix[9 ] + myMatrix[13],
			aPoint.x * myMatrix[2] + aPoint.y * myMatrix[6] + aPoint.z * myMatrix[10] + myMatrix[14]
		};
	}
	template<typename T>
	constexpr Vector4<T> Matrix4x4<T>::TransformPoint(const Vector4<T>& aPoint) const
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
	constexpr auto Matrix4x4<T>::Add(const Matrix4x4& aRight) -> Matrix4x4&
	{
		for (int i = 0; i < COUNT; ++i)
		{
			myMatrix[i] += aRight[i];
		}

		return *this;
	}

	template<typename T>
	constexpr auto Matrix4x4<T>::Subtract(const Matrix4x4& aRight) -> Matrix4x4&
	{
		for (int i = 0; i < COUNT; ++i)
		{
			myMatrix[i] -= aRight[i];
		}

		return *this;
	}

	template<typename T>
	constexpr auto Matrix4x4<T>::Combine(const Matrix4x4& aRight) -> Matrix4x4&
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
	constexpr auto Matrix4x4<T>::CreateOrthographic(T aWidth, T aHeight, T aDepth) -> Matrix4x4
	{
		return Matrix4x4
		{
			T(2) / aWidth,	0,				0,				0,
			0,				T(2) / aHeight, 0,				0,
			0,				0,				T(1) / aDepth,	0,
			0,				0,				0,				1
		};
	}
	template<typename T>
	constexpr auto Matrix4x4<T>::CreateOrthographic(T aLeft, T aRight, T aTop, T aBottom, T aNear, T aFar) -> Matrix4x4
	{
		const T width	= (aRight - aLeft);
		const T height	= (aBottom - aTop);
		const T depth	= (aFar - aNear);

		return Matrix4x4
		{
			T(2) / width,				0,							0,							0,
			0,							T(2) / height,				0,							0,
			0,							0,							T(1) / depth,				0,
			-(aRight + aLeft) / width,	-(aBottom + aTop) / height, -aNear / depth,				1
		};
	}
	template<typename T>
	constexpr auto Matrix4x4<T>::CreatePerspective(T aHorizontalFOVDeg, T aAspectRatio, T aNearClip, T aFarClip) -> Matrix4x4
	{
		assert((float)aNearClip < (float)aFarClip);
		assert((float)aNearClip >= 0.00000000000001f);

		const float hFOVRad = (float)aHorizontalFOVDeg * au::DEG2RAD;
		const float xScale = 1.0f / std::tan(hFOVRad / 2.0f);
		const float yScale = (float)aAspectRatio * xScale;

		const float Q = aFarClip / (aFarClip - aNearClip);

		return Matrix4x4
		{
			xScale, 0,		0,				0,
			0,		yScale,	0,				0,
			0,		0,		Q,				1,
			0,		0,		-Q * aNearClip,	0
		};
	}

	template<typename T>
	constexpr auto Matrix4x4<T>::CreateTRS(const Vector3<T>& aPosition, const Vector3<T>& aPitchYawRoll, const Vector3<T>& aScale) -> Matrix4x4
	{
		return Matrix4x4()
			.Scale(aScale)
			.Rotate(aPitchYawRoll)
			.Translate(aPosition);
	}
	template<typename T>
	constexpr auto Matrix4x4<T>::CreateTRS(const Vector3<T>& aPosition, const Quaternion<T>& aQuaternion, const Vector3<T>& aScale) -> Matrix4x4
	{
		return Matrix4x4()
			.Scale(aScale)
			.Rotate(aQuaternion)
			.Translate(aPosition);
	}

	template<typename T>
	constexpr auto Matrix4x4<T>::CreateRotationAroundX(T aRadians) -> Matrix4x4
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
	constexpr auto Matrix4x4<T>::CreateRotationAroundY(T aRadians) -> Matrix4x4
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
	constexpr auto Matrix4x4<T>::CreateRotationAroundZ(T aRadians) -> Matrix4x4
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

	template<typename T>
	inline constexpr auto Matrix4x4<T>::CreateLookAt(const Vector3<T>& aEyePosition, const Vector3<T>& aDirection, const Vector3<T>& aUp)
	{
		Vector3<T> xAxis = aUp.Cross(aDirection).GetNormalized();
		Vector3<T> yAxis = aDirection.Cross(xAxis);

		return Matrix4x4<T>
		{
			xAxis.x,					yAxis.x,					aDirection.x,					0,
			xAxis.y,					yAxis.y,					aDirection.y,					0,
			xAxis.z,					yAxis.z,					aDirection.z,					0,
			-xAxis.Dot(aEyePosition),	-yAxis.Dot(aEyePosition),	-aDirection.Dot(aEyePosition),	1
		};
	}

	template<typename T>
	constexpr auto Matrix4x4<T>::CreateRotationMatrixFromNormalizedQuaternion(const Quaternion<T>& aQuaternion) -> Matrix4x4
	{
		// assumes the quaternion to be normalized

		T xx = aQuaternion.x * aQuaternion.x;
		T xy = aQuaternion.x * aQuaternion.y;
		T xz = aQuaternion.x * aQuaternion.z;
		T xw = aQuaternion.x * aQuaternion.w;

		T yy = aQuaternion.y * aQuaternion.y;
		T yz = aQuaternion.y * aQuaternion.z;
		T yw = aQuaternion.y * aQuaternion.w;

		T zz = aQuaternion.z * aQuaternion.z;
		T zw = aQuaternion.z * aQuaternion.w;

		T a00 = T(1) - T(2) * (yy + zz);
		T a01 = T(2) * (xy + zw);
		T a02 = T(2) * (xz - yw);

		T a10 = T(2) * (xy - zw);
		T a11 = T(1) - T(2) * (xx + zz);
		T a12 = T(2) * (yz + xw);

		T a20 = T(2) * (xz + yw);
		T a21 = T(2) * (yz - xw);
		T a22 = T(1) - T(2) * (xx + yy);

		return Matrix4x4<T> 
		{
			a00,	a01,	a02,	0,
			a10,	a11,	a12,	0,
			a20,	a21,	a22,	0,
			0,		0,		0,		1
		};
	}

	// GLOBAL OPERATORS

	template<typename T>
	NODISC  Matrix4x4<T> operator+(const Matrix4x4<T>& aLeft, const Matrix4x4<T>& aRight)
	{
		return Matrix4x4<T>(aLeft).Add(aRight);
	}
	template<typename T>
	NODISC constexpr Matrix4x4<T> operator-(const Matrix4x4<T>& aLeft, const Matrix4x4<T>& aRight)
	{
		return Matrix4x4<T>(aLeft).Subtract(aRight);
	}
	template<typename T>
	NODISC constexpr Matrix4x4<T> operator*(const Matrix4x4<T>& aLeft, const Matrix4x4<T>& aRight)
	{
		return Matrix4x4<T>(aLeft).Combine(aRight);
	}

	template<typename T>
	constexpr Matrix4x4<T>& operator+=(Matrix4x4<T>& aLeft, const Matrix4x4<T>& aRight)
	{
		return aLeft.Add(aRight);
	}
	template<typename T>
	constexpr Matrix4x4<T>& operator-=(Matrix4x4<T>& aLeft, const Matrix4x4<T>& aRight)
	{
		return aLeft.Subtract(aRight);
	}
	template<typename T>
	constexpr Matrix4x4<T>& operator*=(Matrix4x4<T>& aLeft, const Matrix4x4<T>& aRight)
	{
		return aLeft.Combine(aRight);
	}

	template<typename T>
	NODISC constexpr Vector4<T> operator*(const Vector4<T>& aLeft, const Matrix4x4<T>& aRight)
	{
		return aRight.TransformPoint(aLeft);
	}
	template<typename T>
	NODISC constexpr Vector4<T> operator*(const Matrix4x4<T>& aLeft, const Vector4<T>& aRight)
	{
		return aRight * aLeft;
	}

	template<typename T>
	NODISC constexpr Vector3<T> operator*(const Matrix4x4<T>& aLeft, const Vector3<T>& aRight)
	{
		return aLeft.TransformPoint(aRight);
	}
	template<typename T>
	NODISC constexpr Vector3<T> operator*(const Vector3<T>& aLeft, const Matrix4x4<T>& aRight)
	{
		return aRight * aLeft;
	}

	template<typename T>
	NODISC constexpr bool operator==(const Matrix4x4<T>& aLeft, const Matrix4x4<T>& aRight)
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
	NODISC constexpr bool operator!=(const Matrix4x4<T>& aLeft, const Matrix4x4<T>& aRight)
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
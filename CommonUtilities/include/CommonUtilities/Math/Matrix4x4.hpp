#pragma once

#include <array>
#include <cmath>
#include <cassert>
#include <immintrin.h>
#include <stdexcept>
#include <iostream>

#include <CommonUtilities/Config.h>
#include <CommonUtilities/Utility/ArithmeticUtils.hpp>

#include "Vector4.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"

namespace CommonUtilities
{
	template<typename t> 
	class AABB;

	template<typename t>
	class Sphere;

	template<typename T>
	class Matrix3x3;

	template<typename T>
	class Matrix4x4
	{
	public:
		constexpr Matrix4x4() = default;
		constexpr ~Matrix4x4() = default;

		constexpr Matrix4x4(
			T a00, T a10, T a20, T a30,
			T a01, T a11, T a21, T a31,
			T a02, T a12, T a22, T a32,
			T a03, T a13, T a23, T a33);

		constexpr Matrix4x4(const std::array<T, 16>& aArray);

		constexpr Matrix4x4(const T(&aArray)[16]);

		constexpr Matrix4x4(const Matrix3x3<T>& aMatrix);

		constexpr Matrix4x4(std::array<__m128, 4> aRegisters) requires (std::is_same_v<T, float>);

		template<class OtherMatrix>
		NODISC constexpr explicit operator OtherMatrix() const;

		NODISC constexpr T& operator[](int aIndex);
		NODISC constexpr const T& operator[](int aIndex) const;

		NODISC constexpr T& operator()(int aRow, int aColumn);
		NODISC constexpr const T& operator()(int aRow, int aColumn) const;

		NODISC constexpr const T* GetData() const noexcept;
		NODISC constexpr T* GetData() noexcept;

		NODISC constexpr const std::array<T, 16>& GetValues() const noexcept;

		NODISC constexpr auto GetTranspose() const -> Matrix4x4;
		NODISC constexpr auto GetTranslationMatrix() const -> Matrix4x4;
		NODISC constexpr auto GetRotationMatrix() const -> Matrix4x4;
		NODISC constexpr auto GetRotationScaleMatrix() const -> Matrix4x4;
		NODISC constexpr auto GetScaleMatrix() const -> Matrix4x4;

		NODISC constexpr Vector3<T> GetTranslation() const;
		NODISC constexpr Vector3<T> GetRotation() const;
		NODISC constexpr Vector3<T> GetScale() const;
		NODISC constexpr Vector3<T> GetScaleSqr() const;

		NODISC constexpr Vector3<T> GetForward() const;
		NODISC constexpr Vector3<T> GetUp() const;
		NODISC constexpr Vector3<T> GetRight() const;

		NODISC constexpr std::array<__m128, 4> ToSIMD() const requires (std::is_same_v<T, float>);

		constexpr void DecomposeTransform(Vector3<T>& outPosition, Vector3<T>& outRotation, Vector3<T>& outScale) const;
		constexpr void DecomposeTransform(Vector3<T>& outPosition, Quaternion<T>& outQuaternion, Vector3<T>& outScale) const;

		constexpr void SetTranslation(const Vector3<T>& aTranslation);
		constexpr void SetRotation(const Vector3<T>& aPitchYawRoll);
		constexpr void SetRotation(const Vector3<T>& aPitchYawRoll, AxisOrder aRotationOrder);
		constexpr void SetRotation(T aYaw, T aPitch, T aRoll);
		constexpr void SetRotation(T aYaw, T aPitch, T aRoll, AxisOrder aRotationOrder);
		constexpr void SetRotation(const Quaternion<T>& aQuaternion);
		constexpr void SetScale(const Vector3<T>& aScale);

		template<std::size_t R> requires (R < 4)
		constexpr void SetRow(const Vector4<T>& aRow);

		template<std::size_t R> requires (R < 4)
		constexpr void SetRow(const Vector3<T>& aRow);

		NODISC constexpr auto GetInverse() const -> Matrix4x4;
		NODISC constexpr auto GetFastInverse() const -> Matrix4x4;

		constexpr auto Translate(const Vector3<T>& aTranslation) -> Matrix4x4&;
		constexpr auto Scale(const Vector3<T>& someFactors) -> Matrix4x4&;
		constexpr auto Rotate(const Vector3<T>& aPitchYawRoll) -> Matrix4x4&;
		constexpr auto Rotate(const Vector3<T>& aPitchYawRoll, AxisOrder aRotationOrder) -> Matrix4x4&;
		constexpr auto Rotate(T aYaw, T aPitch, T aRoll) -> Matrix4x4&;
		constexpr auto Rotate(T aYaw, T aPitch, T aRoll, AxisOrder aRotationOrder) -> Matrix4x4&;
		constexpr auto Rotate(const Quaternion<T>& aQuaternion) -> Matrix4x4&;

		constexpr auto RotateRoll(T aRoll) -> Matrix4x4&;
		constexpr auto RotateYaw(T aYaw) -> Matrix4x4&;
		constexpr auto RotatePitch(T aPitch) -> Matrix4x4&;

		NODISC constexpr Vector3<T> TransformPoint(const Vector3<T>& aPoint) const;
		NODISC constexpr Vector4<T> TransformPoint(const Vector4<T>& aPoint) const;
		NODISC constexpr AABB<T> TransformAABB(const AABB<T>& aAABB) const;
		NODISC constexpr Sphere<T> TransformSphere(const Sphere<T>& aSphere) const;

		constexpr auto Add(const Matrix4x4& aRight) -> Matrix4x4&;
		constexpr auto Subtract(const Matrix4x4& aRight) -> Matrix4x4&;
		constexpr auto Combine(const Matrix4x4& aRight) -> Matrix4x4&;

		NODISC constexpr static auto CreateFromRows(
			const Vector4<T>& aRow0 = { 1, 0, 0, 0 },
			const Vector4<T>& aRow1 = { 0, 1, 0, 0 },
			const Vector4<T>& aRow2 = { 0, 0, 1, 0 },
			const Vector4<T>& aRow3 = { 0, 0, 0, 1 }) -> Matrix4x4;

		NODISC constexpr static auto CreateOrthographic(T aWidth, T aHeight, T aDepth) -> Matrix4x4;
		NODISC constexpr static auto CreateOrthographic(T aLeft, T aRight, T aTop, T aBottom, T aNear, T aFar) -> Matrix4x4;
		NODISC constexpr static auto CreatePerspective(T aHorizontalFOVDeg, T aAspectRatio, T aNearClip, T aFarClip) -> Matrix4x4;

		NODISC constexpr static auto CreateTRS(const Vector3<T>& aPosition, const Vector3<T>& aPitchYawRoll, const Vector3<T>& aScale) -> Matrix4x4;
		NODISC constexpr static auto CreateTRS(const Vector3<T>& aPosition, const Vector3<T>& aPitchYawRoll, const Vector3<T>& aScale, AxisOrder aRotationOrder) -> Matrix4x4;
		NODISC constexpr static auto CreateTRS(const Vector3<T>& aPosition, const Quaternion<T>& aQuaternion, const Vector3<T>& aScale) -> Matrix4x4;

		NODISC constexpr static auto CreateRotationAroundX(T aRadians) -> Matrix4x4;
		NODISC constexpr static auto CreateRotationAroundY(T aRadians) -> Matrix4x4;
		NODISC constexpr static auto CreateRotationAroundZ(T aRadians) -> Matrix4x4;

		NODISC constexpr static auto CreateLookAt(const Vector3<T>& aEyePosition, const Vector3<T>& aDirection, const Vector3<T>& aUp = Vector3<T>(0, 1, 0));

		NODISC constexpr static auto CreateRotationMatrixFromQuaternion(const Quaternion<T>& aQuaternion) -> Matrix4x4;
		NODISC constexpr static auto CreateRotationMatrixFromNormalizedQuaternion(const Quaternion<T>& aQuaternion) -> Matrix4x4;

		NODISC constexpr static auto BlendMatrices(const Matrix4x4& aFrom, const Matrix4x4& aTo, T aBlendFactor) -> Matrix4x4;

		NODISC constexpr static auto DominantEigen(const Matrix4x4& aMatrix, const Vector4<T>& aVector, int aIterations, float aEpsilon) -> Vector4<T>;

		NODISC constexpr static auto Multiply(const Matrix4x4& aLeft, const Matrix4x4& aRight) -> Matrix4x4;

		static const Matrix4x4 IDENTITY;
		static const Matrix4x4 Zero;

	private:
		std::array<T, 16> myMatrix
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
	constexpr Matrix4x4<T>::Matrix4x4(const std::array<T, 16>& aArray)
		: myMatrix{ aArray }
	{

	}

	template<typename T>
	constexpr Matrix4x4<T>::Matrix4x4(const T(&aArray)[16])
		: myMatrix{ std::to_array(aArray) }
	{

	}

	template<typename T>
	constexpr Matrix4x4<T>::Matrix4x4(const Matrix3x3<T>& aMatrix)
		: myMatrix{
			aMatrix[0],		aMatrix[1],		0,		aMatrix[2],
			aMatrix[3],		aMatrix[4],		0,		aMatrix[5],
			0,				0,				1,		0,
			aMatrix[6],		aMatrix[7],		0,		aMatrix[8]}
	{

	}

	template<typename T>
	constexpr Matrix4x4<T>::Matrix4x4(std::array<__m128, 4> aRegisters) requires (std::is_same_v<T, float>)
	{
		alignas(16) std::array<T, 16> values{};
		_mm_store_ps(values.data() + 4 * 0, aRegisters[0]);
		_mm_store_ps(values.data() + 4 * 1, aRegisters[1]);
		_mm_store_ps(values.data() + 4 * 2, aRegisters[2]);
		_mm_store_ps(values.data() + 4 * 3, aRegisters[3]);
		myMatrix = values;
	}

	template<typename T>
	template<class OtherMatrix>
	constexpr Matrix4x4<T>::operator OtherMatrix() const
	{
		return OtherMatrix{ GetData() };
	}

	template<typename T>
	template<std::size_t R> requires (R < 4)
	constexpr void Matrix4x4<T>::SetRow(const Vector4<T>& aRow)
	{
		if constexpr (R == 0)		myMatrix[0 ] = aRow.x, myMatrix[1 ] = aRow.y, myMatrix[2 ] = aRow.z, myMatrix[3 ] = aRow.w;
		else if constexpr (R == 1)	myMatrix[4 ] = aRow.x, myMatrix[5 ] = aRow.y, myMatrix[6 ] = aRow.z, myMatrix[7 ] = aRow.w;
		else if constexpr (R == 2)	myMatrix[8 ] = aRow.x, myMatrix[9 ] = aRow.y, myMatrix[10] = aRow.z, myMatrix[11] = aRow.w;
		else if constexpr (R == 3)	myMatrix[12] = aRow.x, myMatrix[13] = aRow.y, myMatrix[14] = aRow.z, myMatrix[15] = aRow.w;
	}

	template<typename T>
	template<std::size_t R> requires (R < 4)
	constexpr void Matrix4x4<T>::SetRow(const Vector3<T>& aRow)
	{
		if constexpr (R == 0)		myMatrix[0 ] = aRow.x, myMatrix[1 ] = aRow.y, myMatrix[2 ] = aRow.z;
		else if constexpr (R == 1)	myMatrix[4 ] = aRow.x, myMatrix[5 ] = aRow.y, myMatrix[6 ] = aRow.z;
		else if constexpr (R == 2)	myMatrix[8 ] = aRow.x, myMatrix[9 ] = aRow.y, myMatrix[10] = aRow.z;
		else if constexpr (R == 3)	myMatrix[12] = aRow.x, myMatrix[13] = aRow.y, myMatrix[14] = aRow.z;
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
		return myMatrix[(aColumn - 1) + (aRow - 1) * 4];
	}
	template<typename T>
	constexpr const T& Matrix4x4<T>::operator()(int aRow, int aColumn) const
	{
		return myMatrix[(aColumn - 1) + (aRow - 1) * 4];
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
	constexpr const std::array<T, 16>& Matrix4x4<T>::GetValues() const noexcept
	{
		return myMatrix;
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
	constexpr auto Matrix4x4<T>::GetTranslationMatrix() const -> Matrix4x4
	{
		Matrix4x4<T> translationMatrix
		{
			1,				0,				0,				0,
			0,				1,				0,				0,
			0,				0,				1,				0,
			myMatrix[12],	myMatrix[13],	myMatrix[14],	1
		};

		return translationMatrix;
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
	constexpr auto Matrix4x4<T>::GetRotationScaleMatrix() const -> Matrix4x4
	{
		Matrix4x4<T> rotationScaleMatrix
		{
			myMatrix[0],	myMatrix[1],	myMatrix[2],	0,
			myMatrix[4],	myMatrix[5],	myMatrix[6],	0,
			myMatrix[8],	myMatrix[9],	myMatrix[10],	0,
			0,				0,				0,				1
		};

		return rotationScaleMatrix;
	}
	template<typename T>
	constexpr auto Matrix4x4<T>::GetScaleMatrix() const -> Matrix4x4
	{
		const Vector3<T> s = GetScale();

		Matrix4x4<T> scaleMatrix
		{
			s.x,	0,		0,		0,
			0,		s.y,	0,		0,
			0,		0,		s.z,	0,
			0,		0,		0,		1
		};

		return scaleMatrix;
	}

	template<typename T>
	constexpr Vector3<T> Matrix4x4<T>::GetTranslation() const
	{
		return Vector3<T>(myMatrix[12], myMatrix[13], myMatrix[14]);
	}
	template<typename T>
	constexpr Vector3<T> Matrix4x4<T>::GetRotation() const
	{
		Vector3<T> forward	= GetForward().GetNormalized();
		Vector3<T> right	= GetRight().GetNormalized();
		Vector3<T> up		= GetUp().GetNormalized();

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
	constexpr Vector3<T> Matrix4x4<T>::GetScaleSqr() const
	{
		return Vector3<T>
		{
			GetRight().LengthSqr(), GetUp().LengthSqr(), GetForward().LengthSqr()
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
	constexpr std::array<__m128, 4> Matrix4x4<T>::ToSIMD() const requires (std::is_same_v<T, float>)
	{
		return std::array<__m128, 4>
		{
			Vector4<T>{ myMatrix[0 ], myMatrix[1 ], myMatrix[2 ], myMatrix[3 ] }.ToSIMD(), // would like to use __m512 but is not supported by many CPUs
			Vector4<T>{ myMatrix[4 ], myMatrix[5 ], myMatrix[6 ], myMatrix[7 ] }.ToSIMD(),
			Vector4<T>{ myMatrix[8 ], myMatrix[9 ], myMatrix[10], myMatrix[11] }.ToSIMD(),
			Vector4<T>{ myMatrix[12], myMatrix[13], myMatrix[14], myMatrix[15] }.ToSIMD()
		};
	}

	template<typename T>
	constexpr void Matrix4x4<T>::DecomposeTransform(Vector3<T>& outPosition, Vector3<T>& outRotation, Vector3<T>& outScale) const
	{
		outPosition = GetTranslation();

		Vector3<T> right	= GetRight();
		Vector3<T> up		= GetUp();
		Vector3<T> forward	= GetForward();

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
	constexpr void Matrix4x4<T>::DecomposeTransform(Vector3<T>& outPosition, Quaternion<T>& outQuaternion, Vector3<T>& outScale) const
	{
		outPosition = GetTranslation();

		Vector3<T> right	= GetRight();
		Vector3<T> up		= GetUp();
		Vector3<T> forward	= GetForward();

		outScale.x = right.Length();
		outScale.y = up.Length();
		outScale.z = forward.Length();

		right   = right.GetNormalized(outScale.x, T(1));
		up	    = up.GetNormalized(outScale.y, T(1));
		forward = forward.GetNormalized(outScale.z, T(1));

		const Matrix4x4 rotationMatrix
		{
			right.x,	right.y,	right.z,	0,
			up.x,		up.y,		up.z,		0,
			forward.x,	forward.y,	forward.z, 	0,
			0,			0,			0,			1
		};

		outQuaternion = Quaternion<T>(rotationMatrix).GetNormalized();
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
	constexpr void Matrix4x4<T>::SetRotation(const Vector3<T>& aPitchYawRoll, AxisOrder aRotationOrder)
	{
		*this = CreateTRS(GetTranslation(), aPitchYawRoll, GetScale(), aRotationOrder);
	}
	template<typename T>
	constexpr void Matrix4x4<T>::SetRotation(T aYaw, T aPitch, T aRoll)
	{
		SetRotation(Vector3<T>(aPitch, aYaw, aRoll));
	}
	template<typename T>
	constexpr void Matrix4x4<T>::SetRotation(T aYaw, T aPitch, T aRoll, AxisOrder aRotationOrder)
	{
		SetRotation(Vector3<T>(aPitch, aYaw, aRoll), aRotationOrder);
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

		if (scaleX != Vector3<T>::Zero) scaleX.Normalize(aScale.x);
		if (scaleY != Vector3<T>::Zero) scaleY.Normalize(aScale.y);
		if (scaleZ != Vector3<T>::Zero) scaleZ.Normalize(aScale.z);

		myMatrix[0] = scaleX.x; myMatrix[1] = scaleX.y; myMatrix[2 ] = scaleX.z;
		myMatrix[4] = scaleY.x; myMatrix[5] = scaleY.y; myMatrix[6 ] = scaleY.z;
		myMatrix[8] = scaleZ.x; myMatrix[9] = scaleZ.y; myMatrix[10] = scaleZ.z;
	}

	template<typename T>
	constexpr auto Matrix4x4<T>::GetInverse() const -> Matrix4x4
	{
		const T* m = myMatrix.data();

		const T A2323 = m[10] * m[15] - m[11] * m[14];
		const T A1323 = m[9 ] * m[15] - m[11] * m[13];
		const T A1223 = m[9 ] * m[14] - m[10] * m[13];
		const T A0323 = m[8 ] * m[15] - m[11] * m[12];
		const T A0223 = m[8 ] * m[14] - m[10] * m[12];
		const T A0123 = m[8 ] * m[13] - m[9 ] * m[12];
		const T A2313 = m[6 ] * m[15] - m[7 ] * m[14];
		const T A1313 = m[5 ] * m[15] - m[7 ] * m[13];
		const T A1213 = m[5 ] * m[14] - m[6 ] * m[13];
		const T A2312 = m[6 ] * m[11] - m[7 ] * m[10];
		const T A1312 = m[5 ] * m[11] - m[7 ] * m[9 ];
		const T A1212 = m[5 ] * m[10] - m[6 ] * m[9 ];
		const T A0313 = m[4 ] * m[15] - m[7 ] * m[12];
		const T A0213 = m[4 ] * m[14] - m[6 ] * m[12];
		const T A0312 = m[4 ] * m[11] - m[7 ] * m[8 ];
		const T A0212 = m[4 ] * m[10] - m[6 ] * m[8 ];
		const T A0113 = m[4 ] * m[13] - m[5 ] * m[12];
		const T A0112 = m[4 ] * m[9 ] - m[5 ] * m[8 ];

		const T det = m[0] * (m[5] * A2323 - m[6] * A1323 + m[7] * A1223) -
					  m[1] * (m[4] * A2323 - m[6] * A0323 + m[7] * A0223) + 
					  m[2] * (m[4] * A1323 - m[5] * A0323 + m[7] * A0123) - 
					  m[3] * (m[4] * A1223 - m[5] * A0223 + m[6] * A0123);

		return (det != T(0)) ? Matrix4x4
		{
			 (m[5] * A2323 - m[6] * A1323 + m[7] * A1223) / det,
			-(m[1] * A2323 - m[2] * A1323 + m[3] * A1223) / det,
			 (m[1] * A2313 - m[2] * A1313 + m[3] * A1213) / det,
			-(m[1] * A2312 - m[2] * A1312 + m[3] * A1212) / det,
			-(m[4] * A2323 - m[6] * A0323 + m[7] * A0223) / det,
			 (m[0] * A2323 - m[2] * A0323 + m[3] * A0223) / det,
			-(m[0] * A2313 - m[2] * A0313 + m[3] * A0213) / det,
			 (m[0] * A2312 - m[2] * A0312 + m[3] * A0212) / det,
			 (m[4] * A1323 - m[5] * A0323 + m[7] * A0123) / det,
			-(m[0] * A1323 - m[1] * A0323 + m[3] * A0123) / det,
			 (m[0] * A1313 - m[1] * A0313 + m[3] * A0113) / det,
			-(m[0] * A1312 - m[1] * A0312 + m[3] * A0112) / det,
			-(m[4] * A1223 - m[5] * A0223 + m[6] * A0123) / det,
			 (m[0] * A1223 - m[1] * A0223 + m[2] * A0123) / det,
			-(m[0] * A1213 - m[1] * A0213 + m[2] * A0113) / det,
			 (m[0] * A1212 - m[1] * A0212 + m[2] * A0112) / det,
		} : Matrix4x4{};
	}
	template<typename T>
	constexpr auto Matrix4x4<T>::GetFastInverse() const -> Matrix4x4
	{
		const Vector3<T> s = GetScale();

		Vector3<T> row0{ myMatrix[0], myMatrix[1], myMatrix[2 ] };
		Vector3<T> row1{ myMatrix[4], myMatrix[5], myMatrix[6 ] };
		Vector3<T> row2{ myMatrix[8], myMatrix[9], myMatrix[10] };

		row0 = (s.x != T(0)) ? row0.GetNormalized(s.x, T(1)) : row0;
		row1 = (s.y != T(0)) ? row1.GetNormalized(s.y, T(1)) : row1;
		row2 = (s.z != T(0)) ? row2.GetNormalized(s.z, T(1)) : row2;

		const Vector3<T> is
		{
			(s.x != T(0)) ? T(1) / s.x : T(0),
			(s.y != T(0)) ? T(1) / s.y : T(0),
			(s.z != T(0)) ? T(1) / s.z : T(0)
		};

		const Matrix4x4 scalingMatrix
		{
			is.x,	0,		0,		0,
			0,		is.y,	0,		0,
			0,		0,		is.z,	0,
			0,		0,		0,		1
		};

		const Matrix4x4 rotationMatrix
		{
			row0.x,		row1.x,		row2.x,		0,
			row0.y,		row1.y,		row2.y,		0,
			row0.z,		row1.z,		row2.z, 	0,
			0,			0,			0,			1
		};

		Matrix4x4 inverseMatrix = rotationMatrix;

		const Vector3<T> ip = -GetTranslation();
		inverseMatrix.SetTranslation(inverseMatrix.TransformPoint(ip));

		return inverseMatrix.Combine(scalingMatrix);
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
		return Rotate(Quaternion<T>(aPitchYawRoll));
	}
	template<typename T>
	constexpr auto Matrix4x4<T>::Rotate(const Vector3<T>& aPitchYawRoll, AxisOrder aRotationOrder) -> Matrix4x4&
	{
		return Rotate(Quaternion<T>(aPitchYawRoll, aRotationOrder));
	}
	template<typename T>
	constexpr auto Matrix4x4<T>::Rotate(T aYaw, T aPitch, T aRoll) -> Matrix4x4&
	{
		return Rotate(Vector3<T>(aPitch, aYaw, aRoll));
	}
	template<typename T>
	constexpr auto Matrix4x4<T>::Rotate(T aYaw, T aPitch, T aRoll, AxisOrder aRotationOrder) -> Matrix4x4&
	{
		return Rotate(Vector3<T>(aPitch, aYaw, aRoll), aRotationOrder);
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
	constexpr AABB<T> Matrix4x4<T>::TransformAABB(const AABB<T>& aAABB) const
	{
		const std::array<Vector3<T>, 8> points = aAABB.GetPoints();

		Vector3<T> min = TransformPoint(points[0]);
		Vector3<T> max = TransformPoint(points[0]);

		for (std::size_t i = 1; i < points.size(); ++i)
		{
			const Vector3<T> point = TransformPoint(points[i]);

			if      (point.x < min.x) min.x = point.x;
			else if (point.x > max.x) max.x = point.x;
					 						  
			if      (point.y < min.y) min.y = point.y;
			else if (point.y > max.y) max.y = point.y;
					 						  
			if      (point.z < min.z) min.z = point.z;
			else if (point.z > max.z) max.z = point.z;
		}

		return AABB<T>(min, max);
	}

	template<typename T>
	constexpr Sphere<T> Matrix4x4<T>::TransformSphere(const Sphere<T>& aSphere) const
	{
		const Vector3<T> center		= TransformPoint(aSphere.GetCenter());

		const Vector3<T> scaleSqr	= GetScaleSqr();
		const T radius				= T(std::sqrt(Max(scaleSqr.x, scaleSqr.y, scaleSqr.z))) * aSphere.GetRadius();

		return Sphere<T>(center, radius);
	}

	template<typename T>
	constexpr auto Matrix4x4<T>::Add(const Matrix4x4& aRight) -> Matrix4x4&
	{
		for (int i = 0; i < 16; ++i)
		{
			myMatrix[i] += aRight[i];
		}

		return *this;
	}

	template<typename T>
	constexpr auto Matrix4x4<T>::Subtract(const Matrix4x4& aRight) -> Matrix4x4&
	{
		for (int i = 0; i < 16; ++i)
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
	constexpr auto Matrix4x4<T>::CreateFromRows(
		const Vector4<T>& aRow0, 
		const Vector4<T>& aRow1,
		const Vector4<T>& aRow2, 
		const Vector4<T>& aRow3) -> Matrix4x4
	{
		return Matrix4x4
		{
			aRow0.x, aRow0.y, aRow0.z, aRow0.w,
			aRow1.x, aRow1.y, aRow1.z, aRow1.w,
			aRow2.x, aRow2.y, aRow2.z, aRow2.w,
			aRow3.x, aRow3.y, aRow3.z, aRow3.w
		};
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

		const float hFOVRad = (float)aHorizontalFOVDeg * DEG2RAD;
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
	constexpr auto Matrix4x4<T>::CreateTRS(const Vector3<T>& aPosition, const Vector3<T>& aPitchYawRoll, const Vector3<T>& aScale, AxisOrder aRotationOrder) -> Matrix4x4
	{
		return Matrix4x4()
			.Scale(aScale)
			.Rotate(aPitchYawRoll, aRotationOrder)
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
	constexpr auto Matrix4x4<T>::CreateLookAt(const Vector3<T>& aEyePosition, const Vector3<T>& aDirection, const Vector3<T>& aUp)
	{
		const Vector3<T> xAxis = (aUp.GetAbs() != aDirection.GetAbs()) ? aUp.Cross(aDirection).GetNormalized() : Vector3<T>::Right;
		const Vector3<T> yAxis = aDirection.Cross(xAxis);

		return Matrix4x4<T>
		{
			xAxis.x,					yAxis.x,					aDirection.x,					0,
			xAxis.y,					yAxis.y,					aDirection.y,					0,
			xAxis.z,					yAxis.z,					aDirection.z,					0,
			-xAxis.Dot(aEyePosition),	-yAxis.Dot(aEyePosition),	-aDirection.Dot(aEyePosition),	1
		};
	}

	template<typename T>
	constexpr auto Matrix4x4<T>::CreateRotationMatrixFromQuaternion(const Quaternion<T>& aQuaternion) -> Matrix4x4
	{
		return CreateRotationMatrixFromNormalizedQuaternion(aQuaternion.GetNormalized());
	}
	template<typename T>
	constexpr auto Matrix4x4<T>::CreateRotationMatrixFromNormalizedQuaternion(const Quaternion<T>& aQuaternion) -> Matrix4x4
	{
		// assumes the quaternion to be normalized

		const T xx = aQuaternion.x * aQuaternion.x;
		const T xy = aQuaternion.x * aQuaternion.y;
		const T xz = aQuaternion.x * aQuaternion.z;
		const T xw = aQuaternion.x * aQuaternion.w;
		
		const T yy = aQuaternion.y * aQuaternion.y;
		const T yz = aQuaternion.y * aQuaternion.z;
		const T yw = aQuaternion.y * aQuaternion.w;
		
		const T zz = aQuaternion.z * aQuaternion.z;
		const T zw = aQuaternion.z * aQuaternion.w;
		
		const T a00 = T(1) - T(2) * (yy + zz);
		const T a01 = T(2) * (xy + zw);
		const T a02 = T(2) * (xz - yw);
		 
		const T a10 = T(2) * (xy - zw);
		const T a11 = T(1) - T(2) * (xx + zz);
		const T a12 = T(2) * (yz + xw);
		
		const T a20 = T(2) * (xz + yw);
		const T a21 = T(2) * (yz - xw);
		const T a22 = T(1) - T(2) * (xx + yy);

		return Matrix4x4<T> 
		{
			a00,	a01,	a02,	0,
			a10,	a11,	a12,	0,
			a20,	a21,	a22,	0,
			0,		0,		0,		1
		};
	}

	template<typename T>
	constexpr auto Matrix4x4<T>::BlendMatrices(const Matrix4x4& aFrom, const Matrix4x4& aTo, T aBlendFactor) -> Matrix4x4
	{
		Vector3<T> currentPosition;
		Quaternion<T> currentRotation;
		Vector3<T> currentScale;
		aFrom.DecomposeTransform(currentPosition, currentRotation, currentScale);

		Vector3<T> nextPosition;
		Quaternion<T> nextRotation;
		Vector3<T> nextScale;
		aTo.DecomposeTransform(nextPosition, nextRotation, nextScale);

		const Vector3<T> lerpPos		= cu::Vector3f::Lerp(currentPosition, nextPosition, aBlendFactor);
		const Quaternion<T> slerpRot	= cu::Quatf::Slerp(currentRotation, nextRotation, aBlendFactor);
		const Vector3<T> lerpScale		= cu::Vector3f::Lerp(currentScale, nextScale, aBlendFactor);

		return Matrix4x4<T>::CreateTRS(lerpPos, slerpRot, lerpScale);
	}

	template<typename T>
	constexpr auto Matrix4x4<T>::DominantEigen(const Matrix4x4& aMatrix, const Vector4<T>& aVector, int aIterations, float aEpsilon) -> Vector4<T>
	{
		Vector4<T> v = aVector;
		float ev = (aMatrix * v).x / v.x;
    
		for (int i = 0; i < aIterations; ++i)
		{
			Vector4<T> matV = aMatrix * v;
        
			Vector4<T> vNew = matV.GetNormalizedSafe();
			T evNew = (aMatrix * vNew).x / vNew.x;
       
			if (std::abs(ev - evNew) < aEpsilon)
			{
				break;
			}
        
			v = vNew;
			ev = evNew;
		}
    
		return v;
	}

	template<typename T>
	constexpr auto Matrix4x4<T>::Multiply(const Matrix4x4& aLeft, const Matrix4x4& aRight) -> Matrix4x4
	{
		// https://stackoverflow.com/a/18508113

		Matrix4x4 result;

		const T* a	= aLeft.GetData();
		const T* b	= aRight.GetData();
		T* c		= result.GetData();

		__m128 row1 = _mm_load_ps(&b[0]);
		__m128 row2 = _mm_load_ps(&b[4]);
		__m128 row3 = _mm_load_ps(&b[8]);
		__m128 row4 = _mm_load_ps(&b[12]);

		for (int i = 0; i < 4; i++)
		{
			__m128 brod1 = _mm_set_ps1(a[4 * i + 0]);
			__m128 brod2 = _mm_set_ps1(a[4 * i + 1]);
			__m128 brod3 = _mm_set_ps1(a[4 * i + 2]);
			__m128 brod4 = _mm_set_ps1(a[4 * i + 3]);

			__m128 row = _mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(brod1, row1),
					_mm_mul_ps(brod2, row2)),
				_mm_add_ps(
					_mm_mul_ps(brod3, row3),
					_mm_mul_ps(brod4, row4)));

			_mm_store_ps(&c[4 * i], row);
		}

		return result;
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
	NODISC constexpr Matrix4x4<T> operator*(const Matrix4x4<T>& aLeft, T aRight)
	{
		return Matrix4x4<T>
		{
			aLeft[0] * aRight,  aLeft[1] * aRight,  aLeft[2] * aRight,  aLeft[3] * aRight,
			aLeft[4] * aRight,  aLeft[5] * aRight,  aLeft[6] * aRight,  aLeft[7] * aRight,
			aLeft[8] * aRight,  aLeft[9] * aRight,  aLeft[10] * aRight, aLeft[11] * aRight,
			aLeft[12] * aRight, aLeft[13] * aRight, aLeft[14] * aRight, aLeft[15] * aRight
		};
	}
	template<typename T>
	NODISC constexpr Matrix4x4<T> operator*(T aLeft, const Matrix4x4<T>& aRight)
	{
		return aRight * aLeft;
	}

	template<typename T>
	NODISC constexpr bool operator==(const Matrix4x4<T>& aLeft, const Matrix4x4<T>& aRight)
	{
		for (int i = 0; i < 16; ++i)
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

	template <class T>
	constexpr std::ostream& operator<<(std::ostream& os, const Matrix4x4<T>& aMatrix)
	{
		os << "{ " << aMatrix(1, 1) << ", " << aMatrix(1, 2) << ", " << aMatrix(1, 3) << ", " << aMatrix(1, 4) << " }\n"
			<< "{ " << aMatrix(2, 1) << ", " << aMatrix(2, 2) << ", " << aMatrix(2, 3) << ", " << aMatrix(2, 4) << " }\n"
			<< "{ " << aMatrix(3, 1) << ", " << aMatrix(3, 2) << ", " << aMatrix(3, 3) << ", " << aMatrix(3, 4) << " }\n"
			<< "{ " << aMatrix(4, 1) << ", " << aMatrix(4, 2) << ", " << aMatrix(4, 3) << ", " << aMatrix(4, 4) << " }\n";
		return os;
	}

	template<typename T>
	inline const Matrix4x4<T> Matrix4x4<T>::IDENTITY;

	template<typename T>
	inline const Matrix4x4<T> Matrix4x4<T>::Zero(std::array<T, 16>{});

	// using declarations

	using Mat4f = Matrix4x4<float>;
	using Mat4d = Matrix4x4<double>;
	using Mat4i = Matrix4x4<int>;
	using Mat4u = Matrix4x4<unsigned>;
}

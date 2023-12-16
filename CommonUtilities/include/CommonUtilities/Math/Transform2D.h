#pragma once

#include "Matrix3x3.hpp"
#include "Vector2.hpp"

namespace CommonUtilities
{
	class Transform2D
	{
	public:
		Transform2D();
		~Transform2D();

		Transform2D(const Vector2f& aPosition, float aRotation, const Vector2f& aScale);
		Transform2D(const Vector2f& aPosition, const Vector2f& aScale);
		Transform2D(const Vector2f& aPosition, float aRotation);
		Transform2D(const Vector2f& aPosition);

		const Mat3f& GetMatrix() const;
		const Mat3f& GetInverseMatrix() const;

		const Vector2f& GetPosition() const noexcept;
		float GetRotation() const noexcept;
		const Vector2f& GetScale() const noexcept;

		void SetPosition(const Vector2f& aPosition);
		void SetRotation(float aRotation);
		void SetScale(const Vector2f& aScale);

		void Move(const Vector2f& aPosition);
		void Rotate(float aRotation);
		void Scale(const Vector2f& aScale);

	private:
		Vector2f		myPosition;
		Vector2f		myScale;
		float			myRotation				{0.0f};
		mutable Mat3f	myMatrix;
		mutable Mat3f	myInverseMatrix;
		mutable bool	myUpdateMatrix			{true};
		mutable bool	myUpdateInverseMatrix	{true};
	};
}
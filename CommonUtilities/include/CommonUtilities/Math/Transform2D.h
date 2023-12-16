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

		

	private:
		Mat3f			myMatrix;
		Mat3f			myInverseMatrix;
		Vector2f		myPosition;
		Vector2f		myScale;
		float			myRotation;
		mutable bool	myUpdateMatrix;
		mutable bool	myUpdateInverseMatrix;
	};
}
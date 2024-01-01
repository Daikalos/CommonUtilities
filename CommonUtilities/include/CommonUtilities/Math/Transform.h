#pragma once

#include "Matrix4x4.hpp"
#include "Vector3.hpp"

namespace CommonUtilities
{
	class Transform
	{
	public:
		Transform();
		~Transform();

		Transform(const Vector3f& aPosition, const Vector3f& aRotation, const Vector3f& aScale);
		Transform(const Vector3f& aPosition, const Vector3f& aRotation);
		Transform(const Vector3f& aPosition);

		const Mat4f& GetMatrix() const;
		const Mat4f& GetInverseMatrix() const;

		const Vector3f& GetPosition() const noexcept;
		const Vector3f& GetRotation() const noexcept;
		const Vector3f& GetScale() const noexcept;

		void SetPosition(const Vector3f& aPosition);
		void SetRotation(const Vector3f& aRotation);
		void SetScale(const Vector3f& aScale);

		void Move(const Vector3f& aPosition);
		void Rotate(const Vector3f& aRotation);
		void Scale(const Vector3f& aScale);

	private:
		Vector3f		myPosition;
		Vector3f		myScale;
		Vector3f		myRotation; // TODO: switch to quaternion in the future
		mutable Mat4f	myMatrix;
		mutable Mat4f	myInverseMatrix;
		mutable bool	myUpdateMatrix			{true};
		mutable bool	myUpdateInverseMatrix	{true};
	};
}
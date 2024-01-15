#pragma once

#include "Matrix4x4.hpp"
#include "Vector3.hpp"

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	class COMMON_UTILITIES_API Transform3D
	{
	public:
		Transform3D() = default;
		virtual ~Transform3D() = default;

		Transform3D(const Vector3f& aPosition, const Vector3f& aRotation, const Vector3f& aScale);
		Transform3D(const Vector3f& aPosition, const Vector3f& aRotation);
		Transform3D(const Vector3f& aPosition);

		NODISC const Mat4f& GetMatrix() const;
		NODISC const Mat4f& GetInverseMatrix() const;

		NODISC const Vector3f& GetPosition() const noexcept;
		NODISC const Vector3f& GetRotation() const noexcept;
		NODISC const Vector3f& GetScale() const noexcept;

		NODISC Vector3f ModelToWorld(const Vector3f& aModelPosition) const;
		NODISC Vector3f WorldToModel(const Vector3f& aWorldPosition) const;

		virtual void SetPosition(const Vector3f& aPosition);
		virtual void SetRotation(const Vector3f& aRotation);
		virtual void SetScale(const Vector3f& aScale);

		void Move(const Vector3f& aPosition);
		void Rotate(const Vector3f& aRotation);
		void Scale(const Vector3f& aScale);

	protected:
		Vector3f		myPosition;
		Vector3f		myRotation; // TODO: switch to quaternion in the future
		Vector3f		myScale					{1.0f, 1.0f, 1.0f};
		mutable Mat4f	myMatrix;
		mutable Mat4f	myInverseMatrix;
		mutable bool	myUpdateMatrix			{true};
		mutable bool	myUpdateInverseMatrix	{true};
	};
}
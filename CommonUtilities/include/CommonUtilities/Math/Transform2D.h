#pragma once

#include "Matrix3x3.hpp"
#include "Vector2.hpp"

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	class COMMON_UTILITIES_API Transform2D
	{
	public:
		Transform2D() = default;
		virtual ~Transform2D() = default;

		Transform2D(const Vector2f& aPosition, float aRotation, const Vector2f& aScale);
		Transform2D(const Vector2f& aPosition, const Vector2f& aScale);
		Transform2D(const Vector2f& aPosition, float aRotation);
		Transform2D(const Vector2f& aPosition);

		NODISC const Mat3f& GetMatrix() const;
		NODISC const Mat3f& GetInverseMatrix() const;

		NODISC const Vector2f& GetPosition() const noexcept;
		NODISC const Vector2f& GetOrigin() const noexcept;
		NODISC float GetRotation() const noexcept;
		NODISC const Vector2f& GetScale() const noexcept;
		NODISC const Vector2f& GetScaleMultiplier() const noexcept;

		NODISC Vector2f ModelToWorld(const Vector2f& aModelPosition) const;
		NODISC Vector2f WorldToModel(const Vector2f& aWorldPosition) const;

		virtual void SetPosition(const Vector2f& aPosition);
		virtual void SetOrigin(const Vector2f& aOrigin);
		virtual void SetRotation(float aRotation);
		virtual void SetScale(const Vector2f& aScale);
		virtual void SetScaleMultiplier(const Vector2f& aScaleMultiplier);

		void Move(const Vector2f& aPosition);
		void Rotate(float aRotation);
		void Scale(const Vector2f& aScale);

	protected:
		Vector2f		myPosition;
		Vector2f		myOrigin;
		float			myRotation				{0.0f};
		Vector2f		myScale					{1.0f, 1.0f};
		Vector2f		myScaleMultiplier		{1.0f, 1.0f};
		mutable Mat3f	myMatrix;
		mutable Mat3f	myInverseMatrix;
		mutable bool	myUpdateMatrix			{true};
		mutable bool	myUpdateInverseMatrix	{true};
	};
}
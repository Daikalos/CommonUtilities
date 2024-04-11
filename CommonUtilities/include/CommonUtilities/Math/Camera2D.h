#pragma once

#include <Windows.h>

#include <CommonUtilities/Config.h>

#include "Matrix3x3.hpp"
#include "Vector2.hpp"

namespace CommonUtilities
{
	class COMMON_UTILITIES_API Camera2D
	{
	public:
		Camera2D() = default;
		virtual ~Camera2D() = default; 

		NODISC const Vector2f& GetPosition() const noexcept;
		NODISC const Vector2f& GetScale() const noexcept;
		NODISC const Vector2f& GetSize() const noexcept;
		NODISC float GetRotation() const noexcept;

		NODISC virtual const Mat3f& GetWorldMatrix() const;
		NODISC virtual const Mat3f& GetViewMatrix() const;

		NODISC Vector2f ViewToWorld(const Vector2f& aPosition) const;
		NODISC Vector2f WorldToView(const Vector2f& aPosition) const;

		virtual void SetPosition(const Vector2f& aPosition);
		virtual void SetScale(const Vector2f& aScale);
		virtual void SetSize(const Vector2f& aSize);
		virtual void SetRotation(float aRotation);

		virtual void Move(const Vector2f& aPosition);
		virtual void Scale(const Vector2f& aScale);
		virtual void Rotate(float aRotation);

		virtual bool HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam);

	private:	
		Vector2f		myPosition;
		Vector2f		myScale			{1.0f, 1.0f};
		Vector2f		mySize;
		float			myRotation		{0.0f};

		mutable Mat3f	myViewMatrix;
		mutable Mat3f	myWorldMatrix;
		mutable bool	myUpdateView	{true};
		mutable bool	myUpdateWorld	{true};
	};
}
#pragma once

#include <Windows.h>

#include <CommonUtilities/System/Timer.h>
#include <CommonUtilities/Config.h>

#include "Matrix3x3.hpp"
#include "Vector2.hpp"

namespace CommonUtilities
{
	class COMMON_UTILITIES_API Camera2D
	{
	public:
		Camera2D();
		virtual ~Camera2D();

		NODISC const Vector2f& GetPosition() const noexcept;
		NODISC const Vector2f& GetScale() const noexcept;
		NODISC const Vector2f& GetSize() const noexcept;
		NODISC const float GetRotation() const noexcept;

		NODISC const Mat3f& GetViewMatrix() const noexcept;
		NODISC const Mat3f& GetWorldMatrix() const noexcept;

		NODISC Vector2f ViewToWorld(const Vector2f& aPosition);

		virtual void SetPosition(const Vector2f& aPosition);
		virtual void SetScale(const Vector2f& aScale);
		virtual void SetSize(const Vector2f& aSize);
		virtual void SetRotation(float aRotation);

		virtual void Update(Timer& aTimer) = 0;
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
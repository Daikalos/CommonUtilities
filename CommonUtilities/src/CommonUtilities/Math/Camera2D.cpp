#include <CommonUtilities/Math/Camera2D.h>

using namespace CommonUtilities;

Camera2D::Camera2D() = default;

Camera2D::~Camera2D() = default;

const Vector2f& Camera2D::GetPosition() const noexcept	{ return myPosition; }
const Vector2f& Camera2D::GetScale() const noexcept		{ return myScale; }
const Vector2f& Camera2D::GetSize() const noexcept		{ return mySize; }
const float Camera2D::GetRotation() const noexcept		{ return myRotation; }

const Mat3f& Camera2D::GetWorldMatrix() const
{
	if (myUpdateWorld)
	{
		myWorldMatrix = GetViewMatrix().FastInverse();
		myUpdateWorld = false;
	}

	return myWorldMatrix;
}
const Mat3f& Camera2D::GetViewMatrix() const
{
	if (myUpdateView)
	{
		//myViewMatrix = Mat3f()
		//	.Translate(mySize / (2.0f * myScale))
		//	.Rotate(myRotation)
		//	.Translate(-myPosition)
		//	.Scale(myScale);

		myViewMatrix = Mat3f()
			.Translate(-myPosition)
			.Rotate(myRotation)
			.Translate(mySize / (2.0f * myScale))
			.Scale(myScale);

		myUpdateView = false;
	}

	return myViewMatrix;
}

Vector2f Camera2D::ViewToWorld(const Vector2f& aPosition) const
{
	return GetWorldMatrix() * aPosition;
}

void Camera2D::SetPosition(const Vector2f& aPosition)
{
	if (myPosition != aPosition)
	{
		myPosition = aPosition;

		myUpdateView	= true;
		myUpdateWorld	= true;
	}
}
void Camera2D::SetScale(const Vector2f& aScale)
{
	if (myScale != aScale)
	{
		myScale = aScale;

		myUpdateView	= true;
		myUpdateWorld	= true;
	}
}
void Camera2D::SetSize(const Vector2f& aSize)
{
	if (mySize != aSize)
	{
		mySize = aSize;

		myUpdateView	= true;
		myUpdateWorld	= true;
	}
}
void Camera2D::SetRotation(float aRotation)
{
	if (myRotation != aRotation)
	{
		myRotation = aRotation;

		myUpdateView	= true;
		myUpdateWorld	= true;
	}
}

void Camera2D::Move(const Vector2f& aPosition)
{
	SetPosition(GetPosition() + aPosition);
}
void Camera2D::Scale(const Vector2f& aScale)
{
	SetScale(GetScale() + aScale);
}
void Camera2D::Rotate(float aRotation)
{
	SetRotation(GetRotation() + aRotation);
}

bool Camera2D::HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam)
{
	switch (aMessage)
	{
		case WM_SIZE:
		{
			UINT width	= LOWORD(lParam);
			UINT height = HIWORD(lParam);

			SetSize(cu::Vector2f((float)width, (float)height)); // update size of camera when window is resized

			break;
		}
	}
	return false;
}

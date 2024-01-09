#include <CommonUtilities/Math/Camera2D.h>

using namespace CommonUtilities;

Camera2D::Camera2D() = default;

Camera2D::~Camera2D() = default;

const Vector2f& Camera2D::GetPosition() const noexcept	{ return myPosition; }
const Vector2f& Camera2D::GetScale() const noexcept		{ return myScale; }
const Vector2f& Camera2D::GetSize() const noexcept		{ return mySize; }
const float Camera2D::GetRotation() const noexcept		{ return myRotation; }

const Mat3f& Camera2D::GetViewMatrix() const noexcept
{
	if (myUpdateView)
	{
		myViewMatrix = Mat3f()
			.Translate(myPosition)
			.Scale(1.0f / myScale)
			.Translate(mySize / -2.0f);

		myUpdateView = false;
	}

	return myViewMatrix;
}
const Mat3f& Camera2D::GetWorldMatrix() const noexcept
{
	if (myUpdateWorld)
	{
		myWorldMatrix = Mat3f()
			.Translate(mySize / 2.0f)
			.Scale(myScale)
			.Translate(-myPosition);

		myUpdateWorld = false;
	}

	return myWorldMatrix;
}

Vector2f Camera2D::ViewToWorld(const Vector2f& aPosition)
{
	return GetViewMatrix() * aPosition;
}

void Camera2D::SetPosition(const Vector2f& aPosition)
{
	if (myPosition != aPosition)
	{

	}
}
void Camera2D::SetScale(const Vector2f& aScale)
{
	if (myScale != aScale)
	{

	}
}
void Camera2D::SetSize(const Vector2f& aSize)
{
	if (mySize != aSize)
	{

	}
}
void Camera2D::SetRotation(float aRotation)
{
	if (myRotation != aRotation)
	{

	}
}

bool Camera2D::HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam)
{
	switch (aMessage)
	{
		case WM_SIZE:
		{

			break;
		}
	}
	return false;
}

#include <CommonUtilities/Math/Camera3D.h>

#include <cassert>

using namespace CommonUtilities;

const Transform3D& Camera3D::GetTransform() const noexcept	{ return myTransform; }
const Mat4f& Camera3D::GetProjection() const noexcept		{ return myProjectionMatrix; }
const Vector3f& Camera3D::GetPosition() const noexcept		{ return myTransform.GetPosition(); }
const Vector3f& Camera3D::GetRotation() const noexcept		{ return myTransform.GetRotation(); }
const float Camera3D::GetNearClip() const noexcept			{ return myNearClip; }
const float Camera3D::GetFarClip() const noexcept			{ return myFarClip; }

std::tuple<float, float> Camera3D::GetProjectionPlanes() const
{
	return std::make_tuple(myNearClip, myFarClip);
}

void Camera3D::SetOrtographicProjection(float aWidth, float aHeight, float aDepth)
{
	myProjectionMatrix = Mat4f::CreateOrtographic(aWidth, aHeight, aDepth);
}
void Camera3D::SetOrtographicProjection(float aLeft, float aRight, float aTop, float aBottom, float aNear, float aFar)
{
	myProjectionMatrix = Mat4f::CreateOrtographic(aLeft, aRight, aTop, aBottom, aNear, aFar);
}
void Camera3D::SetPerspectiveProjection(float aHorizontalFoV, const Vector2f& aResolution, float aNearClip, float aFarClip)
{
	myProjectionMatrix = Mat4f::CreatePerspective(aHorizontalFoV, aResolution.x / aResolution.y, aNearClip, aFarClip);

	myNearClip = aNearClip;
	myFarClip = aFarClip;
}

void Camera3D::SetTransform(const Transform3D& aTransform)
{
	myTransform = aTransform;
}
void Camera3D::SetPosition(const Vector3f& aPosition)
{
	myTransform.SetPosition(aPosition);
}

void Camera3D::SetRotation(const Vector3f& aRotation)
{
	myTransform.SetRotation(aRotation);
}

void Camera3D::Move(const Vector3f& aPosition)
{
	myTransform.Move(aPosition);
}
void Camera3D::Rotate(const Vector3f& aRotation)
{
	myTransform.Rotate(aRotation);
}

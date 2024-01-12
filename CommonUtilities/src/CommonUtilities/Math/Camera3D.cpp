#include <CommonUtilities/Math/Camera3D.h>

#include <cassert>

using namespace CommonUtilities;

Camera3D::Camera3D() = default;

Camera3D::~Camera3D() = default;

const Transform3D& Camera3D::GetTransform() const noexcept	{ return myTransform; }
const Vector3f& Camera3D::GetPosition() const noexcept		{ return myTransform.GetPosition(); }
const Vector3f& Camera3D::GetRotation() const noexcept		{ return myTransform.GetRotation(); }
const Mat4f& Camera3D::GetProjection() const				{ return myProjectionMatrix; }

std::tuple<float, float> Camera3D::GetProjectionPlanes() const
{
	return std::make_tuple(myNearPlane, myFarPlane);
}

void Camera3D::SetOrtographicProjection(float aWidth, float aHeight, float aDepth)
{
	myProjectionMatrix = Mat4f::CreateOrtographic(aWidth, aHeight, aDepth);
}
void Camera3D::SetOrtographicProjection(float aLeft, float aRight, float aTop, float aBottom, float aNear, float aFar)
{
	myProjectionMatrix = Mat4f::CreateOrtographic(aLeft, aRight, aTop, aBottom, aNear, aFar);
}
void Camera3D::SetPerspectiveProjection(float aHorizontalFoV, const Vector2f& aResolution, float aNearPlane, float aFarPlane)
{
	myProjectionMatrix = Mat4f::CreatePerspective(aHorizontalFoV, aResolution.x / aResolution.y, aNearPlane, aFarPlane);

	myNearPlane = aNearPlane;
	myFarPlane = aFarPlane;
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

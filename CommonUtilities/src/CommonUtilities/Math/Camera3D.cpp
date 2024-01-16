#include <CommonUtilities/Math/Camera3D.h>

#include <cassert>

using namespace CommonUtilities;

const Transform3D& Camera3D::GetTransform() const noexcept	{ return myTransform; }
const Mat4f& Camera3D::GetProjection() const noexcept		{ return myProjectionMatrix; }
const Vector3f& Camera3D::GetPosition() const noexcept		{ return myTransform.GetPosition(); }
const Vector3f& Camera3D::GetRotation() const noexcept		{ return myTransform.GetRotation(); }
const Vector2f& Camera3D::GetScreenSize() const noexcept	{ return myScreenSize; }
float Camera3D::GetNearClip() const noexcept				{ return myNearClip; }
float Camera3D::GetFarClip() const noexcept					{ return myFarClip; }

std::tuple<float, float> Camera3D::GetProjectionPlanes() const
{
	return std::make_tuple(myNearClip, myFarClip);
}

Vector3f Camera3D::WorldToView(const Vector3f& aWorldPosition) const
{
	return myTransform.GetInverseMatrix() * aWorldPosition;
}
Vector4f Camera3D::ViewToClip(const Vector3f& aViewPosition) const
{
	return myProjectionMatrix * aViewPosition.XYZW(1.0f);
}
Vector4f Camera3D::WorldToClip(const Vector3f& aWorldPosition) const
{
	return ViewToClip(WorldToView(aWorldPosition));
}
Vector3f Camera3D::ClipToNDC(const Vector4f& aClipPosition) const
{
	assert(aClipPosition.w != 0 && "Clip depth is zero!");
	return cu::Vector3f(aClipPosition.x, aClipPosition.y, aClipPosition.z) / aClipPosition.w;
}
Vector2f Camera3D::NDCToScreen(const Vector3f& aNDCPosition) const
{
	assert(myScreenSize != cu::Vector2f() && "Screen size is invalid");
	return myScreenSize * cu::Vector2f(aNDCPosition.x + 1.0f, aNDCPosition.y + 1.0f) / 2.0f;
}
Vector2f Camera3D::WorldToScreen(const Vector3f& aWorldPosition) const
{
	return NDCToScreen(ClipToNDC(WorldToClip(aWorldPosition)));
}

void Camera3D::SetOrtographicProjection(float aWidth, float aHeight, float aDepth)
{
	myProjectionMatrix = Mat4f::CreateOrtographic(aWidth, aHeight, aDepth);
}
void Camera3D::SetOrtographicProjection(float aLeft, float aRight, float aTop, float aBottom, float aNear, float aFar)
{
	myProjectionMatrix = Mat4f::CreateOrtographic(aLeft, aRight, aTop, aBottom, aNear, aFar);
}
void Camera3D::SetPerspectiveProjection(float aHorizontalFOVDeg, const Vector2f& aResolution, float aNearClip, float aFarClip)
{
	myProjectionMatrix = Mat4f::CreatePerspective(aHorizontalFOVDeg, aResolution.x / aResolution.y, aNearClip, aFarClip);

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

void Camera3D::SetScreenSize(const Vector2f& aScreenSize)
{
	myScreenSize = aScreenSize;
}

#include <CommonUtilities/Math/Transform3D.h>

using namespace CommonUtilities;

Transform3D::Transform3D() : Transform3D(cu::Vector3f(), cu::Vector3f(), cu::Vector3f(1.0f, 1.0f, 1.0f))
{

}

Transform3D::~Transform3D() = default;

Transform3D::Transform3D(const Vector3f& aPosition, const Vector3f& aRotation, const Vector3f& aScale)
	: myPosition(aPosition)
	, myScale(aScale)
	, myRotation(aRotation)
{

}

Transform3D::Transform3D(const Vector3f& aPosition, const Vector3f& aRotation)
	: Transform3D(aPosition, aRotation, cu::Vector3f(1.0f, 1.0f, 1.0f))
{

}

Transform3D::Transform3D(const Vector3f& aPosition)
	: Transform3D(aPosition, cu::Vector3f(), cu::Vector3f(1.0f, 1.0f, 1.0f))
{

}

const Mat4f& Transform3D::GetMatrix() const
{
	if (myUpdateMatrix)
	{
		myMatrix = Mat4f::CreateTRS(myPosition, myRotation, myScale);
		myUpdateMatrix = false;
	}
	return myMatrix;
}
const Mat4f& Transform3D::GetInverseMatrix() const
{
	if (myUpdateInverseMatrix)
	{
		myInverseMatrix = GetMatrix().FastInverse();
		myUpdateInverseMatrix = false;
	}
	return myInverseMatrix;
}

const Vector3f& Transform3D::GetPosition() const noexcept
{
	return myPosition;
}
const Vector3f& Transform3D::GetRotation() const noexcept
{
	return myRotation;
}
const Vector3f& Transform3D::GetScale() const noexcept
{
	return myScale;
}

void Transform3D::SetPosition(const Vector3f& aPosition)
{
	if (myPosition != aPosition)
	{
		myPosition = aPosition;
		myUpdateMatrix = true;
		myUpdateInverseMatrix = true;
	}
}
void Transform3D::SetRotation(const Vector3f& aRotation)
{
	if (myRotation != aRotation)
	{
		myRotation = aRotation;
		myUpdateMatrix = true;
		myUpdateInverseMatrix = true;
	}
}
void Transform3D::SetScale(const Vector3f& aScale)
{
	if (myScale != aScale)
	{
		myScale = aScale;
		myUpdateMatrix = true;
		myUpdateInverseMatrix = true;
	}
}

void Transform3D::Move(const Vector3f& aPosition)
{
	SetPosition(GetPosition() + aPosition);
}
void Transform3D::Rotate(const Vector3f& aRotation)
{
	SetRotation(GetRotation() + aRotation);
}
void Transform3D::Scale(const Vector3f& aScale)
{
	SetScale(GetScale() + aScale);
}
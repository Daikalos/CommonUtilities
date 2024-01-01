#include <CommonUtilities/Math/Transform.h>

using namespace CommonUtilities;

Transform::Transform() = default;

Transform::~Transform() = default;

Transform::Transform(const Vector3f& aPosition, const Vector3f& aRotation, const Vector3f& aScale)
	: myPosition(aPosition)
	, myRotation(aRotation)
	, myScale(aScale)
{

}

Transform::Transform(const Vector3f& aPosition, const Vector3f& aRotation)
	: myPosition(aPosition)
	, myRotation(aRotation)
{

}

Transform::Transform(const Vector3f& aPosition)
	: myPosition(aPosition)
{

}

const Mat4f& Transform::GetMatrix() const
{
	if (myUpdateMatrix)
	{
		myMatrix = Mat4f::CreateTRS(myPosition, myRotation, myScale);
		myUpdateMatrix = false;
	}
	return myMatrix;
}
const Mat4f& Transform::GetInverseMatrix() const
{
	if (myUpdateInverseMatrix)
	{
		myInverseMatrix = myMatrix.FastInverse();
		myUpdateInverseMatrix = false;
	}
	return myInverseMatrix;
}

const Vector3f& Transform::GetPosition() const noexcept
{
	return myPosition;
}
const Vector3f& Transform::GetRotation() const noexcept
{
	return myRotation;
}
const Vector3f& Transform::GetScale() const noexcept
{
	return myScale;
}

void Transform::SetPosition(const Vector3f& aPosition)
{
	if (myPosition != aPosition)
	{
		myPosition = aPosition;
		myUpdateMatrix = true;
		myUpdateInverseMatrix = true;
	}
}
void Transform::SetRotation(const Vector3f& aRotation)
{
	if (myRotation != aRotation)
	{
		myRotation = aRotation;
		myUpdateMatrix = true;
		myUpdateInverseMatrix = true;
	}
}
void Transform::SetScale(const Vector3f& aScale)
{
	if (myScale != aScale)
	{
		myScale = aScale;
		myUpdateMatrix = true;
		myUpdateInverseMatrix = true;
	}
}

void Transform::Move(const Vector3f& aPosition)
{
	SetPosition(GetPosition() + aPosition);
}
void Transform::Rotate(const Vector3f& aRotation)
{
	SetRotation(GetRotation() + aRotation);
}
void Transform::Scale(const Vector3f& aScale)
{
	SetScale(GetScale() + aScale);
}
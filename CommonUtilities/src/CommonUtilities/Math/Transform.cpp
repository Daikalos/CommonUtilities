#include <CommonUtilities/Math/Transform.h>

using namespace CommonUtilities;

Transform::Transform() : Transform(cu::Vector3f(), cu::Vector3f(), cu::Vector3f(1.0f, 1.0f, 1.0f))
{

}

Transform::~Transform() = default;

Transform::Transform(const Vector3f& aPosition, const Vector3f& aRotation, const Vector3f& aScale)
	: myPosition(aPosition)
	, myScale(aScale)
	, myRotation(aRotation)
{

}

Transform::Transform(const Vector3f& aPosition, const Vector3f& aRotation)
	: Transform(aPosition, aRotation, cu::Vector3f(1.0f, 1.0f, 1.0f))
{

}

Transform::Transform(const Vector3f& aPosition)
	: Transform(aPosition, cu::Vector3f(), cu::Vector3f(1.0f, 1.0f, 1.0f))
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
		myInverseMatrix = GetMatrix().FastInverse();
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
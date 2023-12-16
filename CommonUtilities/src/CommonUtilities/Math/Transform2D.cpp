#include <CommonUtilities/Math/Transform2D.h>

using namespace CommonUtilities;

Transform2D::Transform2D() = default;

Transform2D::~Transform2D() = default;

Transform2D::Transform2D(const Vector2f& aPosition, float aRotation, const Vector2f& aScale)
	: myPosition(aPosition)
	, myRotation(aRotation)
	, myScale(aScale)
{

}

Transform2D::Transform2D(const Vector2f& aPosition, const Vector2f& aScale)
	: myPosition(aPosition)
	, myScale(aScale)
{

}

Transform2D::Transform2D(const Vector2f& aPosition, float aRotation)
	: myPosition(aPosition)
	, myRotation(aRotation)
{

}

Transform2D::Transform2D(const Vector2f& aPosition)
	: myPosition(aPosition)
{

}

const Mat3f& Transform2D::GetMatrix() const
{
	if (myUpdateMatrix)
	{
		myMatrix = Mat3f::CreateTRS(myPosition, myRotation, myScale);
		myUpdateMatrix = false;
	}
	return myMatrix;
}
const Mat3f& Transform2D::GetInverseMatrix() const
{
	if (myUpdateInverseMatrix)
	{
		myInverseMatrix = myMatrix.FastInverse();
		myUpdateInverseMatrix = false;
	}
	return myInverseMatrix;
}

const Vector2f& Transform2D::GetPosition() const noexcept
{
	return myPosition;
}
float Transform2D::GetRotation() const noexcept
{
	return myRotation;
}
const Vector2f& Transform2D::GetScale() const noexcept
{
	return myScale;
}

void Transform2D::SetPosition(const Vector2f& aPosition)
{
	if (myPosition != aPosition)
	{
		myPosition = aPosition;
		myUpdateMatrix = true;
		myUpdateInverseMatrix = true;
	}
}
void Transform2D::SetRotation(float aRotation)
{
	if (myRotation != aRotation)
	{
		myRotation = aRotation;
		myUpdateMatrix = true;
		myUpdateInverseMatrix = true;
	}
}
void Transform2D::SetScale(const Vector2f& aScale)
{
	if (myScale != aScale)
	{
		myScale = aScale;
		myUpdateMatrix = true;
		myUpdateInverseMatrix = true;
	}
}

void Transform2D::Move(const Vector2f& aPosition)
{
	SetPosition(GetPosition() + aPosition);
}
void Transform2D::Rotate(float aRotation)
{
	SetRotation(GetRotation() + aRotation);
}
void Transform2D::Scale(const Vector2f& aScale)
{
	SetScale(GetScale() + aScale);
}

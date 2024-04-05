#include <CommonUtilities/pch.h>
#include <CommonUtilities/Math/Transform2D.h>

using namespace CommonUtilities;

Transform2D::Transform2D(const Vector2f& aPosition, float aRotation, const Vector2f& aScale)
	: myPosition(aPosition)
	, myRotation(aRotation)
	, myScale(aScale)
{

}

Transform2D::Transform2D(const Vector2f& aPosition, const Vector2f& aScale)
	: Transform2D(aPosition, 0.0f, aScale)
{

}

Transform2D::Transform2D(const Vector2f& aPosition, float aRotation)
	: Transform2D(aPosition, aRotation, Vector2f(1.0f, 1.0f))
{

}

Transform2D::Transform2D(const Vector2f& aPosition)
	: Transform2D(aPosition, 0.0f, Vector2f(1.0f, 1.0f))
{

}

const Mat3f& Transform2D::GetMatrix() const
{
	if (myUpdateMatrix)
	{
		myMatrix = Mat3f::CreateTRS(myPosition, myRotation, myScale, myScaleMultiplier, myOrigin);
		myUpdateMatrix = false;
	}
	return myMatrix;
}
const Mat3f& Transform2D::GetInverseMatrix() const
{
	if (myUpdateInverseMatrix)
	{
		myInverseMatrix = GetMatrix().GetFastInverse();
		myUpdateInverseMatrix = false;
	}
	return myInverseMatrix;
}

const Vector2f& Transform2D::GetPosition() const noexcept
{
	return myPosition;
}
const Vector2f& Transform2D::GetOrigin() const noexcept
{
	return myOrigin;
}
float Transform2D::GetRotation() const noexcept
{
	return myRotation;
}
const Vector2f& Transform2D::GetScale() const noexcept
{
	return myScale;
}
const Vector2f& Transform2D::GetScaleMultiplier() const noexcept
{
	return myScaleMultiplier;
}

Vector2f Transform2D::ModelToWorld(const Vector2f& aModelPosition) const
{
	return GetMatrix() * aModelPosition;
}
Vector2f Transform2D::WorldToModel(const Vector2f& aWorldPosition) const
{
	return GetInverseMatrix() * aWorldPosition;
}

void Transform2D::SetPosition(const Vector2f& aPosition)
{
	if (myPosition != aPosition)
	{
		myPosition				= aPosition;
		myUpdateMatrix			= true;
		myUpdateInverseMatrix	= true;
	}
}
void Transform2D::SetOrigin(const Vector2f& aOrigin)
{
	if (myOrigin != aOrigin)
	{
		myOrigin				= aOrigin;
		myUpdateMatrix			= true;
		myUpdateInverseMatrix	= true;
	}
}
void Transform2D::SetRotation(float aRotation)
{
	if (myRotation != aRotation)
	{
		myRotation				= aRotation;
		myUpdateMatrix			= true;
		myUpdateInverseMatrix	= true;
	}
}
void Transform2D::SetScale(const Vector2f& aScale)
{
	if (myScale != aScale)
	{
		myScale					= aScale;
		myUpdateMatrix			= true;
		myUpdateInverseMatrix	= true;
	}
}

void Transform2D::SetScaleMultiplier(const Vector2f& aScaleMultiplier)
{
	if (myScaleMultiplier != aScaleMultiplier)
	{
		myScaleMultiplier		= aScaleMultiplier;
		myUpdateMatrix			= true;
		myUpdateInverseMatrix	= true;
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

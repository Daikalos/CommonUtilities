#include <CommonUtilities/Math/Transform3D.h>

using namespace CommonUtilities;

Transform3D::Transform3D(const Vector3f& aPosition, const Quatf& aRotation, const Vector3f& aScale)
	: myPosition(aPosition)
	, myRotation(aRotation)
	, myScale(aScale)
{

}

Transform3D::Transform3D(const Vector3f& aPosition, const Vector3f& aRotation, const Vector3f& aScale, AxisOrder aRotationOrder)
	: myPosition(aPosition)
	, myRotation(aRotation, aRotationOrder)
	, myScale(aScale)
	, myRotationOrder(aRotationOrder)
{

}

Transform3D::Transform3D(const Vector3f& aPosition, const Vector3f& aRotation, AxisOrder aRotationOrder)
	: Transform3D(aPosition, aRotation, Vector3f(1.0f, 1.0f, 1.0f), aRotationOrder)
{

}

Transform3D::Transform3D(const Vector3f& aPosition, AxisOrder aRotationOrder)
	: Transform3D(aPosition, Vector3f(), Vector3f(1.0f, 1.0f, 1.0f), aRotationOrder)
{

}

Transform3D::Transform3D(AxisOrder aRotationOrder)
	: Transform3D(Vector3f(), Vector3f(), Vector3f(1.0f, 1.0f, 1.0f), aRotationOrder)
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
		myInverseMatrix = GetMatrix().GetFastInverse();
		myUpdateInverseMatrix = false;
	}
	return myInverseMatrix;
}

const Vector3f& Transform3D::GetPosition() const noexcept
{
	return myPosition;
}
const Quatf& Transform3D::GetRotation() const noexcept
{
	return myRotation;
}
const Vector3f& Transform3D::GetScale() const noexcept
{
	return myScale;
}
AxisOrder Transform3D::GetRotationOrder() const noexcept
{
	return myRotationOrder;
}

Vector3f Transform3D::ModelToWorld(const Vector3f& aModelPosition) const
{
	return GetMatrix() * aModelPosition;
}
Vector3f Transform3D::WorldToModel(const Vector3f& aWorldPosition) const
{
	return GetInverseMatrix() * aWorldPosition;
}

void Transform3D::SetPosition(const Vector3f& aPosition)
{
	if (myPosition != aPosition)
	{
		myPosition				= aPosition;
		myUpdateMatrix			= true;
		myUpdateInverseMatrix	= true;
	}
}
void Transform3D::SetRotation(const Quatf& aRotation)
{
	if (myRotation != aRotation)
	{
		myRotation				= aRotation;
		myUpdateMatrix			= true;
		myUpdateInverseMatrix	= true;
	}
}
void Transform3D::SetRotationDeg(const Vector3f& aDegrees)
{
	SetRotation(Quatf(aDegrees * DEG2RAD, myRotationOrder));
}
void Transform3D::SetScale(const Vector3f& aScale)
{
	if (myScale != aScale)
	{
		myScale					= aScale;
		myUpdateMatrix			= true;
		myUpdateInverseMatrix	= true;
	}
}
void Transform3D::SetRotationOrder(AxisOrder aRotationOrder)
{
	myRotationOrder = aRotationOrder;
}

void Transform3D::Move(const Vector3f& aPosition)
{
	SetPosition(GetPosition() + aPosition);
}
void Transform3D::Rotate(const Vector3f& aRotation)
{
	SetRotation(GetRotation() * Quatf(aRotation, myRotationOrder));
}
void Transform3D::RotateDeg(const Vector3f& aDegrees)
{
	Rotate(aDegrees * DEG2RAD);
}
void Transform3D::Scale(const Vector3f& aScale)
{
	SetScale(GetScale() + aScale);
}
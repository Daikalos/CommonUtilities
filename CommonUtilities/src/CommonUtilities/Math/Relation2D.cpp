#include <CommonUtilities/Math/Relation2D.h>

#include <CommonUtilities/Utility/ContainerUtils.hpp>
#include <stdexcept>

using namespace CommonUtilities;

Relation2D::Relation2D()
	: myParent()
	, myChildren()
	, myGlobalMatrix()
	, myInverseGlobalMatrix()
	, myUpdateGlobalMatrix(true)
	, myUpdateGlobalInverseMatrix(true)
{

}

Relation2D::~Relation2D() = default;

bool Relation2D::HasParent() const noexcept
{
	return myParent != nullptr;
}
bool Relation2D::HasChildren() const noexcept
{
	return !myChildren.empty();
}

auto Relation2D::GetRoot() const -> const Ref&
{
	if (myParent != nullptr)
	{
		return myParent->GetRoot();
	}

	return myParent;
}

auto Relation2D::GetParent() const -> const Parent&
{
	return myParent;
}
auto Relation2D::GetChildren() const -> const Children&
{
	return myChildren;
}

bool Relation2D::IsDescendant(const Ref& aRelation)
{
	for (const Ref& ref : myChildren)
	{
		if (ref == aRelation || ref->IsDescendant(aRelation))
		{
			return true;
		}
	}

	return false;
}

const Mat3f& Relation2D::GetGlobalMatrix() const
{
	if (myUpdateGlobalMatrix)
	{
		UpdateTransform();
		myUpdateGlobalMatrix = false;
	}

	return myGlobalMatrix;
}

const Mat3f& Relation2D::GetInverseGlobalMatrix() const
{
	if (myUpdateGlobalInverseMatrix)
	{
		myInverseGlobalMatrix = GetGlobalMatrix().FastInverse();
		myUpdateGlobalInverseMatrix = false;
	}

	return myInverseGlobalMatrix;
}

void Relation2D::SetPosition(const Vector2f& aPosition)
{
	if (myPosition != aPosition)
	{
		myPosition				= aPosition;
		myUpdateMatrix			= true;
		myUpdateInverseMatrix	= true;

		DirtyDescendants();
	}
}
void Relation2D::SetRotation(float aRotation)
{
	if (myRotation != aRotation)
	{
		myRotation				= aRotation;
		myUpdateMatrix			= true;
		myUpdateInverseMatrix	= true;

		DirtyDescendants();
	}
}
void Relation2D::SetScale(const Vector2f& aScale)
{
	if (myScale != aScale)
	{
		myScale					= aScale;
		myUpdateMatrix			= true;
		myUpdateInverseMatrix	= true;

		DirtyDescendants();
	}
}

void Relation2D::Attach(Ref aParent, Ref aChild)
{
	if (aParent == nullptr || aChild == nullptr)
		return;

	if (aChild->HasParent() && aChild->GetParent() == aParent) // child is already correctly parented
		return;

	if (aParent == aChild) // cant attach to itself
		return;

#if _DEBUG
	if (aParent->IsDescendant(aChild))
		throw std::runtime_error("The new aParent cannot be a descendant of the aChild");
#endif

	if (aParent->HasParent() && aParent->GetParent() == aChild) // special case
	{
		Detach(aParent->myParent, aParent);
		return;
	}

	if (aChild->HasParent()) // if aChild already has an attached aParent we need to detach it
	{
		Detach(aChild->myParent, aChild);
	}

	aChild->myParent = aParent;
	aChild->DirtyDescendants();

	aParent->myChildren.emplace_back(aChild);
}

bool Relation2D::Detach(Ref aParent, Ref aChild)
{
	if (aParent == nullptr || aChild == nullptr)
		return false;

	auto it = std::find_if(aParent->myChildren.begin(), aParent->myChildren.end(),
		[&aChild](const Relation2D::Ref& child)
		{
			return aChild == child;
		});

	if (it == aParent->myChildren.end()) // none found
		return false;

	aChild->myParent.reset();

	*it = aParent->myChildren.back();
	aParent->myChildren.pop_back();

	return true;
}

void Relation2D::CheckForNull()
{
	const Ref& root = GetRoot();
	CheckForNullImpl(root == nullptr ? *this : *root);
}

void Relation2D::UpdateTransform() const
{
	if (myParent == nullptr)
	{
		UpdateToLocal();
	}

	if (myParent->myUpdateGlobalMatrix)
	{
		myParent->UpdateTransform();
	}

	myGlobalMatrix = myParent->myGlobalMatrix * myMatrix;
}
void Relation2D::UpdateToLocal() const
{
	myGlobalMatrix = myMatrix;
}

void Relation2D::DirtyDescendants()
{
	if (myUpdateGlobalMatrix)
	{
		return;
	}

	const auto RecursiveDirty = [this](Relation2D& aRelation)
	{
		auto RecursiveImpl = [this](Relation2D& aRelation, auto& aRecursiveRef) mutable -> void
		{
			aRelation.myUpdateGlobalMatrix = true; // all of the children needs their global matrices to be updated
			aRelation.myUpdateGlobalInverseMatrix = true;

			for (const Ref& child : aRelation.GetChildren())
			{
				if (child != nullptr && !child->myUpdateMatrix)
				{
					aRecursiveRef(*child, aRecursiveRef);
				}
			}
		};

		return RecursiveImpl(aRelation, RecursiveImpl);
	};

	RecursiveDirty(*this);
}
void Relation2D::CheckForNullImpl(Relation2D& aCurrentRelation)
{
	const auto size = std::ssize(aCurrentRelation.myChildren);
	for (auto index = size - 1; index >= 0; --index)
	{
		const Ref& child = aCurrentRelation.myChildren[index];

		if (child == nullptr)
		{
			aCurrentRelation.myChildren.erase(aCurrentRelation.myChildren.begin() + index);
		}
		else
		{
			Relation2D::CheckForNullImpl(*child);
		}
	}
}
#include <CommonUtilities/Math/Relation3D.h>

#include <stdexcept>

using namespace CommonUtilities;

Relation3D::Relation3D(const Transform3D& aTransform) : Transform3D(aTransform)
{

}

Relation3D::~Relation3D()
{
	if (HasParent()) // detach itself from parent
	{
		auto parentPtr = myParent.lock();

		auto it = std::find_if(parentPtr->myChildren.begin(), parentPtr->myChildren.end(),
			[this](const typename Relation3D::Ref& child)
			{
				return !child.expired() && this == child.lock().get();
			});

		assert(it != parentPtr->myChildren.end()); // this should exist in parent

		*it = parentPtr->myChildren.back(); // erase cyclic
		parentPtr->myChildren.pop_back();
	}

	for (const typename Relation3D::Ref& child : myChildren) // detach itself from all children
	{
		if (child.expired())
		{
			continue;
		}

		const Relation3DPtr childPtr = child.lock();

		childPtr->myParent.reset();
		childPtr->DirtyDescendants();
	}
}

bool Relation3D::HasParent() const noexcept
{
	return !myParent.expired();
}
bool Relation3D::HasChildren() const noexcept
{
	return !myChildren.empty();
}

auto Relation3D::GetRoot() const -> const Ref&
{
	if (HasParent())
	{
		return myParent.lock()->GetRoot();
	}

	return myParent;
}

auto Relation3D::GetParent() const noexcept -> const Parent&
{
	return myParent;
}
auto Relation3D::GetChildren() const noexcept -> const Children&
{
	return myChildren;
}

bool Relation3D::IsDescendant(const Relation3DPtr& aRelation) const
{
	for (const Ref& child : myChildren)
	{
		if (!child.expired())
		{
			Relation3DPtr childPtr = child.lock();
			if (childPtr == aRelation || childPtr->IsDescendant(aRelation))
			{
				return true;
			}
		}
	}

	return false;
}

const Mat4f& Relation3D::GetGlobalMatrix() const
{
	if (myUpdateGlobalMatrix)
	{
		UpdateTransform();
	}

	return myGlobalMatrix;
}
const Mat4f& Relation3D::GetInverseGlobalMatrix() const
{
	if (myUpdateGlobalInverseMatrix)
	{
		myInverseGlobalMatrix = GetGlobalMatrix().GetFastInverse();
		myUpdateGlobalInverseMatrix = false;
	}

	return myInverseGlobalMatrix;
}

const Vector3f& Relation3D::GetGlobalPosition() const
{
	if (myUpdateGlobalPosition)
	{
		myGlobalPosition = GetGlobalMatrix().GetTranslation();
		myUpdateGlobalPosition = false;
	}

	return myGlobalPosition;
}
const Quatf& Relation3D::GetGlobalRotation() const
{
	if (myUpdateGlobalRotation)
	{
		myGlobalRotation = cu::Quatf(GetMatrix().GetRotationMatrix());
		myUpdateGlobalRotation = false;
	}

	return myGlobalRotation;
}
const Vector3f& Relation3D::GetGlobalScale() const
{
	if (myUpdateGlobalScale)
	{
		myGlobalScale = GetGlobalMatrix().GetScale();
		myUpdateGlobalScale = false;
	}

	return myGlobalScale;
}

Vector3f Relation3D::LocalToWorld(const Vector3f& aLocal) const
{
	return !HasParent() ? aLocal : GetParent().lock()->GetGlobalMatrix() * aLocal;
}

void Relation3D::SetPosition(const Vector3f& aPosition)
{
	if (myPosition != aPosition)
	{
		myPosition				= aPosition;
		myUpdateMatrix			= true;
		myUpdateInverseMatrix	= true;

		DirtyDescendants();
	}
}
void Relation3D::SetRotation(const Quatf& aRotation)
{
	if (myRotation != aRotation)
	{
		myRotation				= aRotation;
		myUpdateMatrix			= true;
		myUpdateInverseMatrix	= true;

		DirtyDescendants();
	}
}
void Relation3D::SetScale(const Vector3f& aScale)
{
	if (myScale != aScale)
	{
		myScale					= aScale;
		myUpdateMatrix			= true;
		myUpdateInverseMatrix	= true;

		DirtyDescendants();
	}
}

void Relation3D::SetGlobalPosition(const Vector3f& aPosition)
{
	if (!HasParent())
	{
		SetPosition(aPosition);
	}
	else
	{
		SetPosition(GetParent().lock()->GetInverseGlobalMatrix() * aPosition); // to parent-space
	}
}

void Relation3D::Attach(Relation3DPtr aParent, Relation3DPtr aChild)
{
	if (aParent == nullptr || aChild == nullptr)
		return;

	if (aChild->HasParent() && aChild->GetParent().lock() == aParent) // child is already correctly parented
		return;

	if (aParent == aChild) // cant attach to itself
		return;

#if _DEBUG
	if (aParent->IsDescendant(aChild))
		throw std::runtime_error("The new parent cannot be a descendant of the child");
#endif

	if (aParent->HasParent() && aParent->GetParent().lock() == aChild) // special weird case
	{
		Detach(aParent->GetParent().lock(), aParent);
		return;
	}

	if (aChild->HasParent()) // if child already has an attached parent we need to detach it
	{
		Detach(aChild->GetParent().lock(), aChild);
	}

	aChild->myParent = aParent;
	aParent->myChildren.emplace_back(aChild);

	aChild->DirtyDescendants();
}

bool Relation3D::Detach(Relation3DPtr aParent, Relation3DPtr aChild)
{
	if (aParent == nullptr || aChild == nullptr)
		return false;

	auto it = std::find_if(aParent->myChildren.begin(), aParent->myChildren.end(),
		[&aChild](const Relation3D::Ref& child)
		{
			return !child.expired() && aChild == child.lock();
		});

	if (it == aParent->myChildren.end()) // none found
		return false;

	aChild->myParent.reset();
	aChild->DirtyDescendants();

	*it = aParent->myChildren.back(); // erase cyclic
	aParent->myChildren.pop_back();

	return true;
}

void Relation3D::RemoveAllExpired()
{
	const Ref& root = GetRoot();
	RemoveAllExpiredImpl(root.expired() ? *this : *root.lock());
}

void Relation3D::UpdateTransform() const
{
	if (!HasParent())
	{
		UpdateToLocal();
		return;
	}

	Relation3DPtr parentPtr = myParent.lock();

	if (parentPtr->myUpdateGlobalMatrix)
	{
		parentPtr->UpdateTransform();
	}

	Mat4f combinedMatrix = parentPtr->GetGlobalMatrix() * GetMatrix();
	if (combinedMatrix != myGlobalMatrix)
	{
		myGlobalMatrix			= combinedMatrix;

		myUpdateGlobalPosition	= true; // we delay updating since they may be quite expensive
		myUpdateGlobalRotation	= true;
		myUpdateGlobalScale		= true;
	}

	myUpdateGlobalMatrix = false;
}
void Relation3D::UpdateToLocal() const
{
	const Mat4f& localMatrix = GetMatrix();
	if (myGlobalMatrix != localMatrix)
	{ 
		myGlobalMatrix			= localMatrix;

		myGlobalPosition		= myPosition;
		myGlobalRotation		= myRotation;
		myGlobalScale			= myScale;

		myUpdateGlobalPosition	= false;
		myUpdateGlobalRotation	= false;
		myUpdateGlobalScale		= false;
	}

	myUpdateGlobalMatrix = false;
}

void Relation3D::DirtyDescendants()
{
	if (myUpdateGlobalMatrix)
	{
		return;
	}

	const auto RecursiveDirty = [this](Relation3D& aRelation)
	{
		auto RecursiveImpl = [this](Relation3D& aRelation, auto& aRecursiveRef) mutable -> void
		{
			aRelation.myUpdateGlobalMatrix = true; // all of the children needs their global matrices to be updated
			aRelation.myUpdateGlobalInverseMatrix = true;

			for (const Ref& child : aRelation.GetChildren())
			{
				if (!child.expired())
				{
					Relation3DPtr childPtr = child.lock();
					if (!childPtr->myUpdateGlobalMatrix)
					{
						aRecursiveRef(*childPtr, aRecursiveRef);
					}
				}
			}
		};

		return RecursiveImpl(aRelation, RecursiveImpl);
	};

	RecursiveDirty(*this);
}
void Relation3D::RemoveAllExpiredImpl(Relation3D& aCurrentRelation)
{
	const auto size = std::ssize(aCurrentRelation.myChildren);
	for (auto index = size - 1; index >= 0; --index)
	{
		const Ref& child = aCurrentRelation.myChildren[index];

		if (child.expired())
		{
			aCurrentRelation.myChildren.erase(aCurrentRelation.myChildren.begin() + index);
		}
		else
		{
			RemoveAllExpiredImpl(*child.lock());
		}
	}
}
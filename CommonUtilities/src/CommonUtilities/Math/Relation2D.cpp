#include <CommonUtilities/Math/Relation2D.h>

#include <stdexcept>

using namespace CommonUtilities;

Relation2D::Relation2D() = default;

Relation2D::~Relation2D() = default;

bool Relation2D::HasParent() const noexcept
{
	return !myParent.expired();
}
bool Relation2D::HasChildren() const noexcept
{
	return !myChildren.empty();
}

auto Relation2D::GetRoot() const -> const Ref&
{
	if (HasParent())
	{
		return myParent.lock()->GetRoot();
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

bool Relation2D::IsDescendant(const std::shared_ptr<Relation2D>& aRelation)
{
	for (const Ref& child : myChildren)
	{
		if (!child.expired())
		{
			std::shared_ptr<Relation2D> childPtr = child.lock();
			if (childPtr == aRelation || childPtr->IsDescendant(aRelation))
			{
				return true;
			}
		}
	}

	return false;
}

const Mat3f& Relation2D::GetGlobalMatrix() const
{
	if (myUpdateGlobalMatrix)
	{
		UpdateTransform();
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

void Relation2D::Attach(std::shared_ptr<Relation2D> aParent, std::shared_ptr<Relation2D> aChild)
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
		Detach(aChild->myParent.lock(), aChild);
	}

	aChild->myParent = aParent;
	aParent->myChildren.emplace_back(aChild);

	aChild->DirtyDescendants();
}

bool Relation2D::Detach(std::shared_ptr<Relation2D> aParent, std::shared_ptr<Relation2D> aChild)
{
	if (aParent == nullptr || aChild == nullptr)
		return false;

	auto it = std::find_if(aParent->myChildren.begin(), aParent->myChildren.end(),
		[&aChild](const Relation2D::Ref& child)
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

void Relation2D::RemoveAllExpired()
{
	const Ref& root = GetRoot();
	RemoveAllExpiredImpl(root.expired() ? *this : *root.lock());
}

void Relation2D::UpdateTransform() const
{
	if (myParent.expired())
	{
		UpdateToLocal();
		return;
	}

	std::shared_ptr<Relation2D> parentPtr = myParent.lock();

	if (parentPtr->myUpdateGlobalMatrix)
	{
		parentPtr->UpdateTransform();
	}

	myGlobalMatrix = parentPtr->myGlobalMatrix * GetMatrix();
	myUpdateGlobalMatrix = false;
}
void Relation2D::UpdateToLocal() const
{
	myGlobalMatrix = GetMatrix();
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
				if (!child.expired())
				{
					std::shared_ptr<Relation2D> childPtr = child.lock();
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
void Relation2D::RemoveAllExpiredImpl(Relation2D& aCurrentRelation)
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
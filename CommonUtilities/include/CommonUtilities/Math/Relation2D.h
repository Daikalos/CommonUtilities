#pragma once

#include <vector>
#include <memory>
#include <cassert>

#include "Transform2D.h"

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	/// Relation2D is a way of modelling hierarchies but is slightly unsafe to use for now. Be sure to update this
	/// accordingly when objects are destroyed (use CheckIfDirty if unsure). Most getters and setters works in local
	/// space while GetGlobalMatrix retrieves the global representation for the current relation. Furthermore, do 
	/// note that when hierarchies start to grow this solution will be insufficient due to performance issues in trade-off 
	/// for intuitive interface. 
	/// 
	class Relation2D : public Transform2D
	{
	public:
		using Ref		= std::weak_ptr<Relation2D>;
		using Parent	= Ref;
		using Children	= std::vector<Ref>;

		Relation2D();
		~Relation2D();

		NODISC bool HasParent() const noexcept;
		NODISC bool HasChildren() const noexcept;

		NODISC auto GetRoot() const -> const Ref&;

		NODISC auto GetParent() const -> const Parent&;
		NODISC auto GetChildren() const -> const Children&;

		NODISC bool IsDescendant(const Relation2D& aRelation);

		NODISC const Mat3f& GetGlobalMatrix() const;
		NODISC const Mat3f& GetInverseGlobalMatrix() const;

		void SetPosition(const Vector2f& aPosition) override;
		void SetRotation(float aRotation) override;
		void SetScale(const Vector2f& aScale) override;

		static void Attach(std::shared_ptr<Relation2D> aParent, std::shared_ptr<Relation2D> aChild);
		static bool Detach(std::shared_ptr<Relation2D> aParent, std::shared_ptr<Relation2D> aChild);

		void CheckForNull();

	private:
		void UpdateTransform() const;
		void UpdateToLocal() const;

		void DirtyDescendants();

		static void CheckForNullImpl(Relation2D& aCurrentRelation);

		Parent			myParent;
		Children		myChildren;
		mutable Mat3f	myGlobalMatrix;
		mutable Mat3f	myInverseGlobalMatrix;
		mutable bool	myUpdateGlobalMatrix		{true};
		mutable bool	myUpdateGlobalInverseMatrix	{true};
	};
}
#pragma once

#include <vector>
#include <memory>
#include <cassert>

#include <CommonUtilities/Config.h>

#include "Transform3D.h"

namespace CommonUtilities
{
	class Relation3D;

	using Relation3DPtr = std::shared_ptr<Relation3D>;

	/// Relation3D is a way of modeling hierarchies. Most getters and setters works in local space while GetGlobalMatrix 
	/// retrieves the global representation for the current transform. You will have to store Relation3D somewhere outside as
	/// shared pointers for this to work (Relation3D uses weak pointers). Furthermore, do note that when hierarchies start to 
	/// grow this solution will be insufficient due to performance issues because of trade-off for an intuitive interface. 
	/// 
	class COMMON_UTILITIES_API Relation3D : public Transform3D
	{
	public:
		using Ref		= std::weak_ptr<Relation3D>;
		using Parent	= Ref;
		using Children	= std::vector<Ref>;

		~Relation3D();

		/// Creates a shared pointer of Relation3D.
		/// 
		/// \param someArgs: Additional constructor arguments (look at Transform2D for possible options)
		/// 
		template<typename... Args>
		NODISC static Relation3DPtr Create(Args&&... someArgs);

		NODISC bool HasParent() const noexcept;
		NODISC bool HasChildren() const noexcept;

		NODISC auto GetRoot() const -> const Ref&;

		NODISC auto GetParent() const noexcept -> const Parent&;
		NODISC auto GetChildren() const noexcept -> const Children&;

		NODISC bool IsDescendant(const Relation3DPtr& aRelation) const;

		NODISC const Mat4f& GetGlobalMatrix() const;
		NODISC const Mat4f& GetInverseGlobalMatrix() const;

		NODISC const Vector3f& GetGlobalPosition() const;
		NODISC const Vector3f& GetGlobalRotation() const;
		NODISC const Vector3f& GetGlobalScale() const;

		void SetPosition(const Vector3f& aPosition) override;
		void SetRotation(const Vector3f& aRotation) override;
		void SetScale(const Vector3f& aScale) override;

		void SetGlobalPosition(const Vector3f& aPosition);

		/// Attaches child to parent.
		/// 
		/// \param aParent: Parent to attach child to
		/// \param aChild: child to attach to parent
		/// 
		static void Attach(Relation3DPtr aParent, Relation3DPtr aChild);

		/// Detaches child from parent.
		/// 
		/// \param aParent: Parent to detach child from
		/// \param aChild: child to detach from parent
		/// 
		/// \returns Whether detachment was successful
		/// 
		static bool Detach(Relation3DPtr aParent, Relation3DPtr aChild);

		void RemoveAllExpired();

	private:
		using Transform3D::Transform3D; // bring its constructors into scope

		Relation3D(); // constructor is hidden to prevent wrong usage
		Relation3D(const Transform3D& aTransform);

		void UpdateTransform() const;
		void UpdateToLocal() const;

		void DirtyDescendants();

		static void RemoveAllExpiredImpl(Relation3D& aCurrentRelation);

		Parent					myParent;
		Children				myChildren;

		mutable Mat4f			myGlobalMatrix;
		mutable Mat4f			myInverseGlobalMatrix;

		mutable cu::Vector3f	myGlobalPosition;
		mutable cu::Vector3f	myGlobalRotation;
		mutable cu::Vector3f	myGlobalScale				{1.0f, 1.0f, 1.0f};

		mutable bool			myUpdateGlobalMatrix		{true};
		mutable bool			myUpdateGlobalInverseMatrix	{true};
		mutable bool			myUpdateGlobalRotation		{true};
		mutable bool			myUpdateGlobalScale			{true};
	};

	template<typename... Args>
	Relation3DPtr Relation3D::Create(Args&&... someArgs)
	{
		return Relation3DPtr(new Relation3D(std::forward<Args>(someArgs)...));
	}
}

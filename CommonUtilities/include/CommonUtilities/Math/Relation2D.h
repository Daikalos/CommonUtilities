#pragma once

#include <vector>
#include <memory>
#include <cassert>

#include <CommonUtilities/Config.h>

#include "Transform2D.h"

namespace CommonUtilities
{
	class Relation2D;

	using Relation2DPtr = std::shared_ptr<Relation2D>;

	/// Relation2D is a way of modeling hierarchies. Most getters and setters works in local space while GetGlobalMatrix 
	/// retrieves the global representation for the current transform. You will have to store Relation2D somewhere outside as
	/// shared pointers for this to work (Relation2D uses weak pointers). Furthermore, do note that when hierarchies start to 
	/// grow this solution will be insufficient due to performance issues because of trade-off for an intuitive interface. 
	/// 
	class COMMON_UTILITIES_API Relation2D : public Transform2D
	{
	public:
		using Ref		= std::weak_ptr<Relation2D>;
		using Parent	= Ref;
		using Children	= std::vector<Ref>;

		~Relation2D() = default;

		Relation2D& operator=(const Relation2D&) = default;
		Relation2D& operator=(Relation2D&&) noexcept = default;

		/// Creates a shared pointer of Relation2D.
		/// 
		/// \param someArgs: Additional constructor arguments (look at Transform2D for possible options)
		/// 
		template<typename... Args>
		NODISC static Relation2DPtr Instantiate(Args&&... someArgs);

		NODISC bool HasParent() const noexcept;
		NODISC bool HasChildren() const noexcept;

		NODISC auto GetRoot() const -> const Ref&;

		NODISC auto GetParent() const noexcept -> const Parent&;
		NODISC auto GetChildren() const noexcept -> const Children&;

		NODISC bool IsDescendant(const Relation2DPtr& aRelation) const;

		NODISC const Mat3f& GetGlobalMatrix() const;
		NODISC const Mat3f& GetInverseGlobalMatrix() const;

		NODISC const Vector2f& GetGlobalPosition() const;
		NODISC float GetGlobalRotation() const;
		NODISC const Vector2f& GetGlobalScale() const;

		void SetPosition(const Vector2f& aPosition) override;
		void SetRotation(float aRotation) override;
		void SetScale(const Vector2f& aScale) override;

		void SetGlobalPosition(const Vector2f& aPosition);

		/// Attaches child to parent.
		/// 
		/// \param aParent: Parent to attach child to
		/// \param aChild: child to attach to parent
		/// 
		static void Attach(Relation2DPtr aParent, Relation2DPtr aChild);

		/// Detaches child from parent.
		/// 
		/// \param aParent: Parent to detach child from
		/// \param aChild: child to detach from parent
		/// 
		/// \returns Whether detachment was successful
		/// 
		static bool Detach(Relation2DPtr aParent, Relation2DPtr aChild);

		void RemoveAllExpired();

	private:
		using Transform2D::Transform2D; // bring its constructors into scope

		Relation2D() = default; // constructor is hidden to prevent wrong usage

		Relation2D(const Relation2D&) = default;
		Relation2D(Relation2D&&) noexcept = default;

		Relation2D(const Transform2D& aTransform);

		void UpdateTransform() const;
		void UpdateToLocal() const;

		void DirtyDescendants();

		static void RemoveAllExpiredImpl(Relation2D& aCurrentRelation);

		Parent					myParent;
		Children				myChildren;

		mutable Mat3f			myGlobalMatrix;
		mutable Mat3f			myInverseGlobalMatrix;

		mutable cu::Vector2f	myGlobalPosition;
		mutable float			myGlobalRotation			{0.0f};
		mutable cu::Vector2f	myGlobalScale				{1.0f, 1.0f};

		mutable bool			myUpdateGlobalMatrix		{true};
		mutable bool			myUpdateGlobalInverseMatrix	{true};
		mutable bool			myUpdateGlobalRotation		{true};
		mutable bool			myUpdateGlobalScale			{true};
	};

	template<typename... Args>
	Relation2DPtr Relation2D::Instantiate(Args&&... someArgs)
	{
		return Relation2DPtr(new Relation2D(std::forward<Args>(someArgs)...));
	}
}
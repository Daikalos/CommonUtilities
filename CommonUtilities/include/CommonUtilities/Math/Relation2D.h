#pragma once

#include <vector>
#include <memory>
#include <cassert>

#include "Transform2D.h"

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	/// Relation2D is a way of modelling hierarchies. Most getters and setters works in local space while GetGlobalMatrix 
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

		~Relation2D();

		/// Creates a shared pointer of Relation2D.
		/// 
		/// \param someArgs: Additional constructor arguments (look at Transform2D for possible options)
		/// 
		template<typename... Args>
		NODISC static std::shared_ptr<Relation2D> Create(Args&&... someArgs);

		NODISC bool HasParent() const noexcept;
		NODISC bool HasChildren() const noexcept;

		NODISC auto GetRoot() const -> const Ref&;

		NODISC auto GetParent() const -> const Parent&;
		NODISC auto GetChildren() const -> const Children&;

		NODISC bool IsDescendant(const std::shared_ptr<Relation2D>& aRelation);

		NODISC const Mat3f& GetGlobalMatrix() const;
		NODISC const Mat3f& GetInverseGlobalMatrix() const;

		void SetPosition(const Vector2f& aPosition) override;
		void SetRotation(float aRotation) override;
		void SetScale(const Vector2f& aScale) override;

		static void Attach(std::shared_ptr<Relation2D> aParent, std::shared_ptr<Relation2D> aChild);
		static bool Detach(std::shared_ptr<Relation2D> aParent, std::shared_ptr<Relation2D> aChild);

		void RemoveAllExpired();

	private:
		using Transform2D::Transform2D; // bring its constructors into scope

		Relation2D(); // constructor is hidden to prevent wrong usage

		void UpdateTransform() const;
		void UpdateToLocal() const;

		void DirtyDescendants();

		static void RemoveAllExpiredImpl(Relation2D& aCurrentRelation);

		Parent			myParent;
		Children		myChildren;
		mutable Mat3f	myGlobalMatrix;
		mutable Mat3f	myInverseGlobalMatrix;
		mutable bool	myUpdateGlobalMatrix		{true};
		mutable bool	myUpdateGlobalInverseMatrix	{true};
	};

	template<typename... Args>
	std::shared_ptr<Relation2D> Relation2D::Create(Args&&... someArgs)
	{
		return std::shared_ptr<Relation2D>(new Relation2D(std::forward<Args>(someArgs)...));
	}

	using Relation2DPtr = std::shared_ptr<Relation2D>;
}
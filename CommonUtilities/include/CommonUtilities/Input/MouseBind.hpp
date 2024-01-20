#pragma once

#include "Binds.hpp"
#include "MouseInput.h"

namespace CommonUtilities
{
	template<typename Bind> requires (!std::same_as<Bind, Mouse::Button>)
	class MouseBind final : public Binds<Bind, Mouse::Button>
	{
	public:
		using ButtonType = Bind;

		MouseBind() = default;
		~MouseBind() = default;

		explicit MouseBind(const MouseInput& aMouse);

		NODISC const MouseInput& GetMouse() const;
		NODISC MouseInput& GetMouse();

		NODISC bool IsHeld(const ButtonType& aBind) const;
		NODISC bool IsPressed(const ButtonType& aBind) const;
		NODISC bool IsReleased(const ButtonType& aBind) const;

	private:
		const MouseInput* myMouse {nullptr};
	};

	template<typename Bind> requires (!std::same_as<Bind, Mouse::Button>)
	inline MouseBind<Bind>::MouseBind(const MouseInput& aMouse)
		: myMouse(&aMouse) { }

	template<typename Bind>
	inline const MouseInput& MouseBind<Bind>::GetMouse() const
	{
		return *myMouse;
	}
	template<typename Bind>
	inline MouseInput& MouseBind<Bind>::GetMouse()
	{
		return *myMouse;
	}

	template<typename Bind> requires (!std::same_as<Bind, Mouse::Button>)
	inline bool MouseBind<Bind>::IsHeld(const ButtonType& aBind) const
	{
		return this->GetEnabled() && GetMouse().IsHeld(this->At(aBind));
	}

	template<typename Bind> requires (!std::same_as<Bind, Mouse::Button>)
	inline bool MouseBind<Bind>::IsPressed(const ButtonType& aBind) const
	{
		return this->GetEnabled() && GetMouse().IsPressed(this->At(aBind));
	}

	template<typename Bind> requires (!std::same_as<Bind, Mouse::Button>)
	inline bool MouseBind<Bind>::IsReleased(const ButtonType& aBind) const
	{
		return this->GetEnabled() && GetMouse().IsReleased(this->At(aBind));
	}
}
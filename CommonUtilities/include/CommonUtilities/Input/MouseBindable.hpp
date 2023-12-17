#pragma once

#include "Binds.hpp"
#include "MouseInput.h"

namespace CommonUtilities
{
	template<typename Bind> requires (!std::same_as<Bind, Mouse::Button>)
	class MouseBindable : public Binds<Bind, Mouse::Button>
	{
	public:
		using ButtonType = Bind;

		explicit MouseBindable(const MouseInput& aMouse);
		~MouseBindable();

		NODISC const MouseInput& GetMouse() const;
		NODISC MouseInput& GetMouse();

		NODISC bool Held(const ButtonType& aBind) const;
		NODISC bool Pressed(const ButtonType& aBind) const;
		NODISC bool Released(const ButtonType& aBind) const;

	private:
		const MouseInput* myMouse {nullptr};
	};

	template<typename Bind> requires (!std::same_as<Bind, Mouse::Button>)
	inline MouseBindable<Bind>::MouseBindable(const MouseInput& aMouse)
		: myMouse(&aMouse) { }

	template<typename Bind>
	inline MouseBindable<Bind>::~MouseBindable() = default;

	template<typename Bind>
	inline const MouseInput& MouseBindable<Bind>::GetMouse() const
	{
		return *myMouse;
	}
	template<typename Bind>
	inline MouseInput& MouseBindable<Bind>::GetMouse()
	{
		return *myMouse;
	}

	template<typename Bind> requires (!std::same_as<Bind, Mouse::Button>)
	inline bool MouseBindable<Bind>::Held(const ButtonType& aBind) const
	{
		return this->GetEnabled() && GetMouse().IsHeld(this->At(aBind));
	}

	template<typename Bind> requires (!std::same_as<Bind, Mouse::Button>)
	inline bool MouseBindable<Bind>::Pressed(const ButtonType& aBind) const
	{
		return this->GetEnabled() && GetMouse().IsPressed(this->At(aBind));
	}

	template<typename Bind> requires (!std::same_as<Bind, Mouse::Button>)
	inline bool MouseBindable<Bind>::Released(const ButtonType& aBind) const
	{
		return this->GetEnabled() && GetMouse().IsReleased(this->At(aBind));
	}
}
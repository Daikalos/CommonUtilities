#pragma once

#include "Binds.hpp"
#include "KeyboardInput.h"

namespace CommonUtilities
{
	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key>)
	class KeyboardBindable final : public Binds<Bind, Keyboard::Key>
	{
	public:
		using ButtonType = Bind;

		explicit KeyboardBindable(const KeyboardInput& aKeyboard);
		~KeyboardBindable();

		NODISC const KeyboardInput& GetKeyboard() const;
		NODISC KeyboardInput& GetKeyboard();

		NODISC bool IsHeld(const ButtonType& aBind) const;
		NODISC bool IsPressed(const ButtonType& aBind) const;
		NODISC bool IsReleased(const ButtonType& aBind) const;

	private:
		const KeyboardInput* myKeyboard {nullptr};
	};

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key>)
	inline KeyboardBindable<Bind>::KeyboardBindable(const KeyboardInput& aKeyboard)
		: myKeyboard(&aKeyboard) 
	{ 

	}

	template<typename Bind>
	inline KeyboardBindable<Bind>::~KeyboardBindable() = default;

	template<typename Bind>
	inline const KeyboardInput& KeyboardBindable<Bind>::GetKeyboard() const
	{
		return *myKeyboard;
	}
	template<typename Bind>
	inline KeyboardInput& KeyboardBindable<Bind>::GetKeyboard()
	{
		return *myKeyboard;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key>)
	inline bool KeyboardBindable<Bind>::IsHeld(const ButtonType& aBind) const
	{
		return this->GetEnabled() && GetKeyboard().IsHeld(this->At(aBind));
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key>)
	inline bool KeyboardBindable<Bind>::IsPressed(const ButtonType& aBind) const
	{
		return this->GetEnabled() && GetKeyboard().IsPressed(this->At(aBind));
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key>)
	inline bool KeyboardBindable<Bind>::IsReleased(const ButtonType& aBind) const
	{
		return this->GetEnabled() && GetKeyboard().IsReleased(this->At(aBind));
	}
}
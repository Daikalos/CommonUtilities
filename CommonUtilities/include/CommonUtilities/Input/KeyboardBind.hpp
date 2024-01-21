#pragma once

#include "Binds.hpp"
#include "KeyboardInput.h"

namespace CommonUtilities
{
	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key>)
	class KeyboardBind final : public Binds<Bind, Keyboard::Key>
	{
	public:
		using ButtonType = Bind;

		KeyboardBind(const KeyboardInput* aKeyboard = nullptr);
		~KeyboardBind() = default;

		NODISC const KeyboardInput* Keyboard() const noexcept;
		NODISC KeyboardInput* Keyboard() noexcept;

		NODISC bool IsConnected() const noexcept;

		void Connect(const KeyboardInput& aKeyboard);
		void Disconnect();

		NODISC bool IsHeld(const ButtonType& aBind) const;
		NODISC bool IsPressed(const ButtonType& aBind) const;
		NODISC bool IsReleased(const ButtonType& aBind) const;

	private:
		const KeyboardInput* myKeyboard {nullptr};
	};

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key>)
	inline KeyboardBind<Bind>::KeyboardBind(const KeyboardInput* aKeyboard)
		: myKeyboard(aKeyboard) 
	{ 

	}

	template<typename Bind>
	inline const KeyboardInput* KeyboardBind<Bind>::Keyboard() const noexcept
	{
		return myKeyboard;
	}
	template<typename Bind>
	inline KeyboardInput* KeyboardBind<Bind>::Keyboard() noexcept
	{
		return myKeyboard;
	}

	template<typename Bind>
	inline bool KeyboardBind<Bind>::IsConnected() const noexcept
	{
		return myKeyboard != nullptr;
	}

	template<typename Bind>
	inline void KeyboardBind<Bind>::Connect(const KeyboardInput& aKeyboard)
	{
		myKeyboard = &aKeyboard;
	}
	template<typename Bind>
	inline void KeyboardBind<Bind>::Disconnect()
	{
		myKeyboard = nullptr;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key>)
	inline bool KeyboardBind<Bind>::IsHeld(const ButtonType& aBind) const
	{
		return this->GetEnabled() && IsConnected() && myKeyboard->IsHeld(this->At(aBind));
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key>)
	inline bool KeyboardBind<Bind>::IsPressed(const ButtonType& aBind) const
	{
		return this->GetEnabled() && IsConnected() && myKeyboard->IsPressed(this->At(aBind));
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key>)
	inline bool KeyboardBind<Bind>::IsReleased(const ButtonType& aBind) const
	{
		return this->GetEnabled() && IsConnected() && myKeyboard->IsReleased(this->At(aBind));
	}
}
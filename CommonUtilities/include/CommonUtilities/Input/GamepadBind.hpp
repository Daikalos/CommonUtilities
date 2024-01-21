#pragma once

#include "Binds.hpp"
#include "GamepadInput.h"

namespace CommonUtilities
{
	template<typename Bind> requires (!std::same_as<Bind, Gamepad::Button>)
	class GamepadBind final : public Binds<Bind, Gamepad::Button>
	{
	public:
		using ButtonType = Bind;

		explicit GamepadBind(const GamepadInput* aGamepad = nullptr);
		~GamepadBind() = default;

		NODISC const GamepadInput* Gamepad() const noexcept;
		NODISC GamepadInput* Gamepad() noexcept;

		NODISC bool IsConnected() const noexcept;

		void Connect(const GamepadInput& aGamepad);
		void Disconnect();

		NODISC bool IsHeld(const ButtonType& aBind) const;
		NODISC bool IsPressed(const ButtonType& aBind) const;
		NODISC bool IsReleased(const ButtonType& aBind) const;

	private:
		const GamepadInput* myGamepad {nullptr};
	};

	template<typename Bind> requires (!std::same_as<Bind, Gamepad::Button>)
	inline GamepadBind<Bind>::GamepadBind(const GamepadInput* aGamepad)
		: myGamepad(aGamepad)
	{ 

	}

	template<typename Bind>
	inline const GamepadInput* GamepadBind<Bind>::Gamepad() const noexcept
	{
		return myGamepad;
	}
	template<typename Bind>
	inline GamepadInput* GamepadBind<Bind>::Gamepad() noexcept
	{
		return myGamepad;
	}

	template<typename Bind>
	inline bool GamepadBind<Bind>::IsConnected() const noexcept
	{
		return myGamepad != nullptr;
	}

	template<typename Bind>
	inline void GamepadBind<Bind>::Connect(const GamepadInput& aGamepad)
	{
		myGamepad = &aGamepad;
	}
	template<typename Bind>
	inline void GamepadBind<Bind>::Disconnect()
	{
		myGamepad = nullptr;
	}

	template<typename Bind> requires (!std::same_as<Bind, Gamepad::Button>)
	inline bool GamepadBind<Bind>::IsHeld(const ButtonType& aBind) const
	{
		return this->GetEnabled() && IsConnected() && myGamepad->IsHeld(this->At(aBind));
	}

	template<typename Bind> requires (!std::same_as<Bind, Gamepad::Button>)
	inline bool GamepadBind<Bind>::IsPressed(const ButtonType& aBind) const
	{
		return this->GetEnabled() && IsConnected() && myGamepad->IsPressed(this->At(aBind));
	}

	template<typename Bind> requires (!std::same_as<Bind, Gamepad::Button>)
	inline bool GamepadBind<Bind>::IsReleased(const ButtonType& aBind) const
	{
		return this->GetEnabled() && IsConnected() && myGamepad->IsReleased(this->At(aBind));
	}
}
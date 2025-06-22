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

		explicit GamepadBind(GamepadInput* aGamepad = nullptr);
		~GamepadBind() = default;

		NODISC const GamepadInput* Gamepad() const noexcept;
		NODISC GamepadInput* Gamepad() noexcept;

		NODISC bool IsConnected() const noexcept;

		void Connect(GamepadInput& aGamepad);
		void Disconnect();

		NODISC bool IsHeld(const ButtonType& aBind) const;
		NODISC bool IsPressed(const ButtonType& aBind) const;
		NODISC bool IsReleased(const ButtonType& aBind) const;

		NODISC bool IsAnyPressed() const;

	private:
		GamepadInput* myGamepad {nullptr};
	};

	template<typename Bind> requires (!std::same_as<Bind, Gamepad::Button>)
	inline GamepadBind<Bind>::GamepadBind(GamepadInput* aGamepad)
		: myGamepad(aGamepad)
	{ 

	}

	template<typename Bind> requires (!std::same_as<Bind, Gamepad::Button>)
	inline const GamepadInput* GamepadBind<Bind>::Gamepad() const noexcept
	{
		return myGamepad;
	}
	template<typename Bind> requires (!std::same_as<Bind, Gamepad::Button>)
	inline GamepadInput* GamepadBind<Bind>::Gamepad() noexcept
	{
		return myGamepad;
	}

	template<typename Bind> requires (!std::same_as<Bind, Gamepad::Button>)
	inline bool GamepadBind<Bind>::IsConnected() const noexcept
	{
		return myGamepad != nullptr && myGamepad->IsConnected();
	}

	template<typename Bind> requires (!std::same_as<Bind, Gamepad::Button>)
	inline void GamepadBind<Bind>::Connect(GamepadInput& aGamepad)
	{
		myGamepad = &aGamepad;
	}
	template<typename Bind> requires (!std::same_as<Bind, Gamepad::Button>)
	inline void GamepadBind<Bind>::Disconnect()
	{
		myGamepad = nullptr;
	}

	template<typename Bind> requires (!std::same_as<Bind, Gamepad::Button>)
	inline bool GamepadBind<Bind>::IsHeld(const ButtonType& aBind) const
	{
		if (!this->GetEnabled() || !IsConnected())
			return false;

		auto range = this->At(aBind);

		for (auto it = range.first; it != range.second; ++it)
		{
			if (myGamepad->IsHeld(it->second))
			{
				return true;
			}
		}

		return false;
	}

	template<typename Bind> requires (!std::same_as<Bind, Gamepad::Button>)
	inline bool GamepadBind<Bind>::IsPressed(const ButtonType& aBind) const
	{
		if (!this->GetEnabled() || !IsConnected())
			return false;

		auto range = this->At(aBind);

		for (auto it = range.first; it != range.second; ++it)
		{
			if (myGamepad->IsPressed(it->second))
			{
				return true;
			}
		}

		return false;
	}

	template<typename Bind> requires (!std::same_as<Bind, Gamepad::Button>)
	inline bool GamepadBind<Bind>::IsReleased(const ButtonType& aBind) const
	{
		if (!this->GetEnabled() || !IsConnected())
			return false;

		auto range = this->At(aBind);

		for (auto it = range.first; it != range.second; ++it)
		{
			if (myGamepad->IsReleased(it->second))
			{
				return true;
			}
		}

		return false;
	}

	template<typename Bind> requires (!std::same_as<Bind, Gamepad::Button>)
	inline bool GamepadBind<Bind>::IsAnyPressed() const
	{
		if (!this->GetEnabled() || !IsConnected())
			return false;

		return myGamepad->IsAnyPressed();
	}
}
#pragma once

#include <array>

#include <CommonUtilities/Input/InputHandler.h>
#include <CommonUtilities/Input/Keyboard.h>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	class COMMON_UTILITIES_API KeyboardInput final : public InputHandler
	{
	public:
		using ButtonType = Keyboard::Key;

		KeyboardInput();
		~KeyboardInput();

		NODISC bool IsHeld(ButtonType aKey) const;
		NODISC bool IsPressed(ButtonType aKey) const;
		NODISC bool IsReleased(ButtonType aKey) const;

		void Update() override;
		bool HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam) override;

	private:
		bool SetTentativeState(WPARAM wParam, bool aState);

		std::array<bool, Keyboard::KeyCount> myCurrentState		= {false};
		std::array<bool, Keyboard::KeyCount> myPreviousState	= {false};
		std::array<bool, Keyboard::KeyCount> myTentativeState	= {false};
	};
}
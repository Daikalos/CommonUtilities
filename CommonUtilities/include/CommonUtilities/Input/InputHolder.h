#pragma once

#include <CommonUtilities/Utility/NonCopyable.h>
#include <CommonUtilities/Config.h>

#include "KeyboardInput.h"
#include "MouseInput.h"
#include "MouseCursor.h"
#include "GamepadInput.h"

namespace CommonUtilities
{
	class COMMON_UTILITIES_API InputHolder final : private NonCopyable
	{
	public:
		InputHolder() = default;
		~InputHolder() = default;

		NODISC const KeyboardInput& Keyboard() const noexcept;
		NODISC KeyboardInput& Keyboard() noexcept;

		NODISC const MouseInput& Mouse() const noexcept;
		NODISC MouseInput& Mouse() noexcept;

		NODISC const MouseCursor& Cursor() const noexcept;
		NODISC MouseCursor& Cursor() noexcept;

		NODISC const GamepadInput& Gamepad() const noexcept;
		NODISC GamepadInput& Gamepad() noexcept;

		void SetEnabled(bool aFlag);

		void Update();
		bool HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam);

	private:
		KeyboardInput	myKeyboard;
		MouseInput		myMouse;
		MouseCursor		myCursor;
		GamepadInput	myGamepad;
	};
}
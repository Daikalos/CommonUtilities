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

		NODISC const GamepadInput& Gamepad(unsigned aGamepadIndex = 0) const noexcept;
		NODISC GamepadInput& Gamepad(unsigned aGamepadIndex = 0) noexcept;

		NODISC int ConnectedGamepadCount() const;

		NODISC bool IsAnyPressed() const;

		void SetEnabled(bool aFlag);

		void SetInFocus(bool aFlag);

		void SetExternalFocus(bool aFlag);

		void Update();
		bool HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam);

	private:
		using GamepadArray = std::array<GamepadInput, XUSER_MAX_COUNT>;

		KeyboardInput	myKeyboard;
		MouseInput		myMouse;
		MouseCursor		myCursor;
		GamepadArray	myGamepads;
	};
}
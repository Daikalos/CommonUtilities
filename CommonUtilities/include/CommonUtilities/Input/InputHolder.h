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

		NODISC const GamepadInput& Gamepad(unsigned aGamepadIndex = 0) const;
		NODISC GamepadInput& Gamepad(unsigned aGamepadIndex = 0);

		NODISC int ConnectedGamepadCount() const;

		NODISC bool IsAnyPressed() const;

		/// Enable or disable the InputHandler, will cause all input to return false or 0.0f.
		/// 
		void SetEnabled(bool aFlag);

		/// Manually enable or disable that input has focus
		/// 
		void SetInFocus(bool aFlag);

		/// Whether something inside the application is stealing input, for example, ImGui
		/// 
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
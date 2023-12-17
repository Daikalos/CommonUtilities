#pragma once

#include <CommonUtilities/Utility/NonCopyable.h>
#include <CommonUtilities/Config.h>

#include "KeyboardInput.h"
#include "MouseInput.h"
#include "MouseCursor.h"

namespace CommonUtilities
{
	class COMMON_UTILITIES_API InputHolder final : private NonCopyable
	{
	public:
		InputHolder() = default;
		~InputHolder() = default;

		const KeyboardInput& Keyboard() const noexcept;
		KeyboardInput& Keyboard() noexcept;

		const MouseInput& Mouse() const noexcept;
		MouseInput& Mouse() noexcept;

		const MouseCursor& Cursor() const noexcept;
		MouseCursor& Cursor() noexcept;

		void Update();
		bool HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam);

	private:
		KeyboardInput	myKeyboard;
		MouseInput		myMouse;
		MouseCursor		myCursor;
	};
}
#include <CommonUtilities/Input/InputHolder.h>

using namespace CommonUtilities;

const KeyboardInput& InputHolder::Keyboard() const noexcept { return myKeyboard; }
KeyboardInput& InputHolder::Keyboard() noexcept             { return myKeyboard; }

const MouseInput& InputHolder::Mouse() const noexcept		{ return myMouse; }
MouseInput& InputHolder::Mouse() noexcept					{ return myMouse; }

const MouseCursor& InputHolder::Cursor() const noexcept		{ return myCursor; }
MouseCursor& InputHolder::Cursor() noexcept					{ return myCursor; }

const GamepadInput& InputHolder::Gamepad(unsigned aGamepadIndex) const	{ return myGamepads[aGamepadIndex]; }
GamepadInput& InputHolder::Gamepad(unsigned aGamepadIndex)				{ return myGamepads[aGamepadIndex]; }

int InputHolder::ConnectedGamepadCount() const
{
	int pluggedInGamepads = 0;

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		XINPUT_STATE gamepadState{};
		if (Gamepad::IsConnected(i))
		{
			++pluggedInGamepads;
		}
	}

	return pluggedInGamepads;
}

bool InputHolder::IsAnyPressed() const
{
	for (auto& gamepad : myGamepads)
	{
		if (gamepad.IsAnyPressed())
		{
			return true;
		}
	}

	return myKeyboard.IsAnyPressed() || myMouse.IsAnyPressed();
}

void InputHolder::SetEnabled(bool aFlag)
{
	myKeyboard.SetEnabled(aFlag);
	myMouse.SetEnabled(aFlag);
	myCursor.SetEnabled(aFlag);

	for (auto& gamepad : myGamepads)
	{
		gamepad.SetEnabled(aFlag);
	}
}

void InputHolder::SetInFocus(bool aFlag)
{
	myKeyboard.SetInFocus(aFlag);
	myMouse.SetInFocus(aFlag);
	myCursor.SetInFocus(aFlag);

	for (auto& gamepad : myGamepads)
	{
		gamepad.SetInFocus(aFlag);
	}
}

void InputHolder::SetExternalFocus(bool aFlag)
{
	myKeyboard.SetExternalFocus(aFlag);
	myMouse.SetExternalFocus(aFlag);
	myCursor.SetExternalFocus(aFlag);

	for (auto& gamepad : myGamepads)
	{
		gamepad.SetExternalFocus(aFlag);
	}
}

void InputHolder::Update()
{
	myKeyboard.Update();
	myMouse.Update();
	myCursor.Update();

	for (auto& gamepad : myGamepads)
	{
		gamepad.Update();

		if (gamepad.GetIndex() != -1)
		{
			GamepadInput::ourOccupiedGamepadIndices[gamepad.GetIndex()] = gamepad.IsConnected();
		}
	}
}

bool InputHolder::HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam)
{
	if (myKeyboard.HandleEvent(aMessage, wParam, lParam))
	{
		return true;
	}
	if (myMouse.HandleEvent(aMessage, wParam, lParam))
	{
		return true;
	}
	if (myCursor.HandleEvent(aMessage, wParam, lParam))
	{
		return true;
	}

	bool gamepadHandledEvent = false;
	for (auto& gamepad : myGamepads)
	{
		gamepadHandledEvent = gamepad.HandleEvent(aMessage, wParam, lParam);
	}

	if (gamepadHandledEvent)
	{
		return true;
	}

	return false;
}

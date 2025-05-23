#include <CommonUtilities/Input/InputHolder.h>

using namespace CommonUtilities;

const KeyboardInput& InputHolder::Keyboard() const noexcept { return myKeyboard; }
KeyboardInput& InputHolder::Keyboard() noexcept             { return myKeyboard; }

const MouseInput& InputHolder::Mouse() const noexcept		{ return myMouse; }
MouseInput& InputHolder::Mouse() noexcept					{ return myMouse; }

const MouseCursor& InputHolder::Cursor() const noexcept		{ return myCursor; }
MouseCursor& InputHolder::Cursor() noexcept					{ return myCursor; }

const GamepadInput& InputHolder::Gamepad(const unsigned aGamepadIndex) const noexcept	{ return myGamepads[aGamepadIndex]; }
GamepadInput& InputHolder::Gamepad(const unsigned aGamepadIndex) noexcept				{ return myGamepads[aGamepadIndex]; }

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

void InputHolder::SetHasExternalFocus(bool aFlag)
{
	myKeyboard.SetHasExternalFocus(aFlag);
	myMouse.SetHasExternalFocus(aFlag);
	myCursor.SetHasExternalFocus(aFlag);

	for (auto& gamepad : myGamepads)
	{
		gamepad.SetHasExternalFocus(aFlag);
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
			GamepadInput::gOccupiedGamepadIndices[gamepad.GetIndex()] = gamepad.IsConnected();
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

	bool gamePadHandledEvent = false;
	for (auto& gamepad : myGamepads)
	{
		gamePadHandledEvent = gamepad.HandleEvent(aMessage, wParam, lParam);
	}

	if (gamePadHandledEvent)
	{
		return true;
	}

	return false;
}

#include <CommonUtilities/Input/KeyboardInput.h>

using namespace CommonUtilities;

KeyboardInput::KeyboardInput() = default;

KeyboardInput::~KeyboardInput() = default;

bool KeyboardInput::IsHeld(ButtonType aKey) const
{
	return myEnabled && myCurrentState[aKey];
}
bool KeyboardInput::IsPressed(ButtonType aKey) const
{
	return myEnabled && myCurrentState[aKey] && !myPreviousState[aKey];
}
bool KeyboardInput::IsReleased(ButtonType aKey) const
{
	return myEnabled && !myCurrentState[aKey] && myPreviousState[aKey];
}

void KeyboardInput::Update()
{	 
	if (!myEnabled)
		return;

	myPreviousState = myCurrentState;
	myCurrentState = myTentativeState;
}	 
	 
bool KeyboardInput::HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam)
{
	switch (aMessage)
	{
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			return SetTentativeState(wParam, true);
		}
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			return SetTentativeState(wParam, false);
		}
		case WM_SETFOCUS:
		{
			myEnabled = true;
			return false;
		}
		case WM_KILLFOCUS:
		{
			myEnabled = false;
			return false;
		}
	}

	return false;
}

bool KeyboardInput::SetTentativeState(WPARAM wParam, bool aState)
{
	Keyboard::Key key = Keyboard::VirtualKeyToCUKey(wParam);
	if (key != Keyboard::None)
	{
		myTentativeState[key] = aState;
		return true;
	}
	else
	{
		// keyboard does not support this key
	}

	return false;
}

#include <CommonUtilities/Input/KeyboardInput.h>

using namespace CommonUtilities;

KeyboardInput::KeyboardInput() = default;

KeyboardInput::~KeyboardInput() = default;

bool KeyboardInput::IsHeld(ButtonType aKey) const
{
	return GetInFocus() && GetEnabled() && myCurrentState[aKey];
}
bool KeyboardInput::IsPressed(ButtonType aKey) const
{
	return GetInFocus() && GetEnabled() && myCurrentState[aKey] && !myPreviousState[aKey];
}
bool KeyboardInput::IsReleased(ButtonType aKey) const
{
	return GetInFocus() && GetEnabled() && !myCurrentState[aKey] && myPreviousState[aKey];
}

void KeyboardInput::Update()
{	 
	myPreviousState = myCurrentState;
	myCurrentState = myTentativeState;
}	 
	 
bool KeyboardInput::HandleEventImpl(UINT aMessage, WPARAM wParam, LPARAM lParam)
{
	switch (aMessage)
	{
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			return SetTentativeState(wParam, lParam, true);
		}
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			return SetTentativeState(wParam, lParam, false);
		}
	}

	return false;
}

void KeyboardInput::ResetTentativeState()
{
	std::fill(myTentativeState.begin(), myTentativeState.end(), false);
}

bool KeyboardInput::SetTentativeState(WPARAM wParam, LPARAM lParam, bool aState)
{
	Keyboard::Key key = Keyboard::ProcessVirtualKey(wParam, lParam);
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

#include <CommonUtilities/Input/KeyboardInput.h>

using namespace CommonUtilities;

bool KeyboardInput::IsHeld(ButtonType aKey) const
{
	return IsEnabled() && myCurrentState[aKey];
}
bool KeyboardInput::IsPressed(ButtonType aKey) const
{
	return IsEnabled() && myCurrentState[aKey] && !myPreviousState[aKey];
}
bool KeyboardInput::IsReleased(ButtonType aKey) const
{
	return IsEnabled() && !myCurrentState[aKey] && myPreviousState[aKey];
}

bool KeyboardInput::IsAnyPressed() const
{
	if (!IsEnabled())
		return false;

	for (std::size_t i = 0; i < Keyboard::KeyCount; ++i)
	{
		ButtonType key = (ButtonType)i;
		if (myCurrentState[key] && !myPreviousState[key])
		{
			return true;
		}
	}

	return false;
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

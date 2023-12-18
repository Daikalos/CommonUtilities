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
	if (!GetInFocus() || !GetEnabled())
		return;

	myPreviousState = myCurrentState;
	myCurrentState = myTentativeState;
}	 
	 
bool KeyboardInput::HandleEventImpl(UINT aMessage, WPARAM wParam, [[maybe_unused]] LPARAM lParam)
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
	}

	return false;
}

bool KeyboardInput::SetTentativeState(WPARAM wParam, bool aState)
{
	Keyboard::Key key = Keyboard::VirtualKeyToCUKey((int)wParam);
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

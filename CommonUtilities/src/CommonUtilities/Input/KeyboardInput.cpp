#include <CommonUtilities/Input/KeyboardInput.h>

using namespace CommonUtilities;

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

	for (int i = 0; i < Keyboard::KeyCount; ++i)
	{
		myPreviousState[i] = myCurrentState[i];
		myCurrentState[i] = Keyboard::IsKeyPressed(static_cast<Keyboard::Key>(i));
	}
}	 
	 
void KeyboardInput::HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam)
{
	switch (aMessage)
	{
	case WM_SETFOCUS:

		break;
	case WM_KILLFOCUS:

		break;
	}
}

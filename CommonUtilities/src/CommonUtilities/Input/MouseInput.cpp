#include <CommonUtilities/Input/MouseInput.h>

using namespace CommonUtilities;

bool MouseInput::ScrollUp() const noexcept
{
    return myEnabled && myScrollDelta > myScrollThreshold;
}
bool MouseInput::ScrollDown() const noexcept
{
	return myEnabled && myScrollDelta < -myScrollThreshold;
}

bool MouseInput::IsHeld(ButtonType aButton) const
{
    return myEnabled && myCurrentState[aButton];
}
bool MouseInput::IsPressed(ButtonType aButton) const
{
	return myEnabled && myCurrentState[aButton] && !myPreviousState[aButton];
}
bool MouseInput::IsReleased(ButtonType aButton) const
{
	return myEnabled && !myCurrentState[aButton] && myPreviousState[aButton];
}

void MouseInput::Update()
{
	if (!myEnabled)
		return;

	myScrollDelta = 0.0f;

	for (int i = 0; i < Mouse::ButtonCount; ++i)
	{
		myPreviousState[i] = myCurrentState[i];
		myCurrentState[i] = Mouse::IsMouseButtonPressed(static_cast<Mouse::Button>(i));
	}
}

void MouseInput::HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam)
{
    switch (aMessage)
    {
        case WM_MOUSEWHEEL:
		{
			std::int16_t delta = static_cast<std::int16_t>(HIWORD(wParam)); // value between 0 and 120
			myScrollDelta = static_cast<float>(delta) / 120.0f;
			break;
		}
    }
}

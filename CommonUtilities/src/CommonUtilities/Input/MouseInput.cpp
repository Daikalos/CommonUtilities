#include <CommonUtilities/Input/MouseInput.h>

using namespace CommonUtilities;

bool MouseInput::ScrollUp() const noexcept
{
    return GetEnabled() && myScrollDelta > myScrollThreshold;
}
bool MouseInput::ScrollDown() const noexcept
{
	return GetEnabled() && myScrollDelta < -myScrollThreshold;
}

bool MouseInput::IsHeld(ButtonType aButton) const
{
    return GetEnabled() && myCurrentState[aButton];
}
bool MouseInput::IsPressed(ButtonType aButton) const
{
	return GetEnabled() && myCurrentState[aButton] && !myPreviousState[aButton];
}
bool MouseInput::IsReleased(ButtonType aButton) const
{
	return GetEnabled() && !myCurrentState[aButton] && myPreviousState[aButton];
}

bool MouseInput::IsAnyPressed() const
{
	if (!GetEnabled())
		return false;

	for (std::size_t i = 0; i < Mouse::ButtonCount; ++i)
	{
		ButtonType button = (ButtonType)i;
		if (myCurrentState[button] && !myPreviousState[button])
		{
			return true;
		}
	}

	return false;
}

void MouseInput::SetScrollThreshold(float aScrollThreshold)
{
	myScrollThreshold = aScrollThreshold;
}

void MouseInput::Update()
{
	myScrollDelta = myTentativeScrollDelta;
	myTentativeScrollDelta = 0.0f;

	myPreviousState = myCurrentState;
	myCurrentState = myTentativeState;
}

bool MouseInput::HandleEventImpl(UINT aMessage, WPARAM wParam, UNSD LPARAM lParam)
{
    switch (aMessage)
    {
		case WM_LBUTTONDOWN:
		{
			return SetTentativeState(VK_LBUTTON, true);
		}
		case WM_MBUTTONDOWN:
		{
			return SetTentativeState(VK_MBUTTON, true);
		}
		case WM_RBUTTONDOWN:
		{
			return SetTentativeState(VK_RBUTTON, true);
		}
		case WM_LBUTTONUP:
		{
			return SetTentativeState(VK_LBUTTON, false);
		}
		case WM_MBUTTONUP:
		{
			return SetTentativeState(VK_MBUTTON, false);
		}
		case WM_RBUTTONUP:
		{
			return SetTentativeState(VK_RBUTTON, false);
		}
		case WM_XBUTTONDOWN:
		{
			const int xButton = GET_XBUTTON_WPARAM(wParam);
			return SetTentativeState((xButton == 1) ? VK_XBUTTON1 : VK_XBUTTON2, true);
		}
		case WM_XBUTTONUP:
		{			
			const int xButton = GET_XBUTTON_WPARAM(wParam);
			return SetTentativeState((xButton == 1) ? VK_XBUTTON1 : VK_XBUTTON2, false);
		}
        case WM_MOUSEWHEEL:
		{
			std::int16_t delta = (std::int16_t)HIWORD(wParam); // value between 0 and 120
			myTentativeScrollDelta = (float)delta / 120.0f;
			return true;
		}
    }

	return false;
}

void MouseInput::ResetTentativeState()
{
	std::fill(myTentativeState.begin(), myTentativeState.end(), false);
}

bool MouseInput::SetTentativeState(WPARAM wParam, bool aState)
{
	Mouse::Button button = Mouse::VirtualKeyToCUButton((int)wParam);
	if (button != Mouse::Button::None)
	{
		myTentativeState[button] = aState;
		return true;
	}
	else
	{
		// mouse does not support this button
	}

	return false;
}

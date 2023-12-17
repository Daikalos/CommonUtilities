#include <CommonUtilities/Input/MouseInput.h>

using namespace CommonUtilities;

MouseInput::MouseInput() = default;

MouseInput::~MouseInput() = default;

bool MouseInput::ScrollUp() const noexcept
{
    return GetInFocus() && GetEnabled() && myScrollDelta > myScrollThreshold;
}
bool MouseInput::ScrollDown() const noexcept
{
	return GetInFocus() && GetEnabled() && myScrollDelta < -myScrollThreshold;
}

bool MouseInput::IsHeld(ButtonType aButton) const
{
    return GetInFocus() && GetEnabled() && myCurrentState[aButton];
}
bool MouseInput::IsPressed(ButtonType aButton) const
{
	return GetInFocus() && GetEnabled() && myCurrentState[aButton] && !myPreviousState[aButton];
}
bool MouseInput::IsReleased(ButtonType aButton) const
{
	return GetInFocus() && GetEnabled() && !myCurrentState[aButton] && myPreviousState[aButton];
}

void MouseInput::SetScrollThreshold(float aScrollThreshold)
{
	myScrollThreshold = aScrollThreshold;
}

void MouseInput::Update()
{
	if (!GetInFocus() || !GetEnabled())
		return;

	myScrollDelta = myTentativeScrollDelta;
	myTentativeScrollDelta = 0.0f;

	myPreviousState = myCurrentState;
	myCurrentState = myTentativeState;
}

bool MouseInput::HandleEventImpl(UINT aMessage, WPARAM wParam, LPARAM lParam)
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
			std::int16_t delta = static_cast<std::int16_t>(HIWORD(wParam)); // value between 0 and 120
			myTentativeScrollDelta = static_cast<float>(delta) / 120.0f;
			return true;
		}
    }

	return false;
}

bool MouseInput::SetTentativeState(WPARAM wParam, bool aState)
{
	Mouse::Button button = Mouse::VirtualKeyToCUButton(wParam);
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

#include <CommonUtilities/Input/Mouse.h>

#include <Windows.h>

using namespace CommonUtilities;

Mouse::Button Mouse::VirtualKeyToCUButton(int aVirtualKey)
{
    switch (aVirtualKey)
    {
        case VK_LBUTTON:    return Mouse::Left;
        case VK_RBUTTON:    return Mouse::Right;
        case VK_MBUTTON:    return Mouse::Middle;
        case VK_XBUTTON1:   return Mouse::XButton1;
        case VK_XBUTTON2:   return Mouse::XButton2;
        default:            return Mouse::None;
    }
}

int Mouse::CUButtonToVirtualKey(Button aButton)
{
    switch (aButton)
    {
        case Mouse::Left:       return GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON;
        case Mouse::Right:      return GetSystemMetrics(SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON;
        case Mouse::Middle:     return VK_MBUTTON;
        case Mouse::XButton1:   return VK_XBUTTON1;
        case Mouse::XButton2:   return VK_XBUTTON2;
        default:                return 0;
    }
}

bool Mouse::IsMouseButtonPressed(Mouse::Button aButton)
{
    int virtualKey = CUButtonToVirtualKey(aButton);
    return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
}

POINT Mouse::GetMousePosition()
{
    POINT point;
    GetCursorPos(&point);
    return point;
}

POINT Mouse::GetMousePosition(HWND aHandle)
{
    if (aHandle)
    {
    	POINT point;
    	GetCursorPos(&point);
    	ScreenToClient(aHandle, &point);
    	return point;
    }
    return POINT();
}

void Mouse::SetMousePosition(const POINT& aPoint)
{
    SetCursorPos(aPoint.x, aPoint.y);
}

void Mouse::SetMousePosition(POINT aPoint, HWND aHandle)
{
    if (aHandle)
    {
    	ClientToScreen(aHandle, &aPoint);
    	SetCursorPos(aPoint.x, aPoint.y);
    }
}

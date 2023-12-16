#include <CommonUtilities/Input/Mouse.h>

using namespace CommonUtilities;

bool Mouse::IsMouseButtonPressed(Mouse::Button aButton)
{
    int virtualKey = 0;
    switch (aButton)
    {
        case Mouse::Left:
            virtualKey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON;
            break;
        case Mouse::Right:
            virtualKey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON;
            break;
        case Mouse::Middle:
            virtualKey = VK_MBUTTON;
            break;
        case Mouse::XButton1:
            virtualKey = VK_XBUTTON1;
            break;
        case Mouse::XButton2:
            virtualKey = VK_XBUTTON2;
            break;
    }
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

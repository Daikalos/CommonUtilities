#include <CommonUtilities/pch.h>
#include <CommonUtilities/Input/Mouse.h>

#include <cassert>
#include <Windows.h>

using namespace CommonUtilities;

Mouse::Button Mouse::VirtualKeyToCUButton(WPARAM aKey)
{
    switch (aKey)
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

Vector2i Mouse::GetMousePosition()
{
    POINT point;
    GetCursorPos(&point);
    return { point.x, point.y };
}

Vector2i Mouse::GetMousePosition(HWND aHandle)
{
    assert(aHandle != nullptr && "A handle is required for retrieving relative position");

    POINT point;
    GetCursorPos(&point);
    ScreenToClient(aHandle, &point);
    return { point.x, point.y };
}

void Mouse::SetMousePosition(const Vector2i& aPoint)
{
    SetCursorPos(aPoint.x, aPoint.y);
}

void Mouse::SetMousePosition(const Vector2i& aPoint, HWND aHandle)
{
    assert(aHandle != nullptr && "A handle is required for setting relative position");

    POINT point { aPoint.x, aPoint.y };
    ClientToScreen(aHandle, &point);
    SetCursorPos(point.x, point.y);
}

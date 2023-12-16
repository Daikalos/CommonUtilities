#pragma once

#include <CommonUtilities/Config.h>

#include <Windows.h>

namespace CommonUtilities::Mouse
{
    enum Button
    {
        None = -1,

        Left,
        Right,
        Middle,
        XButton1,
        XButton2,

        ButtonCount
    };

    COMMON_UTILITIES_API NODISC bool IsMouseButtonPressed(Button aButton);

    COMMON_UTILITIES_API NODISC POINT GetMousePosition();
    COMMON_UTILITIES_API NODISC POINT GetMousePosition(HWND aHandle);

    COMMON_UTILITIES_API void SetMousePosition(const POINT& aPoint);
    COMMON_UTILITIES_API void SetMousePosition(POINT aPoint, HWND aHandle);
}
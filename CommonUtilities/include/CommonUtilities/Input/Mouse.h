#pragma once

#include <CommonUtilities/System/WindowsHeader.h>
#include <CommonUtilities/Math/Vector2.hpp>
#include <CommonUtilities/Config.h>

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

    COMMON_UTILITIES_API NODISC Button VirtualKeyToCUButton(WPARAM aKey);
    COMMON_UTILITIES_API NODISC int CUButtonToVirtualKey(Button aButton);
    COMMON_UTILITIES_API NODISC bool IsMouseButtonPressed(Button aButton);

    COMMON_UTILITIES_API NODISC Vector2i GetMousePosition();
    COMMON_UTILITIES_API NODISC Vector2i GetMousePosition(HWND aHandle);

    COMMON_UTILITIES_API void SetMousePosition(const Vector2i& aPoint);
    COMMON_UTILITIES_API void SetMousePosition(const Vector2i& aPoint, HWND aHandle);
}
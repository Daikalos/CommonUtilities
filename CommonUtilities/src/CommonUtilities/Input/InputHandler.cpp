#include <CommonUtilities/Input/InputHandler.h>

#include <windowsx.h>
#include <WinUser.h>
#include <cassert>

using namespace CommonUtilities;

bool InputHandler::GetEnabled() const noexcept
{
	return myEnabled;
}
bool InputHandler::GetInFocus() const noexcept
{
	return (myInFocus || !myFocusInput);
}

void InputHandler::SetEnabled(bool aFlag) noexcept
{
	myEnabled = aFlag;
}
void InputHandler::SetFocusAffectInput(bool aFlag) noexcept
{
	myFocusInput = aFlag;
}

bool InputHandler::HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam)
{
	switch (aMessage)
	{
		case WM_SETFOCUS:
		{
			myInFocus = true;
			return false;
		}
		case WM_KILLFOCUS:
		{
			if (myInFocus)
			{
				if (myFocusInput)
				{
					ResetTentativeState();
				}
				myInFocus = false;
			}
			return false;
		}
	}

	return GetInFocus() ? HandleEventImpl(aMessage, wParam, lParam) : false;
}

namespace CommonUtilities::deprecated
{
	InputHandler::InputHandler()
		: myCurrentState()
		, myPreviousState()
		, myInputState()
		, myMousePosition()
		, myPreviousMousePos()
		, myMouseDeltaPos()
		, myScrollDelta()
		, myCursorGrabbed(false)
		, myInFocus(true)
	{

	}

	bool InputHandler::HandleEvents(HWND aHandle, UINT aMessage, WPARAM wParam, LPARAM lParam)
	{
		if (HandleWindowEvents(aHandle, aMessage, wParam, lParam))
		{
			return true;
		}
		if (HandleMouseEvents(aMessage, wParam, lParam))
		{
			return true;
		}
		if (HandleKeyboardEvents(aMessage, wParam, lParam))
		{
			return true;
		}

		return false;
	}

	bool InputHandler::HandleWindowEvents(HWND aHandle, UINT aMessage, UNSD WPARAM wParam, UNSD LPARAM lParam)
	{
		assert(aHandle != nullptr && "A handle is required for window events!");

		switch (aMessage)
		{
		case WM_SIZE:
		{
			GrabCursor(myCursorGrabbed, aHandle);
			return true;
		}
		case WM_ENTERSIZEMOVE:
		{
			GrabCursor(false, aHandle);
			return true;
		}
		case WM_EXITSIZEMOVE:
		{
			GrabCursor(myCursorGrabbed, aHandle);
			return true;
		}
		case WM_SETFOCUS:
		{
			GrabCursor(myCursorGrabbed, aHandle);
			myInFocus = true;
			return true;
		}
		case WM_KILLFOCUS:
		{
			GrabCursor(false, aHandle);
			myInFocus = false;

			// TODO: hack, look for something better later
			for (std::uint32_t index = 0; index < 256; ++index)
			{
				myInputState[index] = false;
			}

			return true;
		}
		}

		return false;
	}

	bool InputHandler::HandleKeyboardEvents(UINT aMessage, WPARAM wParam, UNSD LPARAM lParam)
	{
		switch (aMessage)
		{
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			myInputState[wParam] = true;
			return true;
		}
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			myInputState[wParam] = false;
			return true;
		}
		}

		return false;
	}
	bool InputHandler::HandleMouseEvents(UINT aMessage, WPARAM wParam, LPARAM lParam)
	{
		switch (aMessage)
		{
		case WM_LBUTTONDOWN:
		{
			myInputState[VK_LBUTTON] = true;
			return true;
		}
		case WM_RBUTTONDOWN:
		{
			myInputState[VK_RBUTTON] = true;
			return true;
		}
		case WM_MBUTTONDOWN:
		{
			myInputState[VK_MBUTTON] = true;
			return true;
		}
		case WM_LBUTTONUP:
		{
			myInputState[VK_LBUTTON] = false;
			return true;
		}
		case WM_MBUTTONUP:
		{
			myInputState[VK_MBUTTON] = false;
			return true;
		}
		case WM_RBUTTONUP:
		{
			myInputState[VK_RBUTTON] = false;
			return true;
		}
		case WM_MOUSEWHEEL:
		{
			std::int16_t delta = static_cast<std::int16_t>(HIWORD(wParam)); // value between 0 and 120
			myScrollDelta = static_cast<float>(delta) / 120.0f;
			return true;
		}
		case WM_MOUSEMOVE:
		{
			myMousePosition =
			{
				GET_X_LPARAM(lParam),
				GET_Y_LPARAM(lParam)
			};
			return true;
		}
		}

		return false;
	}

	bool InputHandler::IsKeyDown(int aKeyCode) const
	{
		return myCurrentState[aKeyCode]; // TODO: maybe add held timer
	}

	bool InputHandler::IsKeyPressed(int aKeyCode) const
	{
		return myCurrentState[aKeyCode] && !myPreviousState[aKeyCode];
	}

	bool InputHandler::IsKeyReleased(int aKeyCode) const
	{
		return !myCurrentState[aKeyCode] && myPreviousState[aKeyCode];
	}

	float InputHandler::GetScrollDelta() const
	{
		return myScrollDelta;
	}
	bool InputHandler::GetCursorGrabbed() const
	{
		return myCursorGrabbed;
	}

	POINT InputHandler::GetMousePosition() const
	{
		POINT point;
		GetCursorPos(&point);
		return point;
	}

	POINT InputHandler::GetMousePosition(HWND aHandle) const
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

	POINT InputHandler::GetMouseDelta() const
	{
		return myMouseDeltaPos;
	}

	void InputHandler::SetMousePosition(const POINT& aPoint)
	{
		SetCursorPos(aPoint.x, aPoint.y);
	}

	void InputHandler::SetMousePosition(POINT aPoint, HWND aHandle)
	{
		if (aHandle)
		{
			ClientToScreen(aHandle, &aPoint);
			SetCursorPos(aPoint.x, aPoint.y);
		}
	}

	void InputHandler::SetCursorGrabbed(bool aGrabbed, HWND aHandle)
	{
		myCursorGrabbed = aGrabbed;
		GrabCursor(myCursorGrabbed, aHandle);
	}

	void InputHandler::UpdateInput()
	{
		myScrollDelta = 0.0f;

		POINT currentPos = myMousePosition;
		myMouseDeltaPos =
		{
			currentPos.x - myPreviousMousePos.x,
			currentPos.y - myPreviousMousePos.y
		};
		myPreviousMousePos = currentPos;

		for (std::uint32_t index = 0; index < ourKeyCount; ++index)
		{
			myPreviousState[index] = myCurrentState[index];
		}

		for (std::uint32_t index = 0; index < ourKeyCount; ++index)
		{
			myCurrentState[index] = myInFocus && myInputState[index];
		}
	}

	void InputHandler::GrabCursor(bool aGrabbed, HWND aHandle)
	{
		assert(aHandle != nullptr && "A handle is required for grabbing");

		if (aGrabbed)
		{
			RECT rect;
			GetClientRect(aHandle, &rect);
			MapWindowPoints(aHandle, nullptr, reinterpret_cast<LPPOINT>(&rect), 2);
			ClipCursor(&rect);
		}
		else
		{
			ClipCursor(nullptr);
		}
	}
}

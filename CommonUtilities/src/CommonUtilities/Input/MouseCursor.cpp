#include <CommonUtilities/pch.h>
#include <CommonUtilities/Input/MouseCursor.h>

#include <CommonUtilities/Input/Mouse.h>

#include <cassert>
#include <windowsx.h>
#include <WinUser.h>
#include <hidusage.h>

using namespace CommonUtilities;

MouseCursor::MouseCursor(HWND aHandle) : myHandle(nullptr)
{
	Connect(aHandle);
}

bool MouseCursor::GetGrabbed() const noexcept
{
	return myIsGrabbed;
}
bool MouseCursor::GetVisible() const noexcept
{
	return myIsVisible;
}

const Vector2i& MouseCursor::GetPosition() const noexcept
{
	return myCurrentPosition;
}
const Vector2i& MouseCursor::GetDelta() const noexcept
{
	return myMoveDelta;
}
const Vector2i& MouseCursor::GetWindowSize() const noexcept
{
	return myWindowSize;
}

bool MouseCursor::IsConnected() const noexcept
{
	return myHandle != nullptr;
}

void MouseCursor::Connect(HWND aHandle)
{
	if (myHandle != aHandle)
	{
		Disconnect();
		if (aHandle != nullptr)
		{ 
			RAWINPUTDEVICE rid[1]{};
			rid[0].usUsagePage	= HID_USAGE_PAGE_GENERIC;
			rid[0].usUsage		= HID_USAGE_GENERIC_MOUSE;
			rid[0].dwFlags		= RIDEV_INPUTSINK;
			rid[0].hwndTarget	= aHandle;
			if (!RegisterRawInputDevices(rid, 1, sizeof(rid[0])))
			{
				assert(false && "Failed to register raw input device");
			}

			RECT rect;
			if (GetWindowRect(aHandle, &rect))
			{
				myWindowSize = Vector2i(
					rect.right - rect.left, 
					rect.bottom - rect.top);
			}
			else
			{
				assert(false && "Failed to get window rect");
			}
		}
	}

	myHandle = aHandle;
}
void MouseCursor::Disconnect()
{
	if (myHandle != nullptr)
	{
		RAWINPUTDEVICE rid[1]{};
		rid[0].usUsagePage	= HID_USAGE_PAGE_GENERIC;
		rid[0].usUsage		= HID_USAGE_GENERIC_MOUSE;
		rid[0].dwFlags		= RIDEV_REMOVE;
		rid[0].hwndTarget	= nullptr;
		if (RegisterRawInputDevices(rid, 1, sizeof(rid[0])))
		{
			assert(false && "Failed to remove raw input device");
		}
	}

	myHandle = nullptr;
}

void MouseCursor::SetPosition(const Vector2i& aPoint)
{
	Mouse::SetMousePosition(aPoint);
}
void MouseCursor::SetRelativePosition(const Vector2i& aPoint)
{
	assert(myHandle != nullptr);
	Mouse::SetMousePosition(aPoint, myHandle);
}

void MouseCursor::SetGrabbed(bool aGrabbed)
{
	if (myIsGrabbed != aGrabbed)
	{
		GrabCursor(aGrabbed);
		myIsGrabbed = aGrabbed;
	}
}
void MouseCursor::SetVisible(bool aState)
{
	if (aState != myIsVisible)
	{
		ShowCursor(aState);
		myIsVisible = aState;
	}
}
void MouseCursor::SetLocked(bool aLocked)
{
	myIsLocked = aLocked;
}

void MouseCursor::Update()
{
	myPreviousPosition = myCurrentPosition;
	myCurrentPosition = myTentativePosition;

	myMoveDelta	= myTentativeMoveDelta;
	myTentativeMoveDelta = { 0, 0 };

	if (IsConnected() && myEnabled && myInFocus && myIsLocked)
	{
		SetRelativePosition(myWindowSize / 2);
	}
}

bool MouseCursor::HandleEventImpl(UINT aMessage, UNSD WPARAM wParam, LPARAM lParam)
{
	switch (aMessage)
	{
		case WM_MOUSEMOVE:
		{
			const int xPos = GET_X_LPARAM(lParam);
			const int yPos = GET_Y_LPARAM(lParam);

			myTentativePosition.x = xPos;
			myTentativePosition.y = yPos;

			return true;
		}
		case WM_INPUT:
		{
			if (IsConnected())
			{
				UINT dwSize = sizeof(RAWINPUT);
				static BYTE lpb[sizeof(RAWINPUT)];

				GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

				RAWINPUT* raw = (RAWINPUT*)lpb;

				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					myTentativeMoveDelta.x += raw->data.mouse.lLastX;
					myTentativeMoveDelta.y += raw->data.mouse.lLastY;
				}

				return true;
			}

			return false;
		}
		case WM_SIZE:
		{
			if (IsConnected())
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);

				myWindowSize = Vector2i(width, height);
			}

			GrabCursor(myIsGrabbed);

			return false;
		}
		case WM_ENTERSIZEMOVE:
		{
			GrabCursor(false);
			return false;
		}
		case WM_EXITSIZEMOVE:
		{
			GrabCursor(myIsGrabbed);
			return false;
		}
		case WM_SETFOCUS:
		{
			GrabCursor(myIsGrabbed);
			SetVisible(myIsVisible);
			return false;
		}
		case WM_KILLFOCUS:
		{
			GrabCursor(false);
			SetVisible(true);
			return false;
		}
	}
	return false;
}

void MouseCursor::GrabCursor(bool aGrabbed)
{
	if (aGrabbed)
	{
		assert(IsConnected() && "A handle is required for grabbing");

		RECT rect;
		GetClientRect(myHandle, &rect);
		MapWindowPoints(myHandle, nullptr, reinterpret_cast<LPPOINT>(&rect), 2);
		ClipCursor(&rect);
	}
	else
	{
		ClipCursor(nullptr);
	}
}

void MouseCursor::ResetTentativeState()
{

}

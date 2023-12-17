#include <CommonUtilities/Input/MouseCursor.h>

#include <cassert>
#include <windowsx.h>
#include <WinUser.h>
#include <hidusage.h>

using namespace CommonUtilities;

MouseCursor::MouseCursor() = default;

MouseCursor::~MouseCursor() = default;

MouseCursor::MouseCursor(HWND aHandle)
{
	SetHandle(aHandle);
}

bool MouseCursor::GetGrabbed() const
{
	return myIsGrabbed;
}
bool MouseCursor::GetVisible() const
{
	return myIsVisible;
}

const Vector2i& MouseCursor::GetPosition() const
{
	return myCurrentPosition;
}
Vector2i MouseCursor::GetRelativePosition() const
{
	assert(myHandle != nullptr && "A handle is required for retrieving relative position");

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(myHandle, &point);
	return { point.x, point.y };
}

const Vector2i& MouseCursor::GetMouseDelta() const
{
	return myMoveDelta;
}

void MouseCursor::SetHandle(HWND aHandle)
{
	assert(aHandle != nullptr && "You provided a nullptr handle...");

	myHandle = aHandle;

	RAWINPUTDEVICE Rid[1];
	Rid[0].usUsagePage	= HID_USAGE_PAGE_GENERIC;
	Rid[0].usUsage		= HID_USAGE_GENERIC_MOUSE;
	Rid[0].dwFlags		= RIDEV_INPUTSINK;
	Rid[0].hwndTarget	= myHandle;
	RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
}

void MouseCursor::SetPosition(const Vector2i& aPoint)
{
	SetCursorPos(aPoint.x, aPoint.y);
}
void MouseCursor::SetRelativePosition(const Vector2i& aPoint)
{
	assert(myHandle != nullptr && "A handle is required for setting relative position");

	POINT point { aPoint.x, aPoint.y };
	ClientToScreen(myHandle, &point);
	SetCursorPos(point.x, point.y);
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

void MouseCursor::Update()
{
	if (!GetInFocus() || !GetEnabled())
		return;

	myPreviousPosition	= myCurrentPosition;
	myCurrentPosition	= myTentativePosition;

	myMoveDelta				= myTentativeMoveDelta;
	myTentativeMoveDelta	= { 0, 0 };
}

bool MouseCursor::HandleEventImpl(UINT aMessage, WPARAM wParam, LPARAM lParam)
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
		case WM_SIZE:
		{
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
		assert(myHandle != nullptr && "A handle is required for grabbing");

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

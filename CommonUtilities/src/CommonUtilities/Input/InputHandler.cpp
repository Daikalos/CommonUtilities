#include <CommonUtilities/Input/InputHandler.h>

#include <windowsx.h>
#include <WinUser.h>
#include <cassert>

using namespace CommonUtilities;

bool InputHandler::IsEnabled() const noexcept
{
	return myEnabled;
}
bool InputHandler::IsInFocus() const noexcept
{
	return (myInFocus || !myFocusInput);
}
bool InputHandler::HasExternalFocus() const noexcept
{
	return myHasExternalFocus;
}

void InputHandler::SetEnabled(bool aFlag)
{
	if (myEnabled == aFlag)
		return;

	myEnabled = aFlag;

	if (myEnabled)
	{
		OnEnable();
	}
	else
	{
		OnDisable();
	}
}
void InputHandler::SetFocusAffectInput(bool aFlag) noexcept
{
	myFocusInput = aFlag;
}

void InputHandler::SetInFocus(bool aFlag)
{
	if (myInFocus == aFlag)
		return;

	myInFocus = aFlag;

	if (!myHasExternalFocus)
	{ 
		if (myInFocus)
		{
			OnFocusGained();
		}
		else
		{
			if (myFocusInput)
			{
				ResetTentativeState();
			}

			OnFocusLost();
		}
	}
}

void InputHandler::SetExternalFocus(bool aFlag)
{
	if (myHasExternalFocus == aFlag)
		return;

	myHasExternalFocus = aFlag;

	if (myInFocus)
	{ 
		if (!myHasExternalFocus)
		{
			OnFocusGained();
		}
		else
		{
			if (myFocusInput)
			{
				ResetTentativeState();
			}

			OnFocusLost();
		}
	}
}

bool InputHandler::HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam)
{
	switch (aMessage)
	{
		case WM_SETFOCUS:
		{
			SetInFocus(true);
			return false;
		}
		case WM_KILLFOCUS:
		{
			SetInFocus(false);
			return false;
		}
	}

	return IsInFocus() ? HandleEventImpl(aMessage, wParam, lParam) : false;
}

void InputHandler::OnEnable()
{

}
void InputHandler::OnDisable()
{

}

void InputHandler::OnFocusGained()
{

}
void InputHandler::OnFocusLost()
{

}
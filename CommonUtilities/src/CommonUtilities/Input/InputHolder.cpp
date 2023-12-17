#include <CommonUtilities/Input/InputHolder.h>

using namespace CommonUtilities;

const KeyboardInput& InputHolder::Keyboard() const noexcept { return myKeyboard; }
KeyboardInput& InputHolder::Keyboard() noexcept             { return myKeyboard; }

const MouseInput& InputHolder::Mouse() const noexcept		{ return myMouse; }
MouseInput& InputHolder::Mouse() noexcept					{ return myMouse; }

const MouseCursor& InputHolder::Cursor() const noexcept		{ return myCursor; }
MouseCursor& InputHolder::Cursor() noexcept					{ return myCursor; }

void InputHolder::Update()
{
	myKeyboard.Update();
	myMouse.Update();
	myCursor.Update();
}

bool InputHolder::HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam)
{
	if (myKeyboard.HandleEvent(aMessage, wParam, lParam))
	{
		return true;
	}
	if (myMouse.HandleEvent(aMessage, wParam, lParam))
	{
		return true;
	}
	if (myCursor.HandleEvent(aMessage, wParam, lParam))
	{
		return true;
	}
}

#include <CommonUtilities/Input/InputHolder.h>

using namespace CommonUtilities;

const KeyboardInput& InputHolder::Keyboard() const noexcept { return myKeyboard; }
KeyboardInput& InputHolder::Keyboard() noexcept             { return myKeyboard; }

const MouseInput& InputHolder::Mouse() const noexcept		{ return myMouse; }
MouseInput& InputHolder::Mouse() noexcept					{ return myMouse; }

void InputHolder::Update()
{
	myKeyboard.Update();
	myMouse.Update();
}

void InputHolder::HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam)
{
	myKeyboard.HandleEvent(aMessage, wParam, lParam);
	myMouse.HandleEvent(aMessage, wParam, lParam);
}

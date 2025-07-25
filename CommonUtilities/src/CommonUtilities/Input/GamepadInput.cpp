#include <CommonUtilities/Input/GamepadInput.h>

#pragma comment(lib, "Xinput.lib")

using namespace CommonUtilities;

bool GamepadInput::ourOccupiedGamepadIndices[XUSER_MAX_COUNT] = { false };

static constexpr WORD locXINPUTButtons[] 
{
      XINPUT_GAMEPAD_A,
      XINPUT_GAMEPAD_B,
      XINPUT_GAMEPAD_X,
      XINPUT_GAMEPAD_Y,
      XINPUT_GAMEPAD_DPAD_UP,
      XINPUT_GAMEPAD_DPAD_DOWN,
      XINPUT_GAMEPAD_DPAD_LEFT,
      XINPUT_GAMEPAD_DPAD_RIGHT,
      XINPUT_GAMEPAD_LEFT_SHOULDER,
      XINPUT_GAMEPAD_RIGHT_SHOULDER,
      XINPUT_GAMEPAD_LEFT_THUMB,
      XINPUT_GAMEPAD_RIGHT_THUMB,
      XINPUT_GAMEPAD_START,
      XINPUT_GAMEPAD_BACK
};

GamepadInput::GamepadInput() 
	: myState()
{
}

const Vector2f& GamepadInput::GetDeadzone() const
{
	return myDeadzone;
}

Vector2f GamepadInput::GetLeftStick() const
{
	return IsEnabled() ? myLeftStick : Vector2f();
}
Vector2f GamepadInput::GetRightStick() const
{
	return IsEnabled() ? myRightStick : Vector2f();
}

float GamepadInput::GetLeftTrigger() const
{
	return IsEnabled() ? myLeftTrigger : 0.0f;
}
float GamepadInput::GetRightTrigger() const
{
	return IsEnabled() ? myRightTrigger : 0.0f;
}

bool GamepadInput::IsHeld(ButtonType aButton) const
{
	return IsEnabled() && myCurrentState[aButton];
}
bool GamepadInput::IsPressed(ButtonType aButton) const
{
	return IsEnabled() && myCurrentState[aButton] && !myPreviousState[aButton];
}
bool GamepadInput::IsReleased(ButtonType aButton) const
{
	return IsEnabled() && !myCurrentState[aButton] && myPreviousState[aButton];
}

bool GamepadInput::IsAnyPressed() const
{
	if (!IsEnabled())
		return false;

	for (std::size_t i = 0; i < Gamepad::ButtonCount; ++i)
	{
		ButtonType button = (ButtonType)i;
		if (myCurrentState[button] && !myPreviousState[button])
		{
			return true;
		}
	}

	return false;
}

int GamepadInput::GetIndex() const
{
	return myIndex;
}
bool GamepadInput::IsConnected() const
{
	return (myIndex != -1) ? Gamepad::IsConnected(myIndex) : false;
}

void GamepadInput::Connect()
{
	myActive = true;
}
void GamepadInput::Disconnect()
{
	if (myIndex != -1)
	{
		ourOccupiedGamepadIndices[myIndex] = false;
	}

	myIndex			= -1;

	myLeftStick		= Vector2f{};
	myRightStick	= Vector2f{};

	myLeftTrigger	= 0.0f;
	myRightTrigger	= 0.0f;

	for (int i = 0; i < Gamepad::ButtonCount; ++i)
	{
		myPreviousState[i] = myCurrentState[i] = myTentativeState[i] = false;
	}

	myActive = false;
}

void GamepadInput::Rumble(float aLeftMotor, float aRightMotor)
{
	if (IsConnected())
	{
		Gamepad::Rumble(myIndex, aLeftMotor, aRightMotor);
	}
}

void GamepadInput::SetDeadzoneX(float aDeadzoneX)
{
	myDeadzone.x = aDeadzoneX;
}
void GamepadInput::SetDeadzoneY(float aDeadzoneY)
{
	myDeadzone.y = aDeadzoneY;
}

void GamepadInput::Update()
{
	if (!myActive)
	{
		return;
	}

	// try to connect to a new port if current is invalid
	if (myIndex == -1 && !TryConnect())
	{
		return;
	}
	else if (auto newState = Gamepad::GetState(myIndex); newState.has_value()) // retrieve current state
	{
		myState = newState.value();
	}
	else // current state is invalid, try and find a new port
	{
		if (myIndex != -1)
		{
			ourOccupiedGamepadIndices[myIndex] = false;
		}

		myIndex = -1;
		Update();

		return;
	}

	// update left stick
	{
		float normLX = std::fmaxf(-1, (float)myState.Gamepad.sThumbLX / 32767);
		float normLY = std::fmaxf(-1, (float)myState.Gamepad.sThumbLY / 32767);

		float absLX = std::fabs(normLX);
		float absLY = std::fabs(normLY);

		myLeftStick.x = (absLX < myDeadzone.x || !IsInFocus()) ? 0 : (absLX - myDeadzone.x) * (normLX / absLX);
		myLeftStick.y = (absLY < myDeadzone.y || !IsInFocus()) ? 0 : (absLY - myDeadzone.y) * (normLY / absLY);

		if (myDeadzone.x != 1.0f) myLeftStick.x *= 1.0f / (1.0f - myDeadzone.x);
		if (myDeadzone.y != 1.0f) myLeftStick.y *= 1.0f / (1.0f - myDeadzone.y);
	}

	// update right stick
	{
		float normRX = std::fmaxf(-1, (float)myState.Gamepad.sThumbRX / 32767);
		float normRY = std::fmaxf(-1, (float)myState.Gamepad.sThumbRY / 32767);

		float absRX = std::fabs(normRX);
		float absRY = std::fabs(normRY);

		myRightStick.x = (absRX < myDeadzone.x || !IsInFocus()) ? 0 : (absRX - myDeadzone.x) * (normRX / absRX);
		myRightStick.y = (absRY < myDeadzone.y || !IsInFocus()) ? 0 : (absRY - myDeadzone.y) * (normRY / absRY);

		if (myDeadzone.x != 1.0f) myRightStick.x *= 1.0f / (1.0f - myDeadzone.x);
		if (myDeadzone.y != 1.0f) myRightStick.y *= 1.0f / (1.0f - myDeadzone.y);
	}

	// update triggers
	{
		BYTE leftTrigger = myState.Gamepad.bLeftTrigger;
		myLeftTrigger = (leftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) && IsInFocus() ? (leftTrigger / 255.0f) : 0.0f;

		BYTE rightTrigger = myState.Gamepad.bRightTrigger;
		myRightTrigger = (rightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) && IsInFocus() ? (rightTrigger / 255.0f) : 0.0f;
	}

	// update buttons
	{
		for (int i = 0; i < Gamepad::ButtonCount && IsInFocus(); ++i)
		{
			myTentativeState[i] = (myState.Gamepad.wButtons & locXINPUTButtons[i]) == locXINPUTButtons[i];
		}

		myPreviousState = myCurrentState;
		myCurrentState = myTentativeState;
	}
}

bool GamepadInput::TryConnect()
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT && myIndex == -1; i++)
	{
		if (ourOccupiedGamepadIndices[i]) 
			continue;

		if (auto gamepadState = Gamepad::GetState(i); gamepadState.has_value())
		{
			myState = gamepadState.value();
			myIndex = i;

			return true;
		}
	}

	myIndex = -1;

	return false;
}

bool GamepadInput::HandleEventImpl(UNSD UINT aMessage, UNSD WPARAM wParam, UNSD LPARAM lParam)
{
	return false;
}

void GamepadInput::ResetTentativeState()
{
	for (int i = 0; i < Gamepad::ButtonCount; ++i)
	{
		myTentativeState[i] = false;
	}
}

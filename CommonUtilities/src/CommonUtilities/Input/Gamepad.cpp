#include <CommonUtilities/pch.h>
#include <CommonUtilities/Input/Gamepad.h>

#include <limits>

#pragma comment(lib, "Xinput.lib")

using namespace CommonUtilities;

std::optional<XINPUT_STATE> Gamepad::GetState(int aGamepadIndex)
{
    XINPUT_STATE gamepadState{};
    DWORD result = XInputGetState(aGamepadIndex, &gamepadState);
    return (result == ERROR_SUCCESS) ? gamepadState : std::optional<XINPUT_STATE>{};
}

bool Gamepad::IsConnected(int aGamepadIndex)
{
	XINPUT_STATE gamepadState{};
    return XInputGetState(aGamepadIndex, &gamepadState) == ERROR_SUCCESS;
}

void Gamepad::Rumble(int aGamepadIndex, float aLeftMotor, float aRightMotor)
{
	XINPUT_VIBRATION vibrationState{};

	int leftMotor	= int(aLeftMotor * (float)(std::numeric_limits<unsigned short>::max)());
	int rightMotor	= int(aRightMotor * (float)(std::numeric_limits<unsigned short>::max)());

	vibrationState.wLeftMotorSpeed = static_cast<WORD>(leftMotor);
	vibrationState.wRightMotorSpeed = static_cast<WORD>(rightMotor);

	XInputSetState(aGamepadIndex, &vibrationState);
}

#include <CommonUtilities/Input/Gamepad.h>

#pragma comment(lib, "Xinput.lib")

using namespace CommonUtilities;

std::optional<XINPUT_STATE> Gamepad::GetState(int aGamepadIndex)
{
    XINPUT_STATE gamepadState;
    ZeroMemory(&gamepadState, sizeof(XINPUT_STATE));
    DWORD result = XInputGetState(aGamepadIndex, &gamepadState);
    return (result == ERROR_SUCCESS) ? gamepadState : std::optional<XINPUT_STATE>{};
}

bool Gamepad::IsConnected(int aGamepadIndex)
{
    XINPUT_STATE gamepadState;
    ZeroMemory(&gamepadState, sizeof(XINPUT_STATE));
    return XInputGetState(aGamepadIndex, &gamepadState) == ERROR_SUCCESS;
}

void Gamepad::Rumble(int aGamepadIndex, float aLeftMotor, float aRightMotor)
{
	XINPUT_VIBRATION vibrationState;
	ZeroMemory(&vibrationState, sizeof(XINPUT_VIBRATION));

	int leftMotor = int(aLeftMotor * 65535.0f);
	int rightMotor = int(aRightMotor * 65535.0f);

	vibrationState.wLeftMotorSpeed = static_cast<WORD>(leftMotor);
	vibrationState.wRightMotorSpeed = static_cast<WORD>(rightMotor);

	XInputSetState(aGamepadIndex, &vibrationState);
}

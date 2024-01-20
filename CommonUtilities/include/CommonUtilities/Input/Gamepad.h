#pragma once

#include <Windows.h>
#include <Xinput.h>
#include <optional>

#include <CommonUtilities/Config.h>

namespace CommonUtilities::Gamepad
{
	enum Button
	{
		A,
		B,
		X,
		Y,
		DPadUp,
		DPadDown,
		DPadLeft,
		DPadRight,
		LShoulder,
		RShoulder,
		LThumbstick,
		RThumbstick,
		Start,
		Back,
		ButtonCount
	};

	COMMON_UTILITIES_API NODISC std::optional<XINPUT_STATE> GetState(int aGamepadIndex);
	COMMON_UTILITIES_API NODISC bool IsConnected(int aGamepadIndex);
	COMMON_UTILITIES_API void Rumble(int aGamepadIndex, float aLeftMotor = 0.0f, float aRightMotor = 0.0f);
}
#pragma once

#include <CommonUtilities/System/WindowsHeader.h>

#include <Xinput.h>
#include <array>

#include <CommonUtilities/Input/InputHandler.h>
#include <CommonUtilities/Input/Gamepad.h>
#include <CommonUtilities/Math/Vector2.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	class COMMON_UTILITIES_API GamepadInput final : public InputHandler
	{
	public:
		using ButtonType = Gamepad::Button;

		GamepadInput(int aGamepadIndex = 0);
		~GamepadInput() = default;

		NODISC const Vector2f& GetDeadzone() const;

		NODISC Vector2f GetLeftStick() const;
		NODISC Vector2f GetRightStick() const;

		NODISC float GetLeftTrigger() const;
		NODISC float GetRightTrigger() const;

		NODISC bool IsHeld(ButtonType aButton) const;
		NODISC bool IsPressed(ButtonType aButton) const;
		NODISC bool IsReleased(ButtonType aButton) const;

		NODISC int GetIndex() const;
		NODISC bool IsConnected() const;

		void Connect();
		void Disconnect();

		/// Vibrate the controller
		/// 
		/// \param LeftMotor: Amount to vibrate on left motor (0.0f to cancel, 1.0f is max speed)
		/// \param RightMotor: Amount to vibrate on right motor
		/// 
		void Rumble(float aLeftMotor = 0.0f, float aRightMotor = 0.0f);

		void SetDeadzoneX(float aDeadzoneX);
		void SetDeadzoneY(float aDeadzoneY);

		void Update() override;

	private:
		bool TryConnect();

		void ResetTentativeState() override;

		bool HandleEventImpl(UINT aMessage, WPARAM wParam, LPARAM lParam) override;

		XINPUT_STATE	myState;
		int				myIndex			{-1};

		Vector2f		myDeadzone		{0.05f, 0.02f};
		Vector2f		myLeftStick;
		Vector2f		myRightStick;
		float			myLeftTrigger	{0.0f};
		float			myRightTrigger	{0.0f};
		bool			myActive		{false};

		std::array<bool, Gamepad::ButtonCount> myCurrentState	= {false};
		std::array<bool, Gamepad::ButtonCount> myPreviousState	= {false};
		std::array<bool, Gamepad::ButtonCount> myTentativeState	= {false};
	};
}
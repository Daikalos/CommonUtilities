#pragma once

#include <array>

#include <CommonUtilities/Input/InputHandler.h>
#include <CommonUtilities/Input/Mouse.h>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	class COMMON_UTILITIES_API MouseInput final : public InputHandler
	{
	public:
		using ButtonType = Mouse::Button;

		MouseInput() = default;
		~MouseInput() = default;

		NODISC bool ScrollUp() const noexcept;
		NODISC bool ScrollDown() const noexcept;

		NODISC bool IsHeld(ButtonType aButton) const;
		NODISC bool IsPressed(ButtonType aButton) const;
		NODISC bool IsReleased(ButtonType aButton) const;

		NODISC bool IsAnyPressed() const;

		void SetScrollThreshold(float aScrollThreshold);

		void Update() override;

	private:
		void ResetTentativeState() override;

		bool HandleEventImpl(UINT aMessage, WPARAM wParam, LPARAM lParam) override;

		NODISC bool SetTentativeState(WPARAM wParam, bool aState);

		float myScrollDelta				{0.0f};
		float myTentativeScrollDelta	{0.0f};
		float myScrollThreshold			{0.01f};

		std::array<bool, Mouse::ButtonCount> myCurrentState		= {false};
		std::array<bool, Mouse::ButtonCount> myPreviousState	= {false};
		std::array<bool, Mouse::ButtonCount> myTentativeState	= {false};
	};
}
#pragma once

#include <array>

#include <CommonUtilities/System/WindowsHeader.h>
#include <CommonUtilities/Utility/NonCopyable.h>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	///	Base class for input types
	/// 
	class COMMON_UTILITIES_API InputHandler : private NonCopyable
	{
	public:
		InputHandler() = default;
		virtual ~InputHandler() = default;

		NODISC bool GetEnabled() const noexcept;
		NODISC bool GetInFocus() const noexcept;
		NODISC bool GetHasExternalFocus() const noexcept;

		/// Enable or disable the InputHandler, will cause all input to return false or 0.0f.
		/// 
		void SetEnabled(bool aFlag) noexcept;

		/// Enable or disable for focus to affect whether input gets enabled or disabled
		/// 
		void SetFocusAffectInput(bool aFlag) noexcept;

		/// Manually enable or disable that input has focus
		/// 
		void SetInFocus(bool aFlag);

		void SetHasExternalFocus(bool aFlag);

		///	Update which should determine the current and previous state of the input
		/// 
		virtual void Update() = 0;

		NODISC bool HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam);

	protected:
		virtual void OnEnable();
		virtual void OnDisable();

		virtual void OnFocusGained();
		virtual void OnFocusLost();

		virtual void ResetTentativeState() = 0;

		virtual bool HandleEventImpl(UINT aMessage, WPARAM wParam, LPARAM lParam) = 0;

		bool	myEnabled			{true};		// is always enabled initially
		bool	myInFocus			{true};
		bool	myFocusInput		{true};		// focus affects input at start
		bool	myHasExternalFocus	{false};
	};

	namespace deprecated
	{
		class COMMON_UTILITIES_API InputHandler
		{
		public:
			InputHandler();

			bool HandleEvents(HWND aHandle, UINT aMessage, WPARAM wParam, LPARAM lParam);

			bool HandleWindowEvents(HWND aHandle, UINT aMessage, WPARAM wParam, LPARAM lParam);
			bool HandleKeyboardEvents(UINT aMessage, WPARAM wParam, LPARAM lParam);
			bool HandleMouseEvents(UINT aMessage, WPARAM wParam, LPARAM lParam);

			bool IsKeyDown(int aKeyCode) const;
			bool IsKeyPressed(int aKeyCode) const;
			bool IsKeyReleased(int aKeyCode) const;

			float GetScrollDelta() const;
			bool GetCursorGrabbed() const;

			POINT GetMousePosition() const;
			POINT GetMousePosition(HWND aHandle) const;

			POINT GetMouseDelta() const;

			void SetMousePosition(const POINT& aPoint);
			void SetMousePosition(POINT aPoint, HWND aHandle);

			void SetCursorGrabbed(bool aGrabbed, HWND aHandle);

			void UpdateInput();

		private:
			void GrabCursor(bool aGrabbed, HWND aHandle);

			static const int ourKeyCount = 256;

			std::array<bool, ourKeyCount>	myCurrentState;
			std::array<bool, ourKeyCount>	myPreviousState;
			std::array<bool, ourKeyCount>	myInputState;
			POINT							myMousePosition;
			POINT							myPreviousMousePos;
			POINT							myMouseDeltaPos;
			float							myScrollDelta;
			bool							myCursorGrabbed;
			bool							myInFocus;
		};
	}
}
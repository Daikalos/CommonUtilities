#pragma once

#include <Windows.h>
#include <array>

#include <CommonUtilities/Utility/NonCopyable.h>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	///	Base class for Keyboard, Mouse, and Joystick 
	/// 
	class COMMON_UTILITIES_API InputHandler : private NonCopyable
	{
	public:
		InputHandler() = default;
		virtual ~InputHandler() = default;

		/// Enable or disable the InputHandler, will cause all input to return false or 0.0f.
		/// 
		void SetEnabled(bool flag) noexcept;

		///	Determine the threshold before the button/key is considered held
		/// 
		void SetHeldThreshold(float value) noexcept;

		///	Always put Update before HandleEvent for input to work correctly
		/// 
		virtual void Update() = 0;

		virtual bool HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam) = 0;

	protected:
		bool	myEnabled		{true};	// is always enabled initially
		float	myHeldThreshold	{0.1f}; // threshold before button/key is considered held
	};

	namespace old
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
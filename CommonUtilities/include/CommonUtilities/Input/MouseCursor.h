#pragma once

#include <CommonUtilities/Math/Vector2.hpp>
#include <CommonUtilities/Input/InputHandler.h>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	class COMMON_UTILITIES_API MouseCursor final : public InputHandler
	{
	public:
		MouseCursor() = default;
		~MouseCursor() = default;

		MouseCursor(HWND aHandle);

		NODISC bool GetGrabbed() const noexcept;
		NODISC bool GetVisible() const noexcept;

		NODISC const Vector2i& GetPosition() const noexcept;
		NODISC const Vector2i& GetMouseDelta() const noexcept;
		NODISC const Vector2i& GetWindowSize() const noexcept;

		void SetHandle(HWND aHandle);

		void SetPosition(const Vector2i& aPoint);
		void SetRelativePosition(const Vector2i& aPoint);

		void SetGrabbed(bool aGrabbed);
		void SetVisible(bool aState);
		void SetLocked(bool aLocked);

		void Update() override;

	private:
		bool HandleEventImpl(UINT aMessage, WPARAM wParam, LPARAM lParam) override;

		void GrabCursor(bool aGrabbed);

		HWND		myHandle	{nullptr};
		Vector2i	myTentativePosition;
		Vector2i	myCurrentPosition;
		Vector2i	myPreviousPosition;
		Vector2i	myTentativeMoveDelta;
		Vector2i	myMoveDelta;
		Vector2i	myWindowSize;
		bool		myIsVisible {true};
		bool		myIsGrabbed {false};
		bool		myIsLocked	{false};

		// Inherited via InputHandler
		void ResetTentativeState() override;
	};
}
#pragma once

#include <CommonUtilities/Math/Vector2.hpp>
#include <CommonUtilities/Input/InputHandler.h>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	class COMMON_UTILITIES_API MouseCursor final : public InputHandler
	{
	public:
		MouseCursor();
		~MouseCursor();

		MouseCursor(HWND aHandle);

		bool GetGrabbed() const;
		bool GetVisible() const;

		const Vector2i& GetPosition() const;
		Vector2i GetRelativePosition() const;

		const Vector2i& GetMouseDelta() const;

		void SetHandle(HWND aHandle);

		void SetPosition(const Vector2i& aPoint);
		void SetRelativePosition(const Vector2i& aPoint);

		void SetGrabbed(bool aGrabbed);
		void SetVisible(bool aState);

		void Update() override;
		bool HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam) override;

	private:
		void GrabCursor(bool aGrabbed);

		HWND		myHandle	{nullptr};
		Vector2i	myTentativePosition;
		Vector2i	myCurrentPosition;
		Vector2i	myPreviousPosition;
		Vector2i	myTentativeMoveDelta;
		Vector2i	myMoveDelta;
		bool		myIsVisible {true};
		bool		myIsGrabbed {false};
	};
}
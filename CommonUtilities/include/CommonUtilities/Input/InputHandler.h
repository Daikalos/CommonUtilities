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

		NODISC bool IsEnabled() const noexcept;
		NODISC bool IsInFocus() const noexcept;
		NODISC bool HasExternalFocus() const noexcept;

		/// Enable or disable the InputHandler, will cause all input to return false or 0.0f.
		/// 
		void SetEnabled(bool aFlag);

		/// Enable or disable for focus to affect whether input gets enabled or disabled
		/// 
		void SetFocusAffectInput(bool aFlag) noexcept;

		/// Manually enable or disable that input has focus
		/// 
		void SetInFocus(bool aFlag);

		/// Whether something inside the application is stealing input, for example, ImGui
		/// 
		void SetExternalFocus(bool aFlag);

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
}
#pragma once

#include <functional>
#include <vector>
#include <ranges>

#include <CommonUtilities/Utility/ContainerUtils.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<class I>
	concept HasButtonInput = requires(I input)
	{
		{ input.IsPressed(typename I::ButtonType()) } -> std::same_as<bool>;
		{ input.IsReleased(typename I::ButtonType()) } -> std::same_as<bool>;
		{ input.IsHeld(typename I::ButtonType()) } -> std::same_as<bool>;
	};

	namespace bt
	{
		enum ButtonTrigger
		{
			None = -1,
			Pressed,
			Released,
			Held,
		};
	}

	///	ButtonEvent is small container class for mapping functions to individual key events. This was created to 
	/// have the same lifetime as the provided functions have in order to prevent easy-to-miss UB.
	/// 
	/// \param T: Type of input to read (e.g., keyboard, mouse)
	/// \param Args: Optional arguments that are passed to function when called
	/// 
	template<class T, typename... Args> requires HasButtonInput<T>
	class ButtonEvent final
	{
	public:
		using ButtonType = typename T::ButtonType; // relies on there being a ButtonType defined in input to access its type of button

		~ButtonEvent() = default;

		ButtonEvent(const T* input = nullptr);

		ButtonEvent(const ButtonEvent& aOther);
		ButtonEvent(ButtonEvent&& aOther) noexcept;

		auto operator=(const ButtonEvent& aOther) -> ButtonEvent&;
		auto operator=(ButtonEvent&& aOther) noexcept -> ButtonEvent&;

		void operator()(Args... someArgs);

		NODISC bool IsConnected() const noexcept;

		void Execute(Args... someArgs);

		void Connect(const T& aInput);
		void Disconnect();
	
		template<typename Func>
		void Add(Func&& aFunc, ButtonType aButton, bt::ButtonTrigger aTrigger)
		{
			myCallbacks.emplace_back(std::forward<Func>(aFunc), aButton, aTrigger);
		}

		template<typename Func, typename... InnerArgs>
		void Add(ButtonType aButton, bt::ButtonTrigger aTrigger, Func&& aFunc, InnerArgs&&... someInnerArgs)
		{
			Add([aFunc = std::forward<Func>(aFunc), ...someInnerArgs = std::forward<InnerArgs>(someInnerArgs)](Args... someArgs)
				{
					std::invoke(aFunc, someInnerArgs..., someArgs...);
				}, aButton, aTrigger);
		}

		/// Removes all callbacks that are associated with the provided button.
		/// 
		/// \param Button: Button type to look for when removing.
		/// 
		void Remove(ButtonType aButton);

		/// Removes all callbacks that are associated with the provided trigger.
		/// 
		/// \param Trigger: Trigger type to look for when removing.
		/// 
		void Remove(bt::ButtonTrigger aTrigger);

		/// \return Number of added callbacks.
		/// 
		NODISC std::size_t Count() const noexcept;

		/// Clears all added callbacks.
		/// 
		void Clear();

	private:
		struct ButtonCallback
		{
			std::function<void(Args...)>	func;
			ButtonType						button	{};			// button type to listen for
			bt::ButtonTrigger				trigger	{bt::None};	// button trigger to listen for
		};

		std::vector<ButtonCallback> myCallbacks;
		const T* myInput {nullptr};
	};

	template<class T, typename... Args> requires HasButtonInput<T>
	inline ButtonEvent<T, Args...>::ButtonEvent(const T* input) 
		: myCallbacks(), myInput(input) { }

	template<class T, typename... Args> requires HasButtonInput<T>
	inline ButtonEvent<T, Args...>::ButtonEvent(const ButtonEvent& rhs)
		: myCallbacks(rhs.myCallbacks), myInput(rhs.myInput)
	{

	}

	template<class T, typename... Args> requires HasButtonInput<T>
	inline ButtonEvent<T, Args...>::ButtonEvent(ButtonEvent&& aOther) noexcept
		: myCallbacks(std::move(aOther.myCallbacks)), myInput(std::exchange(aOther.myInput, nullptr))
	{

	}

	template<class T, typename... Args> requires HasButtonInput<T>
	inline auto ButtonEvent<T, Args...>::operator=(const ButtonEvent& aOther) -> ButtonEvent&
	{
		myCallbacks = aOther.myCallbacks;
		myInput = aOther.myInput;

		return *this;
	}

	template<class T, typename... Args> requires HasButtonInput<T>
	inline auto ButtonEvent<T, Args...>::operator=(ButtonEvent&& aOther) noexcept -> ButtonEvent&
	{
		myCallbacks = std::move(aOther.myCallbacks);
		myInput		= std::exchange(aOther.myInput, nullptr);

		return *this;
	}

	template<class T, typename... Args> requires HasButtonInput<T>
	inline void ButtonEvent<T, Args...>::operator()(Args... someArgs)
	{
		Execute(someArgs...);
	}

	template<class T, typename ...Args> requires HasButtonInput<T>
	inline bool ButtonEvent<T, Args...>::IsConnected() const noexcept
	{
		return myInput != nullptr;
	}

	template<class T, typename... Args> requires HasButtonInput<T>
	inline void ButtonEvent<T, Args...>::Execute(Args... someArgs)
	{
		if (myInput == nullptr)
			return;

		const auto CheckTriggered = [this](const ButtonCallback& aCallback) -> bool
		{
			switch (aCallback.trigger)
			{
				case bt::Pressed:	return myInput->IsPressed(aCallback.button);
				case bt::Released:	return myInput->IsReleased(aCallback.button);
				case bt::Held:		return myInput->IsHeld(aCallback.button);
			}

			return false;
		};

		for (const auto& callback : myCallbacks)
		{
			if (CheckTriggered(callback) && callback.func)
				callback.func(someArgs...);
		}
	}

	template<class T, typename... Args> requires HasButtonInput<T>
	inline void ButtonEvent<T, Args...>::Connect(const T& aInput)
	{
		myInput = &aInput;
	}
	template<class T, typename... Args> requires HasButtonInput<T>
	inline void ButtonEvent<T, Args...>::Disconnect()
	{
		myInput = nullptr;
	}

	template<class T, typename... Args> requires HasButtonInput<T>
	inline void ButtonEvent<T, Args...>::Remove(ButtonType aButton)
	{
		std::erase(std::ranges::remove_if(myCallbacks.begin(), myCallbacks.end(),
			[&aButton](const ButtonCallback& aCallback)
			{
				return aCallback.button == aButton;
			}), myCallbacks.end());
	}

	template<class T, typename... Args> requires HasButtonInput<T>
	inline void ButtonEvent<T, Args...>::Remove(bt::ButtonTrigger aTrigger)
	{
		std::erase(std::ranges::remove_if(myCallbacks.begin(), myCallbacks.end(),
			[&aTrigger](const ButtonCallback& aCallback)
			{
				return aCallback.trigger == aTrigger;
			}), myCallbacks.end());
	}

	template<class T, typename... Args> requires HasButtonInput<T>
	inline std::size_t ButtonEvent<T, Args...>::Count() const noexcept
	{
		return myCallbacks.size();
	}

	template<class T, typename... Args> requires HasButtonInput<T>
	inline void ButtonEvent<T, Args...>::Clear()
	{
		myCallbacks.clear();
	}
}
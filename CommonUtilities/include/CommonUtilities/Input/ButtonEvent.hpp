#pragma once

#include <functional>
#include <vector>
#include <algorithm>

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

	enum ButtonTrigger
	{
		BT_None = -1,
		BT_Pressed,
		BT_Released,
		BT_Held,
	};

	///	ButtonEvent is small container class for mapping functions to individual key events. This was created to 
	/// have the same lifetime as the provided functions have in order to prevent easy-to-miss UB.
	/// 
	template<class T, typename... Args> requires HasButtonInput<T>
	class ButtonEvent final
	{
	public:
		using ButtonType = typename T::ButtonType;

		struct ButtonCallback
		{
			std::function<void(Args...)>	func;
			ButtonType						button		{};			// button type to listen for
			ButtonTrigger					trigger		{BT_None};	// button trigger to listen for
			float							priority	{0.0f};		// determines position in callbacks
		};

		ButtonEvent() = default;
		ButtonEvent(const T& input);

		ButtonEvent(const ButtonEvent& aOther);
		ButtonEvent(ButtonEvent&& aOther) noexcept;

		auto operator=(const ButtonEvent& aOther) -> ButtonEvent&;
		auto operator=(ButtonEvent&& aOther) noexcept -> ButtonEvent&;

		void Execute(Args... someArgs);
		void operator()(Args... someArgs);

		void SetInput(const T* aInput);
	
		template<typename Func>
		void Add(ButtonType aButton, ButtonTrigger aTrigger, Func&& aFunc, float aPriority = {})
		{
			const auto it = std::upper_bound(myCallbacks.begin(), myCallbacks.end(), aPriority,
				[](float aPriority, const ButtonCallback& aCallback)
				{
					return aPriority > aCallback.priority;
				});

			myCallbacks.emplace(it, std::forward<Func>(aFunc), aButton, aTrigger, aPriority);
		}

		template<typename Func, typename... InnerArgs>
		void Add(ButtonType aButton, ButtonTrigger aTrigger, float aPriority, Func&& aFunc, InnerArgs&&... someInnerArgs)
		{
			Add(aButton, aTrigger,
				[aFunc = std::forward<Func>(aFunc), ...someInnerArgs = std::forward<InnerArgs>(someInnerArgs)](Args... someArgs)
				{
					std::invoke(aFunc, someInnerArgs..., someArgs...);
				}, aPriority);
		}

		template<typename Func, typename... InnerArgs>
		void Add(ButtonType aButton, ButtonTrigger aTrigger, Func&& aFunc, InnerArgs&&... someInnerArgs)
		{
			Add(aButton, aTrigger, {}, std::forward<Func>(aFunc), std::forward<InnerArgs>(someInnerArgs)...);
		}

		void Remove(ButtonType aButton);
		void Remove(ButtonTrigger aTrigger);

		NODISC std::size_t Count() const noexcept;
		void Clear();

	private:
		std::vector<ButtonCallback> myCallbacks;
		const T* myInput {nullptr};
	};

	template<class T, typename... Args> requires HasButtonInput<T>
	inline ButtonEvent<T, Args...>::ButtonEvent(const T& input) 
		: myCallbacks(), myInput(&input) { }

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
	inline void ButtonEvent<T, Args...>::Execute(Args... someArgs)
	{
		if (myInput == nullptr)
			return;

		const auto CheckTriggered = [this](const ButtonCallback& aCallback) -> bool
		{
			switch (aCallback.trigger)
			{
				case BT_Pressed:	return myInput->IsPressed(aCallback.button);
				case BT_Released:	return myInput->IsReleased(aCallback.button);
				case BT_Held:		return myInput->IsHeld(aCallback.button);
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
	inline void ButtonEvent<T, Args...>::operator()(Args... someArgs)
	{
		Execute(someArgs...);
	}

	template<class T, typename... Args> requires HasButtonInput<T>
	inline void ButtonEvent<T, Args...>::SetInput(const T* aInput)
	{
		myInput = aInput;
	}

	template<class T, typename... Args> requires HasButtonInput<T>
	inline void ButtonEvent<T, Args...>::Remove(ButtonType aButton)
	{
		std::erase(std::remove_if(myCallbacks.begin(), myCallbacks.end(), 
			[&aButton](const ButtonCallback& aCallback)
			{
				return aCallback.button == aButton;
			}), myCallbacks.end());
	}

	template<class T, typename... Args> requires HasButtonInput<T>
	inline void ButtonEvent<T, Args...>::Remove(ButtonTrigger aTrigger)
	{
		std::erase(std::remove_if(myCallbacks.begin(), myCallbacks.end(),
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
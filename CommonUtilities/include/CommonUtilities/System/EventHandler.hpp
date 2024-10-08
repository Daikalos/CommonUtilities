#pragma once

#include <functional>
#include <utility>

#include "EventIdentifiers.h"

namespace CommonUtilities
{
	template<typename... Args>
	class EventHandler
	{
	public:
		using FunctionType = std::function<void(Args...)>;

		explicit EventHandler(const FunctionType& aFunc);
		explicit EventHandler(FunctionType&& aFunc);

		EventHandler(const EventHandler& aOther);
		EventHandler(EventHandler&& aOther) noexcept;

		auto operator=(const EventHandler& aOther) -> EventHandler&;
		auto operator=(EventHandler&& aOther) noexcept -> EventHandler&;

		void operator()(Args... someParams) const;

		bool operator==(const EventHandler& aOther) const;
		bool operator!=(const EventHandler& aOther) const;

		evnt::IDType GetID() const noexcept;

	private:
		FunctionType myFunc;
		evnt::IDType myID {NULL};

		static evnt::AtomicID ourIDCounter;
	};

	template<typename... Args>
	inline evnt::AtomicID EventHandler<Args...>::ourIDCounter = 1; // 0 reserved for null

	template<typename ...Args>
	inline EventHandler<Args...>::EventHandler(const FunctionType& aFunc)
		: myFunc(aFunc)
		, myID(ourIDCounter++)
	{

	}
	template<typename ...Args>
	inline EventHandler<Args...>::EventHandler(FunctionType&& aFunc)
		: myFunc(std::move(aFunc))
		, myID(ourIDCounter++)
	{

	}

	template<typename ...Args>
	inline EventHandler<Args...>::EventHandler(const EventHandler& aOther)
		: myFunc(aOther.myFunc)
		, myID(ourIDCounter++) // copying increases the ID to keep this unique
	{

	}
	template<typename ...Args>
	inline EventHandler<Args...>::EventHandler(EventHandler&& aOther) noexcept
		: myFunc(std::move(aOther.myFunc))
		, myID(std::exchange(aOther.myID, NULL))
	{

	}

	template<typename ...Args>
	inline auto EventHandler<Args...>::operator=(const EventHandler& aOther) -> EventHandler&
	{
		if (*this == aOther)
			return *this;

		myFunc = aOther.myFunc;
		myID = ourIDCounter++;

		return *this;
	}
	template<typename ...Args>
	inline auto EventHandler<Args...>::operator=(EventHandler&& aOther) noexcept -> EventHandler&
	{
		if (*this == aOther)
			return *this;

		myFunc = std::move(aOther.myFunc);
		myID = std::exchange(aOther.myID, NULL);

		return *this;
	}

	template<typename ...Args>
	inline void EventHandler<Args...>::operator()(Args... someParams) const
	{
		myFunc(someParams...);
	}

	template<typename ...Args>
	inline bool EventHandler<Args...>::operator==(const EventHandler& aOther) const
	{
		return myID == aOther.myID;
	}
	template<typename ...Args>
	inline bool EventHandler<Args...>::operator!=(const EventHandler& aOther) const
	{
		return !(*this == aOther);
	}

	template<typename ...Args>
	inline evnt::IDType EventHandler<Args...>::GetID() const noexcept
	{
		return myID;
	}
}
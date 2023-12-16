#pragma once

#include <vector>
#include <future>

#include <CommonUtilities/Config.h>

#include "EventHandler.hpp"
#include "IEvent.h"

namespace CommonUtilities
{
	/// Can hold any number of functions that may all be executed manually.
	/// 
	/// Based on article by Shmuel Zang:
	/// https://www.codeproject.com/Articles/1256352/CppEvent-How-to-Implement-Events-using-Standard-Cp
	/// 
	/// \param Args: Parameters for function
	/// 
	template<typename... Args>
	class Event : public IEvent
	{
	public:
		using HandlerType = EventHandler<Args...>;

		Event();
		~Event();

		Event(const Event& aOther);
		Event(Event&& aOther);

		auto operator=(const Event& aOther) -> Event&;
		auto operator=(Event&& aOther) noexcept -> Event&;

		explicit operator bool() const;

		void operator()(Args... someParams) const;

		evnt::IDType operator+=(const HandlerType& aHandler);
		evnt::IDType operator+=(const typename HandlerType::FuncType& aHandler);

		evnt::IDType operator-=(const HandlerType& aHandler);
		evnt::IDType operator-=(evnt::IDType aHandlerID) override;

		NODISC std::size_t Count() const noexcept override;
		NODISC bool IsEmpty() const noexcept override;

		void Reserve(std::size_t aSize) override;
		void Clear() noexcept override;

		evnt::IDType Add(const HandlerType& aHandler);
		evnt::IDType Add(const typename HandlerType::FuncType& aHandler);

		bool Remove(const HandlerType& aHandler);
		bool RemoveID(evnt::IDType aHandlerID) override;

		void Call(Args... someParams) const;
		std::future<void> CallAsync(Args... someParams) const;

	private:
		using HandlerList = std::vector<HandlerType>;

		HandlerList	myHandlers;
	};

	template<typename... Args>
	inline Event<Args...>::Event() = default;

	template<typename... Args>
	inline Event<Args...>::~Event() = default;

	template<typename... Args>
	inline Event<Args...>::Event(const Event& aOther)
		: myHandlers(aOther.myHandlers)
	{

	}
	template<typename... Args>
	inline Event<Args...>::Event(Event&& aOther)
		: myHandlers(std::move(aOther.myHandlers))
	{

	}

	template<typename... Args>
	inline auto Event<Args...>::operator=(const Event& aOther) -> Event&
	{
		myHandlers = aOther.myHandlers;
		return *this;
	}
	template<typename... Args>
	inline auto Event<Args...>::operator=(Event&& aOther) noexcept -> Event&
	{
		myHandlers = std::move(aOther.myHandlers);
		return *this;
	}

	template<typename... Args>
	inline Event<Args...>::operator bool() const
	{
		return !IsEmpty();
	}

	template<typename... Args>
	inline void Event<Args...>::operator()(Args ...someParams) const
	{
		Call(someParams...);
	}

	template<typename... Args>
	inline evnt::IDType Event<Args...>::operator+=(const HandlerType& aHandler)
	{
		return Add(aHandler);
	}
	template<typename... Args>
	inline evnt::IDType Event<Args...>::operator+=(const typename HandlerType::FuncType& aHandler)
	{
		return Add(aHandler);
	}

	template<typename... Args>
	inline evnt::IDType Event<Args...>::operator-=(const HandlerType& aHandler)
	{
		return Remove(aHandler);
	}
	template<typename... Args>
	inline evnt::IDType Event<Args...>::operator-=(evnt::IDType aHandlerID)
	{
		return RemoveID(aHandlerID);
	}

	template<typename... Args>
	inline std::size_t Event<Args...>::Count() const noexcept
	{
		return myHandlers.size();
	}
	template<typename... Args>
	inline bool Event<Args...>::IsEmpty() const noexcept
	{
		return myHandlers.empty();
	}
	template<typename... Args>
	inline void Event<Args...>::Reserve(std::size_t aSize)
	{
		myHandlers.reserve(aSize);
	}
	template<typename... Args>
	inline void Event<Args...>::Clear() noexcept
	{
		myHandlers.clear();
	}

	template<typename... Args>
	inline evnt::IDType Event<Args...>::Add(const HandlerType& aHandler)
	{
		myHandlers.emplace_back(aHandler);
		return aHandler.GetID();
	}
	template<typename... Args>
	inline evnt::IDType Event<Args...>::Add(const typename HandlerType::FuncType& aHandler)
	{
		myHandlers.emplace_back(aHandler);
		return myHandlers.back().GetID();
	}

	template<typename... Args>
	inline bool Event<Args...>::Remove(const HandlerType& aHandler)
	{
		const auto it = std::find(myHandlers.begin(), myHandlers.end(), aHandler);

		if (it == myHandlers.end())
			return false;

		myHandlers.erase(it);

		return true;
	}
	template<typename... Args>
	inline bool Event<Args...>::RemoveID(evnt::IDType aHandlerID)
	{
		const auto it = std::find_if(myHandlers.begin(), myHandlers.end(),
			[aHandlerID](const HandlerType& aHandler)
			{
				return aHandler.GetID() == aHandlerID;
			});

		if (it == myHandlers.end())
			return false;

		myHandlers.erase(it);

		return true;
	}

	template<typename... Args>
	inline void Event<Args...>::Call(Args... someParams) const
	{
		for (const auto& handler : myHandlers)
		{
			handler(someParams...);
		}
	}
	template<typename... Args>
	inline std::future<void> Event<Args...>::CallAsync(Args... someParams) const
	{
		// TODO: this will likely not always work since user may pass arguments that are destroyed when out-of-scope, add some safety to it, or just let user decide

		return std::async(std::launch::async, 
			[this](Args... asyncParams)
			{
				Call(asyncParams...);
			}, someParams...);
	}
}
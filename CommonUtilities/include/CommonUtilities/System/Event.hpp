#pragma once

#include <vector>
#include <future>
#include <shared_mutex>

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
		evnt::IDType operator+=(const typename HandlerType::FunctionType& aHandler);

		evnt::IDType operator-=(const HandlerType& aHandler);
		evnt::IDType operator-=(evnt::IDType aHandlerID) override;

		NODISC std::size_t Count() const override;
		NODISC bool IsEmpty() const override;

		void Reserve(std::size_t aSize) override;
		void Clear() override;

		evnt::IDType Add(const HandlerType& aHandler);
		evnt::IDType Add(const typename HandlerType::FunctionType& aHandler);

		bool Remove(const HandlerType& aHandler);
		bool RemoveID(evnt::IDType aHandlerID) override;

		void Call(Args... someParams) const;
		std::future<void> CallAsync(Args... someParams) const;

	private:
		using HandlerList = std::vector<HandlerType>;

		HandlerList	myHandlers;
		mutable std::shared_mutex myMutex;
	};

	template<typename... Args>
	inline Event<Args...>::Event() = default;

	template<typename... Args>
	inline Event<Args...>::~Event() = default;

	template<typename... Args>
	inline Event<Args...>::Event(const Event& aOther)
	{
		std::shared_lock lock(aOther.myMutex);
		myHandlers = aOther.myHandlers;
	}
	template<typename... Args>
	inline Event<Args...>::Event(Event&& aOther)
	{
		std::scoped_lock lock(aOther.myMutex);
		myHandlers = std::move(aOther.myHandlers);
	}

	template<typename... Args>
	inline auto Event<Args...>::operator=(const Event& aOther) -> Event&
	{
		std::scoped_lock lock1(myMutex);
		std::shared_lock lock2(aOther.myMutex);

		myHandlers = aOther.myHandlers;

		return *this;
	}
	template<typename... Args>
	inline auto Event<Args...>::operator=(Event&& aOther) noexcept -> Event&
	{
		std::scoped_lock lock1(myMutex);
		std::scoped_lock lock2(aOther.myMutex);

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
	inline evnt::IDType Event<Args...>::operator+=(const typename HandlerType::FunctionType& aHandler)
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
	inline std::size_t Event<Args...>::Count() const
	{
		std::shared_lock lock(myMutex);
		return myHandlers.size();
	}
	template<typename... Args>
	inline bool Event<Args...>::IsEmpty() const
	{
		std::shared_lock lock(myMutex);
		return myHandlers.empty();
	}
	template<typename... Args>
	inline void Event<Args...>::Reserve(std::size_t aSize)
	{
		std::scoped_lock lock(myMutex);
		myHandlers.reserve(aSize);
	}
	template<typename... Args>
	inline void Event<Args...>::Clear()
	{
		std::scoped_lock lock(myMutex);
		myHandlers.clear();
	}

	template<typename... Args>
	inline evnt::IDType Event<Args...>::Add(const HandlerType& aHandler)
	{
		std::scoped_lock lock(myMutex);

		myHandlers.emplace_back(aHandler);

		return aHandler.GetID();
	}
	template<typename... Args>
	inline evnt::IDType Event<Args...>::Add(const typename HandlerType::FunctionType& aHandler)
	{
		std::scoped_lock lock(myMutex);

		myHandlers.emplace_back(aHandler);

		return myHandlers.back().GetID();
	}

	template<typename... Args>
	inline bool Event<Args...>::Remove(const HandlerType& aHandler)
	{
		std::scoped_lock lock(myMutex);

		const auto it = std::find(myHandlers.begin(), myHandlers.end(), aHandler);

		if (it == myHandlers.end())
			return false;

		myHandlers.erase(it);

		return true;
	}
	template<typename... Args>
	inline bool Event<Args...>::RemoveID(evnt::IDType aHandlerID)
	{
		std::scoped_lock lock(myMutex);

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
		std::shared_lock lock(myMutex);

		for (const auto& handler : myHandlers)
		{
			handler(someParams...);
		}
	}
	template<typename... Args>
	inline std::future<void> Event<Args...>::CallAsync(Args... someParams) const
	{
		return std::async(std::launch::async, 
			[this](Args... asyncParams)
			{
				std::scoped_lock lock(myMutex);

				for (const auto& handler : myHandlers)
				{
					handler(asyncParams...);
				}
			}, someParams...);
	}
}
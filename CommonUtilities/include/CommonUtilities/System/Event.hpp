#pragma once

#include <vector>
#include <future>
#include <shared_mutex>

#include <CommonUtilities/Config.h>

#include "EventHandler.hpp"
#include "IEvent.h"

namespace CommonUtilities
{
	/// Can hold any number of callbacks (functions) that may all be executed manually. 
	/// 
	/// Based on article by Shmuel Zang:
	/// https://www.codeproject.com/Articles/1256352/CppEvent-How-to-Implement-Events-using-Standard-Cp
	/// 
	/// \param Args: Parameters for function (Specify them how you would normally write parameters, e.g., 
	/// pass by reference and/or as const). However, do not specify as rvalue (&&), since the argument may be moved
	/// early and is then invalid when passed to the other callbacks.
	/// 
	template<typename... Args>
	class Event final : public IEvent
	{
	public:
		using HandlerType = EventHandler<Args...>;

		Event() = default;
		~Event() = default;

		Event(const Event& aOther);
		Event(Event&& aOther);

		auto operator=(const Event& aOther) -> Event&;
		auto operator=(Event&& aOther) noexcept -> Event&;

		explicit operator bool() const;

		void operator()(Args... someParams) const;

		evnt::IDType operator+=(const HandlerType& aHandler);
		evnt::IDType operator+=(const typename HandlerType::FunctionType& aHandler);
		evnt::IDType operator+=(typename HandlerType::FunctionType&& aHandler);

		evnt::IDType operator-=(const HandlerType& aHandler);
		evnt::IDType operator-=(evnt::IDType aHandlerID) override;

		/// \return Number registered callbacks in the event 
		///
		NODISC std::size_t Count() const override;

		/// \return If the event is empty of callbacks
		///
		NODISC bool IsEmpty() const override;

		/// Preallocate memory for the functions to reduce number of reallocations.
		///
		void Reserve(std::size_t aSize) override;

		/// Removes all registered callbacks.
		/// 
		void Clear() override;

		/// Adds the handler to the list of callbacks.
		/// 
		/// \param Handler: Handler that contains the callback
		/// 
		/// \return Unique ID for the added callback so that you can later remove it
		/// 
		evnt::IDType Add(const HandlerType& aHandler);

		/// Adds the function to the list of callbacks.
		/// 
		/// \param Func: Function that will be invoked
		/// 
		/// \return Unique ID for the added callback so that you can later remove it
		/// 
		evnt::IDType Add(const typename HandlerType::FunctionType& aFunc);

		/// Adds the function to the list of callbacks.
		/// 
		/// \param Func: Function that will be invoked
		/// 
		/// \return Unique ID for the added callback so that you can later remove it
		/// 
		evnt::IDType Add(typename HandlerType::FunctionType&& aFunc);

		/// Removes the callback associated with the provided handler.
		/// 
		/// \param Handler: Handler that contains the associated ID
		/// 
		/// \return If removal was successful
		/// 
		bool Remove(const HandlerType& aHandler);

		/// Removes the callback that has the provided ID
		/// 
		/// \param HandlerID: ID of the handler that contain the callback
		/// 
		/// \return If removal was successful
		/// 
		bool RemoveID(evnt::IDType aHandlerID) override;

		/// Invokes the event, which calls all of the added callbacks.
		/// 
		/// \param Args: Optional arguments that is passed along to each callback
		/// 
		void Call(Args... someParams) const;

		/// Invokes the event asynchronously, which calls all of the added callbacks.
		/// 
		/// \param Args: Optional arguments that is passed along to each callback
		/// 
		std::future<void> CallAsync(Args... someParams) const;

	private:
		using HandlerList = std::vector<HandlerType>;

		HandlerList	myHandlers;
		mutable std::shared_mutex myMutex;
	};

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
	inline void Event<Args...>::operator()(Args... someParams) const
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
	template<typename ...Args>
	inline evnt::IDType Event<Args...>::operator+=(typename HandlerType::FunctionType&& aHandler)
	{
		return Add(std::move(aHandler));
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
	inline evnt::IDType Event<Args...>::Add(const typename HandlerType::FunctionType& aFunc)
	{
		std::scoped_lock lock(myMutex);

		myHandlers.emplace_back(aFunc);

		return myHandlers.back().GetID();
	}

	template<typename ...Args>
	inline evnt::IDType Event<Args...>::Add(typename HandlerType::FunctionType&& aFunc)
	{
		std::scoped_lock lock(myMutex);

		myHandlers.emplace_back(std::move(aFunc));

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
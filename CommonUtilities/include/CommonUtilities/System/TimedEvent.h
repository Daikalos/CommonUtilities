#pragma once

#include "Event.hpp"
#include "StopWatch.h"

namespace CommonUtilities
{
	class TimedEvent final
	{
	public:
		using HandlerType = typename Event<>::HandlerType;

		TimedEvent() = default;
		~TimedEvent() = default;

		explicit operator bool() const;

		evnt::IDType operator+=(const HandlerType& aHandler);
		evnt::IDType operator+=(const typename HandlerType::FunctionType& aHandler);
		evnt::IDType operator+=(typename HandlerType::FunctionType&& aHandler);

		evnt::IDType operator-=(const HandlerType& aHandler);
		evnt::IDType operator-=(evnt::IDType aHandlerID);

		NODISC float GetCallTime() const noexcept;

		NODISC float GetElapsed() const noexcept;

		NODISC bool IsRunning() const noexcept;
		
		NODISC bool IsLooping() const noexcept;

		/// \return Number registered callbacks in the event 
		///
		NODISC std::size_t Count() const;

		/// \return If the event is empty of callbacks
		///
		NODISC bool IsEmpty() const;

		/// Preallocate memory for the functions to reduce number of reallocations.
		///
		void Reserve(std::size_t aSize);

		/// Removes all registered callbacks.
		/// 
		void Clear();

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
		bool RemoveID(evnt::IDType aHandlerID);

		void SetCallTime(float aCallTime);

		void SetIsLooping(bool aFlag);

		void Start();

		void Stop();

		void Reset(float aHeadStart = 0.0f);

		void Update(const Timer& aTimer);

	private:
		Event<>		myEvent;
		float		myCallTime	{1.0f};
		StopWatch	myStopWatch;
		bool		myIsLooping	{false};
	};
}
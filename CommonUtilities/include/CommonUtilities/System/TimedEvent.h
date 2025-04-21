#pragma once

#include "Event.hpp"
#include "StopWatch.h"

namespace CommonUtilities
{
	class TimedEvent final : public IEvent
	{
	public:
		using HandlerType = typename Event<>::HandlerType;

		TimedEvent(float aCallTime, bool aIsLooping = false);

		TimedEvent() = default;
		~TimedEvent() = default;

		explicit operator bool() const;

		evnt::IDType operator+=(const HandlerType& aHandler);
		evnt::IDType operator+=(const typename HandlerType::FunctionType& aHandler);
		evnt::IDType operator+=(typename HandlerType::FunctionType&& aHandler);

		evnt::IDType operator-=(const HandlerType& aHandler);
		evnt::IDType operator-=(evnt::IDType aHandlerID) override;

		NODISC float GetCallTime() const noexcept;

		NODISC float GetElapsed() const noexcept;

		NODISC float GetRemaining() const;

		NODISC float GetElapsedRatio() const;

		NODISC float GetRemainingRatio() const;

		NODISC bool IsRunning() const noexcept;
		
		NODISC bool IsLooping() const noexcept;

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

		void SetCallTime(float aCallTime);

		void SetIsLooping(bool aFlag);

		void SetRepeat(bool aFlag);

		void StartNew();

		void Start();

		void Stop();

		void Reset(float aHeadStart = 0.0f);

		void Finish();

		void Update(const Timer& aTimer);

	private:
		void Execute();

		Event<>		myEvent;
		float		myCallTime	{1.0f};
		StopWatch	myStopWatch;
		bool		myIsLooping	{false};
		bool		myRepeat	{false};
	};
}
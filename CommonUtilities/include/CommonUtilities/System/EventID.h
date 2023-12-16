#pragma once

#include "IEvent.h"

namespace CommonUtilities
{
	/// <summary>
	/// Wrapper around event id to better manage events lifetime
	/// </summary>
	class EventID
	{
	public:
		EventID() = default;

		explicit EventID(IEvent& aEvent, evnt::IDType aEventID);
		~EventID();

		EventID(EventID&& aOther) noexcept;
		EventID(const EventID&) = delete;

		EventID& operator=(EventID&& aOther) noexcept;
		EventID& operator=(const EventID&) = delete;

		bool IsConnected() const noexcept;

		bool Connect(IEvent& aEvent, evnt::IDType aEventID);
		bool Disconnect();

	private:
		IEvent*			myEvent	{nullptr};
		evnt::IDType	myID	{0};
	};
}
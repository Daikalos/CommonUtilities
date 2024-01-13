#pragma once

#include "IEvent.h"

#include <CommonUtilities/Utility/NonCopyable.h>

namespace CommonUtilities
{
	/// Wrapper around event id to better manage the events lifetime
	/// 
	class EventID : public NonCopyable
	{
	public:
		EventID() = default;
		~EventID();

		explicit EventID(IEvent& aEvent, evnt::IDType aEventID);

		EventID(EventID&& aOther) noexcept;
		EventID& operator=(EventID&& aOther) noexcept;

		bool IsConnected() const noexcept;

		bool Connect(IEvent& aEvent, evnt::IDType aEventID);
		bool Disconnect();

	private:
		IEvent*			myEvent	{nullptr};
		evnt::IDType	myID	{0};
	};
}
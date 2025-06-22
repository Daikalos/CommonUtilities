#pragma once

#include "IEvent.h"

#include <CommonUtilities/Utility/NonCopyable.h>

namespace CommonUtilities
{
	/// Wrapper around event id to better manage the events lifetime, for example, will remove the callback 
	/// when this is destroyed.
	/// 
	class COMMON_UTILITIES_API EventID
	{
	public:
		EventID() = default;
		~EventID();

		explicit EventID(IEvent& aEvent, evnt::IDType aEventID);

		EventID(const EventID& aOther);
		EventID& operator=(const EventID& aOther);

		EventID(EventID&& aOther) noexcept;
		EventID& operator=(EventID&& aOther) noexcept;

		NODISC bool operator==(const EventID& aRight);
		NODISC bool operator!=(const EventID& aRight);

		bool IsConnected() const;

		bool Connect(IEvent& aEvent, evnt::IDType aEventID);
		bool Disconnect();

	private:
		void SetLifetime();
		bool CheckLifetime() const;

		IEvent*			myEvent	{nullptr};
		evnt::IDType	myID	{0};
		
		std::weak_ptr<void*> myHandle;
	};
}
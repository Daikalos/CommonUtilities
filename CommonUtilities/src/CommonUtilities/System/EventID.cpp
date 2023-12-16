#include <CommonUtilities/System/EventID.h>

using namespace CommonUtilities;

EventID::EventID(IEvent& aEvent, evnt::IDType aEventID)
	: myEvent(&aEvent)
	, myID(aEventID)
{

}

EventID::~EventID()
{
	if (IsConnected())
		myEvent->RemoveID(myID);
}

EventID::EventID(EventID&& aOther) noexcept
	: myEvent(aOther.myEvent), myID(aOther.myID)
{
	aOther.myEvent = nullptr;
	aOther.myID = NULL;
}

EventID& EventID::operator=(EventID&& aOther) noexcept
{
	std::swap(myEvent, aOther.myEvent);
	std::swap(myID, aOther.myID);

	return *this;
}

bool EventID::IsConnected() const noexcept
{
	return myEvent != nullptr && myID != NULL;
}

bool EventID::Connect(IEvent& aEvent, evnt::IDType aEventID)
{
	if (IsConnected())
		return false;

	myEvent = &aEvent;
	myID = aEventID;

	return true;
}

bool EventID::Disconnect()
{
	if (!IsConnected())
		return false;

	bool result = myEvent->RemoveID(myID); // event might already have been manually removed

	myEvent = nullptr; // invalidate
	myID = NULL;

	return result;
}

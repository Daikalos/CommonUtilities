#include <CommonUtilities/System/EventID.h>

#include <utility>

using namespace CommonUtilities;

EventID::~EventID()
{
	if (IsConnected())
	{
		myEvent->RemoveID(myID);
	}
}

EventID::EventID(IEvent& aEvent, evnt::IDType aEventID)
	: myEvent(&aEvent)
	, myID(aEventID)
	, myHandle()
{
	SetLifetime();
}

EventID::EventID(const EventID& aOther)
	: myEvent(aOther.myEvent)
	, myID(aOther.myID)
	, myHandle(aOther.myHandle)
{

}
EventID& EventID::operator=(const EventID& aOther)
{
	if (IsConnected() && *this != aOther)
	{
		Disconnect();
	}

	myEvent		= aOther.myEvent;
	myID		= aOther.myID;
	myHandle	= aOther.myHandle;

	return *this;
}

EventID::EventID(EventID&& aOther) noexcept
	: myEvent(std::exchange(aOther.myEvent, nullptr))
	, myID(std::exchange(aOther.myID, NULL))
	, myHandle(std::move(aOther.myHandle))
{
	aOther.myHandle.reset();
}
EventID& EventID::operator=(EventID&& aOther) noexcept
{
	std::swap(myEvent, aOther.myEvent);
	std::swap(myID, aOther.myID);
	std::swap(myHandle, aOther.myHandle);

	return *this;
}

bool EventID::operator==(const EventID& aRight)
{
	return (myEvent == aRight.myEvent && myID == aRight.myID);
}
bool EventID::operator!=(const EventID& aRight)
{
	return !(*this == aRight);
}

bool EventID::IsConnected() const
{
	return CheckLifetime();
}

bool EventID::Connect(IEvent& aEvent, evnt::IDType aEventID)
{
	if (IsConnected())
		return false;

	myEvent = &aEvent;
	myID = aEventID;

	SetLifetime();

	return true;
}

bool EventID::Disconnect()
{
	if (!IsConnected())
		return false;

	bool result = myEvent->RemoveID(myID); // event might already have been manually removed

	myEvent = nullptr; // invalidate
	myID = NULL;
	myHandle.reset();

	return result;
}

void EventID::SetLifetime()
{
	if (myEvent == nullptr || myID == NULL)
		return;

	if (myEvent->myLifetime == nullptr)
		myEvent->myLifetime = std::make_shared<void*>();

	myHandle = myEvent->myLifetime;
}
bool EventID::CheckLifetime() const
{
	return myEvent != nullptr && myID != NULL && !myHandle.expired();
}

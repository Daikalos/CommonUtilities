#include <CommonUtilities/System/TimedEvent.h>

#include <cmath>

using namespace CommonUtilities;

TimedEvent::TimedEvent(float aCallTime, bool aIsLooping)
	: myEvent()
	, myCallTime(aCallTime)
	, myStopWatch()
	, myIsLooping(aIsLooping)
{

}

TimedEvent::operator bool() const
{
	return static_cast<bool>(myEvent);
}

evnt::IDType TimedEvent::operator+=(const HandlerType& aHandler)
{
	return (myEvent += aHandler);
}
evnt::IDType TimedEvent::operator+=(const typename HandlerType::FunctionType& aHandler)
{
	return (myEvent += aHandler);
}
evnt::IDType TimedEvent::operator+=(typename HandlerType::FunctionType&& aHandler)
{
	return (myEvent += aHandler);
}

evnt::IDType TimedEvent::operator-=(const HandlerType& aHandler)
{
	return (myEvent -= aHandler);
}
evnt::IDType TimedEvent::operator-=(evnt::IDType aHandlerID)
{
	return (myEvent -= aHandlerID);
}

float TimedEvent::GetCallTime() const noexcept
{
	return myCallTime;
}
float TimedEvent::GetElapsed() const noexcept
{
	return myStopWatch.GetElapsed();
}
bool TimedEvent::IsRunning() const noexcept
{
	return myStopWatch.IsRunning();
}
bool TimedEvent::IsLooping() const noexcept
{
	return myIsLooping;
}

std::size_t TimedEvent::Count() const
{
	return myEvent.Count();
}
bool TimedEvent::IsEmpty() const
{
	return myEvent.IsEmpty();
}
void TimedEvent::Reserve(std::size_t aSize)
{
	myEvent.Reserve(aSize);
}
void TimedEvent::Clear()
{
	myEvent.Clear();
}

evnt::IDType TimedEvent::Add(const HandlerType& aHandler)
{
	return myEvent.Add(aHandler);
}
evnt::IDType TimedEvent::Add(const typename HandlerType::FunctionType& aFunc)
{
	return myEvent.Add(aFunc);
}
evnt::IDType TimedEvent::Add(typename HandlerType::FunctionType&& aFunc)
{
	return myEvent.Add(std::move(aFunc));
}

bool TimedEvent::Remove(const HandlerType& aHandler)
{
	return myEvent.Remove(aHandler);
}
bool TimedEvent::RemoveID(evnt::IDType aHandlerID)
{
	return myEvent.RemoveID(aHandlerID);
}

void TimedEvent::SetCallTime(float aCallTime)
{
	myCallTime = aCallTime;
}

void TimedEvent::SetIsLooping(bool aFlag)
{
	myIsLooping = aFlag;
}

void TimedEvent::SetRepeat(bool aFlag)
{
	myRepeat = aFlag;
}

void TimedEvent::Start()
{
	myStopWatch.Start();
}

void TimedEvent::Stop()
{
	myStopWatch.Stop();
}

void TimedEvent::Reset(float aHeadStart)
{
	myStopWatch.Reset(aHeadStart);
}

void TimedEvent::Update(const Timer& aTimer)
{
	if (IsRunning())
	{ 
		myStopWatch.Update(aTimer);
		if (myRepeat)
		{
			while (GetElapsed() >= GetCallTime())
			{
				Execute();
			}
		}
		else
		{
			if (GetElapsed() >= GetCallTime())
			{
				Execute();
			}
		}
	}
}

void TimedEvent::Execute()
{
	myEvent();

	if (IsLooping())
	{
		myStopWatch.Reset(std::fmod(GetElapsed(), GetCallTime()));
		myStopWatch.Start();
	}
	else
	{
		myStopWatch.Reset();
	}
}

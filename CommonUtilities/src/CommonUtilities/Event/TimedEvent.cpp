#include <CommonUtilities/Event/TimedEvent.h>

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
float TimedEvent::GetRemaining() const
{
	return GetCallTime() - GetElapsed();
}
float TimedEvent::GetElapsedRatio() const
{
	return myCallTime ? GetElapsed() / GetCallTime() : 0.0f;
}
float TimedEvent::GetRemainingRatio() const
{
	return myCallTime ? GetRemaining() / GetCallTime() : 0.0f;
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
	myCallTime = (std::max)(aCallTime, 0.0f);
}

void TimedEvent::SetIsLooping(bool aFlag)
{
	myIsLooping = aFlag;
}

void TimedEvent::SetRepeat(bool aFlag)
{
	myRepeat = aFlag;
}

void TimedEvent::StartNew()
{
	myStopWatch.StartNew();
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

void TimedEvent::Finish()
{
	if (IsRunning())
	{
		if (!IsLooping())
		{
			myStopWatch.Reset(myCallTime);
		}
		else
		{
			myStopWatch.StartNew();
		}

		Execute();
	}
}

void TimedEvent::Update(const Timer& aTimer)
{
	if (IsRunning())
	{ 
		myStopWatch.Update(aTimer);

		if (myRepeat)
		{
			while (GetElapsed() >= GetCallTime() && IsRunning())
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
	if (!IsLooping())
	{
		myStopWatch.Reset(myCallTime);
	}

	myEvent();

	if (IsLooping() && IsRunning())
	{
		myStopWatch.Reset(std::fmod(GetElapsed(), GetCallTime()));
		myStopWatch.Start();
	}
}

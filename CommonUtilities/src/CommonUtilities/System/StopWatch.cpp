#include <CommonUtilities/System/StopWatch.h>

#include <CommonUtilities/System/Timer.h>

using namespace CommonUtilities;

namespace chr = std::chrono;

StopWatch::StopWatch()
{
	Start();
}

StopWatch::~StopWatch() = default;

float StopWatch::GetElapsed() const noexcept
{
	return myDuration;
}
float StopWatch::GetRealElapsed() const noexcept
{
	return myRealDuration;
}
bool StopWatch::IsRunning() const noexcept
{
	return myIsRunning;
}

void StopWatch::Start()
{
	myIsRunning = true;
}
void StopWatch::Stop()
{
	myIsRunning = false;
}
void StopWatch::Reset(float aHeadStart)
{
	myDuration		= aHeadStart;
	myRealDuration	= aHeadStart;
	myIsRunning		= false;
}

void StopWatch::Update(const Timer& aTimer)
{
	if (myIsRunning)
	{
		myDuration		+= aTimer.GetDT();
		myRealDuration	+= aTimer.GetRealDT();
	}
}

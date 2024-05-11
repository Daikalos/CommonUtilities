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
	return myElapsed;
}
float StopWatch::GetRealElapsed() const noexcept
{
	return myRealElapsed;
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
	myElapsed		= aHeadStart;
	myRealElapsed	= aHeadStart;
	myIsRunning		= false;
}

void StopWatch::Update(const Timer& aTimer)
{
	if (myIsRunning)
	{
		myElapsed		+= aTimer.GetDT();
		myRealElapsed	+= aTimer.GetRealDT();
	}
}

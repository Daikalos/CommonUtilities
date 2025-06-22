#include <CommonUtilities/Time/Timer.h>

#include <cmath>

using namespace CommonUtilities;

Timer::Timer()
	: myReferencePoint(ClockImpl::now())
	, myDeltaTime()
	, myRealDeltaTime()
	, myFixedDeltaTime(1.0f / (float)ourFixedFPS)
	, myRealFixedDeltaTime(1.0f / (float)ourFixedFPS)
	, myScaledTime(1.0f)
	, myAlpha()
	, myTotalTime()
	, myTotalRunTime()
	, myFixedFPS(ourFixedFPS)
{

}
int Timer::GetFPS() const				{ return (int)std::round(1.0f / GetRealDT()); }
int Timer::GetFixedFPS() const noexcept	{ return myFixedFPS; }

void Timer::SetScaledTime(float aValue)
{
	myScaledTime = aValue;
	myFixedDeltaTime = GetRealFixedDT() * GetScaledTime();
}
void Timer::SetAlpha(float aValue)
{
	myAlpha = aValue;
}

void Timer::Reset()
{
	myReferencePoint = ClockImpl::now();
}

void Timer::Update()
{
	ClockImpl::time_point currentPoint = ClockImpl::now();
	std::chrono::duration<long double> elapsedTime = currentPoint - myReferencePoint;

	long double dt			= (elapsedTime.count() > ourMaxDeltaTime) ? ourMaxDeltaTime : elapsedTime.count();
	long double scaledDT	= dt * static_cast<long double>(myScaledTime);

	myRealDeltaTime	= static_cast<float>(dt);
	myDeltaTime		= static_cast<float>(scaledDT);

	myTotalTime		+= dt;
	myTotalRunTime	+= scaledDT;

	myReferencePoint = currentPoint;
}

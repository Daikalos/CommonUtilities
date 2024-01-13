#include <CommonUtilities/System/Timer.h>

#include <cmath>

using namespace CommonUtilities;

Timer::Timer() : myReferencePoint(ClockImpl::now())
{

}

float Timer::GetDT() const noexcept					{ return myDeltaTime; }
float Timer::GetRealDT() const noexcept				{ return myRealDeltaTime; }
float Timer::GetFixedDT() const noexcept			{ return myFixedDeltaTime; }
float Timer::GetRealFixedDT() const noexcept		{ return myRealFixedDeltaTime; }
float Timer::GetScaledTime() const noexcept			{ return myScaledTime; }
float Timer::GetAlpha() const noexcept				{ return myAlpha; }
long double Timer::GetTotalTime() const noexcept	{ return myTotalTime; }
long double Timer::GetTotalRunTime() const noexcept { return myTotalRunTime; }
int Timer::GetFPS() const							{ return (int)std::round(1.0f / GetRealDT()); }
int Timer::GetFixedFPS() const						{ return myFixedFPS; }

void Timer::SetScaledTime(float aValue) noexcept
{
	myScaledTime = aValue;
	myFixedDeltaTime = GetRealFixedDT() * GetScaledTime();
}
void Timer::SetAlpha(float aValue) noexcept
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

#pragma once

#include <CommonUtilities/Utility/NonCopyable.h>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	class Timer;

	class COMMON_UTILITIES_API StopWatch
	{
	public:
		StopWatch();
		~StopWatch();

		NODISC float GetElapsed() const noexcept;
		NODISC float GetRealElapsed() const noexcept;
		NODISC bool IsRunning() const noexcept;

		void Start();
		void Stop();
		void Reset(float aHeadStart = 0.0f);

		void Update(const Timer& aTimer);

	private:
		float	myDuration		= 0.0;
		float	myRealDuration	= 0.0;
		bool	myIsRunning		= false;
	};
}
#pragma once

#include <CommonUtilities/Utility/NonCopyable.h>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	class Timer;

	class COMMON_UTILITIES_API StopWatch
	{
	public:
		StopWatch(bool aStartImmediately = false);
		~StopWatch();

		NODISC float GetElapsed() const noexcept;
		NODISC float GetRealElapsed() const noexcept;
		NODISC bool IsRunning() const noexcept;

		void Start();
		void StartNew();
		void Stop();
		void Reset(float aHeadStart = 0.0f);

		void Update(const Timer& aTimer);

	private:
		float	myElapsed		= 0.0;
		float	myRealElapsed	= 0.0;
		bool	myIsRunning		= false;
	};
}
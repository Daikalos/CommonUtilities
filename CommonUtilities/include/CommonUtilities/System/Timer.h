#pragma once

#include <chrono>
#include <type_traits>

#include <CommonUtilities/Utility/NonCopyable.h>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	class COMMON_UTILITIES_API Timer : private NonCopyable
	{
	public:
		Timer();
		~Timer();

		NODISC float GetDT() const noexcept;
		NODISC float GetRealDT() const noexcept;
		NODISC float GetFixedDT() const noexcept;
		NODISC float GetRealFixedDT() const noexcept;
		NODISC float GetScaledTime() const noexcept;
		NODISC float GetAlpha() const noexcept;
		NODISC long double GetTotalTime() const noexcept;
		NODISC long double GetTotalRunTime() const noexcept;
		NODISC int GetFPS() const noexcept;
		NODISC int GetFixedFPS() const noexcept;

		void SetScaledTime(float value) noexcept;
		void SetAlpha(float value) noexcept;

		void Reset();
		void Update();

	private:
		static constexpr long double ourMaxDeltaTime = 0.075;
		using ClockImpl = std::chrono::high_resolution_clock;

		ClockImpl::time_point myReferencePoint;

		float		myDeltaTime				{0.0f};			// total time it took for previous loop
		float		myRealDeltaTime			{0.0f};
		float		myFixedDeltaTime		{1.0f / 60.0f};	// fixed delta time for physics etc.
		float		myRealFixedDeltaTime	{0.0f};
		float		myScaledTime			{1.0f};			// scaled time (set to 1 as default)
		float		myAlpha					{0.0f};

		long double	myTotalTime				{0.0};	// total time in seconds the applicaton has ran
		long double	myTotalRunTime			{0.0};	// total time the application has ran factoring in scaled time

		int			myFPS					{0};
		int			myFixedFPS				{60};
	};
}
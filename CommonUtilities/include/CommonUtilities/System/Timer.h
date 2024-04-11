#pragma once

#include <chrono>
#include <type_traits>

#include <CommonUtilities/Utility/NonCopyable.h>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	class COMMON_UTILITIES_API Timer : public NonCopyable
	{
	public:
		Timer();
		~Timer() = default;

		Timer(Timer&&) = default;
		Timer& operator=(Timer&&) = default;

		NODISC float GetDT() const noexcept;
		NODISC float GetRealDT() const noexcept;
		NODISC float GetFixedDT() const noexcept;
		NODISC float GetRealFixedDT() const noexcept;
		NODISC float GetScaledTime() const noexcept;
		NODISC float GetAlpha() const noexcept;
		NODISC long double GetTotalTime() const noexcept;
		NODISC long double GetTotalRunTime() const noexcept;

		NODISC int GetFPS() const;
		NODISC int GetFixedFPS() const noexcept;

		void SetScaledTime(float aValue) noexcept;
		void SetAlpha(float aValue) noexcept;

		void Reset();
		void Update();

	private:
		static constexpr long double	ourMaxDeltaTime	= 0.075;
		static constexpr int			ourFixedFPS		= 60;

		using ClockImpl = std::chrono::high_resolution_clock;

		typename ClockImpl::time_point myReferencePoint;

		float		myDeltaTime				{0.0f};	// total time it took for previous loop
		float		myRealDeltaTime			{0.0f};
		float		myFixedDeltaTime		{0.0f};	// fixed delta time for physics etc.
		float		myRealFixedDeltaTime	{0.0f};
		float		myScaledTime			{0.0f};	// scaled time (set to 1 as default)
		float		myAlpha					{0.0f};

		long double	myTotalTime				{0.0};	// total time in seconds the timer started in regards to scaled time
		long double	myTotalRunTime			{0.0};	// total time the timer has ran since it started

		int			myFixedFPS				{60};
	};
}
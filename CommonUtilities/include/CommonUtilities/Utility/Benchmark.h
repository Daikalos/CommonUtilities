#pragma once

#include <memory>
#include <functional>
#include <chrono>
#include <iostream>
#include <thread>
#include <queue>
#include <shared_mutex>
#include <mutex>
#include <string>
#include <iomanip>

#include <Windows.h>
#include <Psapi.h>
#include <Pdh.h>

#include <CommonUtilities/Utility/StringUtils.h>
#include <CommonUtilities/Utility/ArithmeticUtils.hpp>

namespace CommonUtilities::bm
{
	COMMON_UTILITIES_API void Begin(std::string aMessage = "BENCHMARK PROFILE");

	COMMON_UTILITIES_API void End();

	template<std::size_t N = 1, class Func, typename... Args>
	void Run(const Func& aFunc, Args&&... someArgs)
	{
		Begin();

		for (std::size_t i = 0; i < N; ++i)
		{
			aFunc(std::forward<Args>(someArgs)...);
		}

		End();
	}

	namespace priv
	{
		constexpr static std::size_t Kilobyte(std::size_t aBytes) { return aBytes / 1024LLU; }
		constexpr static std::size_t Megabyte(std::size_t aBytes) { return Kilobyte(aBytes) / 1024LLU; }
		constexpr static std::size_t Gigabyte(std::size_t aBytes) { return Megabyte(aBytes) / 1024LLU; }

		class Benchmark
		{
		public:
			using Ptr = std::unique_ptr<Benchmark>;

			Benchmark(std::string message);
			~Benchmark();

			void Start();
			void Loop();
			long double GetCPU();

		private:
			std::string			myMessage;
			std::atomic<bool>	myActive;
			std::atomic<bool>	myIsReady;
			std::jthread		myThread;

			ULARGE_INTEGER	myLastCPU;
			ULARGE_INTEGER	myLastSysCPU;
			ULARGE_INTEGER	myLastUserCPU;
			int				myNumProcessors;

			friend void CommonUtilities::bm::Begin(std::string aMessage);
		};

		static std::queue<Benchmark>	globalBenchmarks;
		static std::shared_mutex		globalMutex;
	}
}
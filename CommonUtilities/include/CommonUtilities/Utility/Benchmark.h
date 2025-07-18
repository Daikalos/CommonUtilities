#pragma once

#include <memory>
#include <thread>
#include <queue>
#include <shared_mutex>
#include <mutex>
#include <string>

#include <CommonUtilities/Utility/StringUtils.h>
#include <CommonUtilities/Utility/ArithmeticUtils.hpp>

namespace CommonUtilities::bm
{
	COMMON_UTILITIES_API void Begin(std::string aMessage = "BENCHMARK PROFILE");

	COMMON_UTILITIES_API void End();

	template<std::size_t N = 1, class Func, typename... Args>
	void Run(const Func& aFunc, const Args&... someArgs)
	{
		Begin();

		for (std::size_t i = 0; i < N; ++i)
		{
			aFunc(someArgs...);
		}

		End();
	}

	namespace priv
	{
		constexpr static std::size_t Kilobyte(std::size_t aBytes) { return aBytes / 1024LLU; }
		constexpr static std::size_t Megabyte(std::size_t aBytes) { return Kilobyte(aBytes) / 1024LLU; }
		constexpr static std::size_t Gigabyte(std::size_t aBytes) { return Megabyte(aBytes) / 1024LLU; }

		class COMMON_UTILITIES_API Benchmark
		{
		public:
			using Ptr = std::unique_ptr<Benchmark>;

			Benchmark(std::string message);
			~Benchmark();

			void Start();

		private:
			void Loop();

			std::string			myMessage;
			std::atomic<bool>	myActive;
			std::atomic<bool>	myIsReady;
			std::jthread		myThread;

			friend void CommonUtilities::bm::Begin(std::string aMessage);
		};

		static std::queue<Benchmark>	globalBenchmarks;
		static std::shared_mutex		globalMutex;
	}
}
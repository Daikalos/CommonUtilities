#include <CommonUtilities/Utility/Benchmark.h>

#include <iostream>
#include <iomanip>
#include <functional>
#include <chrono>

#include <CommonUtilities/Utility/Win32Utils.h>

namespace CommonUtilities::bm
{
	void Begin(std::string aMessage)
	{
		std::lock_guard lock(priv::globalMutex);

		auto& benchmark = priv::globalBenchmarks.emplace(std::move(aMessage));
		benchmark.Start();

		while (!benchmark.myIsReady.load());
	}

	void End()
	{
		std::lock_guard lock(priv::globalMutex);

		if (priv::globalBenchmarks.empty())
			throw std::runtime_error("No benchmark to pop");

		priv::globalBenchmarks.pop();
	}

	namespace priv
	{
		static std::mutex locMutex;

		Benchmark::Benchmark(std::string message)
			: myMessage(std::move(message))
			, myActive(false)
			, myIsReady(false)
			, myThread()
		{

		}

		Benchmark::~Benchmark()
		{
			myActive = false;
		}

		void Benchmark::Start()
		{
			myActive = true;
			myThread = std::jthread(&Benchmark::Loop, this);
		}

		void Benchmark::Loop()
		{
			std::size_t totalRAMUsage	= 0;
			long double totalCPUUsage	= 0.0L;

			std::size_t ramAverage		= 0;
			std::size_t ramUsage		= 0;
			std::size_t ramCurr			= 0;
			std::size_t ramDiff			= 0;

			long double cpuAverage		= 0.0L;
			long double cpuUsage		= 0.0L;

			std::size_t totalRAMSamples = 0;
			std::size_t totalCPUSamples = 0;

			const std::size_t initialRAM = Get_RAM_Usage();
			const long double initialCPU = Get_CPU_Usage();

			std::chrono::high_resolution_clock::time_point timeBegin = std::chrono::high_resolution_clock::now();

			myIsReady = true;

			while (myActive.load())
			{
				const std::size_t ram = Get_RAM_Usage();
				const long double cpu = Get_CPU_Usage();

				totalRAMUsage += ram;
				totalCPUUsage += cpu;

				if (ram > 0) ++totalRAMSamples;
				if (cpu > LDBL_EPSILON) ++totalCPUSamples;

				std::this_thread::yield();
			}

			std::chrono::high_resolution_clock::time_point timeEnd = std::chrono::high_resolution_clock::now();

			auto nanoseconds	= std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeBegin).count();
			auto milliseconds	= std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeBegin).count();
			auto seconds		= std::chrono::duration_cast<std::chrono::seconds>(timeEnd - timeBegin).count();

			ramAverage = (totalRAMSamples != 0) ? (totalRAMUsage / totalRAMSamples) : 0;
			ramUsage = ramAverage - initialRAM;

			cpuAverage = (totalCPUSamples != 0) ? (totalCPUUsage / totalCPUSamples) : 0.0L;
			cpuUsage = cpuAverage - initialCPU;

			ramCurr = Get_RAM_Usage();
			ramDiff = ramCurr - initialRAM;

			// -- print information --

			const auto print = [](auto aValue, int aPlaces)
			{
				return cu::RemoveTrailingZeroes(std::to_string(cu::au::SetPrecision(aValue, aPlaces)));
			};

			{
				std::lock_guard<std::mutex> lock(locMutex); // sync console output

				std::puts(myMessage.data());
				std::puts("");

				std::puts("--- BENCHMARK BEGIN ---");
				std::puts("");
				std::puts(std::string("INITIAL RAM:	" + print(Megabyte(initialRAM), 2) + " MB | " + print(Kilobyte(initialRAM), 2) + " KB | " + print(initialRAM, 2) + " Bytes").c_str());
				std::puts(std::string("INITIAL CPU:	" + print(initialCPU, 2) + "%").c_str());
				std::puts("");

				std::puts("--- BENCHMARK END ---");
				std::puts("");
				std::puts(std::string("RAM TOTAL:	" + print(Megabyte(ramAverage), 2) + " MB | " + print(Kilobyte(ramAverage), 2) + " KB | " + print(ramAverage, 2) + " Bytes").c_str());
				std::puts(std::string("RAM USED:	" + print(Megabyte(ramUsage), 2) + " MB | " + print(Kilobyte(ramUsage), 2) + " KB | " + print(ramUsage, 2) + " Bytes").c_str());
				std::puts(std::string("RAM CURR:	" + print(Megabyte(ramCurr), 2) + " MB | " + print(Kilobyte(ramCurr), 2) + " KB | " + print(ramCurr, 2) + " Bytes").c_str());
				std::puts(std::string("RAM DIFF:	" + print(Megabyte(ramDiff), 2) + " MB | " + print(Kilobyte(ramDiff), 2) + " KB | " + print(ramDiff, 2) + " Bytes").c_str());
				std::puts(std::string("CPU TOTAL:	" + print(cpuAverage, 2) + "%").c_str());
				std::puts(std::string("CPU USED:	" + print(cpuUsage, 2) + "%").c_str());
				std::puts("");

				std::puts("TOTAL RUN TIME");
				std::cout << nanoseconds << "ns\n";
				std::cout << milliseconds << "ms\n";
				std::cout << seconds << "s\n";
				std::puts("");

				std::puts("---------------------");
			}
		}
	}
}
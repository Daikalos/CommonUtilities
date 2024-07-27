#pragma once

#include <thread>
#include <functional>
#include <mutex>
#include <type_traits>
#include <array>

#include <CommonUtilities/Structures/FreeVector.hpp>
#include <CommonUtilities/Utility/NonCopyable.h>

namespace CommonUtilities
{
    using LoopID = std::size_t;

	class ThreadLoops
	{
    public:
        ThreadLoops();
        ~ThreadLoops();

        void Start(std::size_t aThreadCount);
        void Shutdown();

        LoopID SetLoopTask(const std::function<void()>& aTask);

        void RemoveLoopTask(LoopID aLoopID);

        void DispatchLoop(LoopID aLoopID);

    private:
        void ThreadLoop(LoopID aLoopID);

        std::vector<std::jthread>           myThreads;
        FreeVector<std::function<void()>>   myLoopTasks;
        std::vector<bool>                   myDispatchedLoops;
        std::condition_variable             myCV;
        std::mutex                          myMutex; // sync access to task queue
        bool                                myShutdown;
	};
}
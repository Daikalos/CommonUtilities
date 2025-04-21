#pragma once

#include <thread>
#include <functional>
#include <mutex>
#include <type_traits>
#include <array>
#include <queue>

#include <CommonUtilities/Structures/FreeVector.hpp>
#include <CommonUtilities/Utility/NonCopyable.h>

namespace CommonUtilities
{
    using LoopID = std::size_t;

	class ThreadLoops
	{
    public:
        using ExceptionCallback = std::function<void(std::exception&)>;

        struct ThreadException
        {
            void*               thread;
            std::exception_ptr  exceptionPtr;
        };

        ThreadLoops();
        ~ThreadLoops();

        void Start(std::size_t aThreadCount);
        void Shutdown();

        ThreadException GetLastException();

        LoopID SetLoopTask(const std::function<void()>& aTask, const ExceptionCallback& aOnException = {});
        void RemoveLoopTask(LoopID aLoopID);
        void DispatchLoop(LoopID aLoopID);

    private:
        struct LoopTask
        {
            std::function<void()>   callback;
            ExceptionCallback       exceptionCallback;
        };

        void ThreadLoop(LoopID aLoopID);

        std::vector<std::jthread>       myThreads;
        FreeVector<LoopTask>            myLoopTasks;
        std::vector<bool>               myDispatchedLoops;
        std::queue<ThreadException>     myExceptions;
        std::condition_variable         myCV;
        std::mutex                      myMutex; // sync access to task queue
        std::mutex                      myExceptionMutex;
        bool                            myShutdown {true};
	};
}
#include <CommonUtilities/System/ThreadPool.h>

using namespace CommonUtilities;

ThreadPool::ThreadPool(std::size_t aThreadCount) : myShutdown(false)
{
    myThreads.reserve(aThreadCount);
    for (std::size_t i = 0; i < aThreadCount; ++i)
    {
        myThreads.emplace_back(&ThreadPool::ThreadLoop, this);
    }
}
ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(myMutex);
        myShutdown = true;
    }
    myCV.notify_all();
}

void ThreadPool::ThreadLoop()
{
    while (true)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(myMutex);

            myCV.wait(lock, [this]
            {
                return !myTasks.empty() || myShutdown;
            });

            if (myTasks.empty() && myShutdown)
                break;

            task = std::move(myTasks.front());
            myTasks.pop();
        }

        task();
    }
}

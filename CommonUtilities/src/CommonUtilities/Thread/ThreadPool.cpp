#include <CommonUtilities/Thread/ThreadPool.h>

#include <CommonUtilities/System/WindowsHeader.h>

using namespace CommonUtilities;

ThreadPool::ThreadPool() = default;

ThreadPool::~ThreadPool()
{
    Shutdown();
}

void ThreadPool::Start(std::size_t aThreadCount)
{
    if (!myShutdown || !myThreads.empty())
        return;

    myShutdown = false;

    myThreads.reserve(aThreadCount);
    for (std::size_t i = 0; i < aThreadCount; ++i)
    {
        myThreads.emplace_back(&ThreadPool::ThreadLoop, this);
    }
}

void ThreadPool::Shutdown()
{
    if (myShutdown && myThreads.empty())
        return;

    {
        std::lock_guard<std::mutex> lock(myMutex);
        myShutdown = true;
    }

    myCV.notify_all();
    
    myThreads.clear();
}

void ThreadPool::ThreadLoop()
{
    while (true)
    {
        Task task;
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

            std::wstring wName = std::wstring(task.name.begin(), task.name.end());
            SetThreadDescription(GetCurrentThread(), PCWSTR(wName.c_str()));
        }

        task.func();
    }
}

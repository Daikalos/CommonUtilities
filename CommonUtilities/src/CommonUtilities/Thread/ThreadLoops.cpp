#include <CommonUtilities/Thread/ThreadLoops.h>

#include <CommonUtilities/System/WindowsHeader.h>

using namespace CommonUtilities;

ThreadLoops::ThreadLoops() : myShutdown(true)
{

}

ThreadLoops::~ThreadLoops()
{
    Shutdown();
}

void ThreadLoops::Start(std::size_t aThreadCount)
{
    if (!myShutdown || !myThreads.empty())
        return;

    myShutdown = false;

    myThreads.reserve(aThreadCount);
    myDispatchedLoops.resize(aThreadCount);

    for (std::size_t i = 0; i < aThreadCount; ++i)
    {
        myThreads.emplace_back(&ThreadLoops::ThreadLoop, this, i);
    }
}
void ThreadLoops::Shutdown()
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

ThreadLoops::ThreadException ThreadLoops::GetLastException()
{
    std::scoped_lock lock(myExceptionMutex);

    ThreadException e;
    if (!myExceptions.empty())
    {
        e = myExceptions.front();
        myExceptions.pop();
    }

    return e;
}

LoopID ThreadLoops::SetLoopTask(const std::function<void()>& aTask, const ExceptionCallback& aOnException)
{
    std::lock_guard lock(myMutex);

    if (myLoopTasks.count() >= myThreads.size())
    {
        throw std::runtime_error("More tasks than threads!");
    }

	return myLoopTasks.emplace(aTask, aOnException);
}

void ThreadLoops::RemoveLoopTask(LoopID aLoopID)
{
    std::lock_guard lock(myMutex);
    myLoopTasks.erase(aLoopID);
}

void ThreadLoops::DispatchLoop(LoopID aLoopID)
{
    {
        std::lock_guard lock(myMutex);
        myDispatchedLoops[aLoopID] = true;
    }
    myCV.notify_all();
}

void ThreadLoops::ThreadLoop(LoopID aLoopID)
{
    while (true)
    {
        {
            std::unique_lock<std::mutex> lock(myMutex);

            myCV.wait(lock, [this, &aLoopID]
            {
                return myDispatchedLoops[aLoopID] || myShutdown;
            });

            if (myShutdown)
                break;

            myDispatchedLoops[aLoopID] = false;
        }

        auto& loopTask = myLoopTasks[aLoopID];

        if (!loopTask.callback)
            continue;

        try
        {
            loopTask.callback();
        }
        catch (std::exception& e)
        {
            {
                std::scoped_lock lock(myExceptionMutex);
                myExceptions.emplace(GetCurrentThread(), std::current_exception());
            }

            if (loopTask.exceptionCallback)
            {
                loopTask.exceptionCallback(e);
            }
        }
    }
}
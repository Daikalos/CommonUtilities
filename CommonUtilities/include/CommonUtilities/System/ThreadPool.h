#pragma once

#include <future>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <type_traits>

#include <CommonUtilities/Utility/NonCopyable.h>

namespace CommonUtilities
{
    class ThreadPool : private NonCopyable
    {
    public:
        ThreadPool();
        ~ThreadPool();

        void Start(std::size_t aThreadCount);
        void Shutdown();

        template<class F, typename... Args> requires(std::is_invocable_v<F, Args...>)
        auto Enqueue(F&& aFunc, std::string&& aThreadName = "CU THREAD", Args&&... someArgs) -> std::future<std::invoke_result_t<F, Args...>>;

    private:
        struct Task
        {
            std::string             name;
            std::function<void()>   func;
        };

        void ThreadLoop();

        std::vector<std::jthread>   myThreads;
        std::queue<Task>            myTasks;
        std::condition_variable     myCV;
        std::mutex                  myMutex; // sync access to task queue
        bool                        myShutdown = true;
    };

    template<class F, typename... Args> requires(std::is_invocable_v<F, Args...>)
    inline auto ThreadPool::Enqueue(F&& aFunc, std::string&& aThreadName, Args&&... someArgs) -> std::future<std::invoke_result_t<F, Args...>>
    {
        using ReturnType = std::invoke_result_t<F, Args...>;

        auto task = std::make_shared<std::packaged_task<ReturnType()>>
        (
            std::bind(std::forward<F>(aFunc), std::forward<Args>(someArgs)...)
        );

        std::future<ReturnType> result = task->get_future();
        {
            std::lock_guard<std::mutex> lock(myMutex); // lock to synchronize access to tasks

            if (myShutdown)
            {
                throw std::runtime_error("Thread pool has shut down, no more tasks can be added");
            }

            // place task in queue, copying the shared pointer keeps it alive
            myTasks.emplace(std::move(aThreadName), [task] { (*task)(); });
        }

        myCV.notify_one(); // notify a thread that a task is available

        return result;
    }
}


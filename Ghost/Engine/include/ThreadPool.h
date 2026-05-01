#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include <future>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>
#include <functional>
#include <vector>
#include "Ghost.h"
#include "SingleTon.h"

namespace ghost
{
    class GHOST_API ThreadPool : public SingleTon<ThreadPool>
    {
    public:
        ThreadPool() = default;
        ~ThreadPool();
        
        void InitThreadPool(int numThreads);
        
        template <typename F, typename... Args>
        auto SubmitTask(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>>
        {
            using returnType = std::invoke_result_t<F, Args...>;
            auto packedTask = std::make_shared<std::packaged_task<returnType()>>(
                [func = std::forward<F>(f), ...args = std::forward<Args>(args)]() mutable
                {
                    return std::invoke(func, std::move(args)...);
                });
            
            std::future<returnType> task = packedTask->get_future();
            {
                std::lock_guard<std::mutex> Lock(_mutex);
                
                if (!_running)
                {
                    throw std::runtime_error("Can't submit task to a stopped thread pool.");
                }
                
                _tasks.emplace([packedTask]()
                {
                    (*packedTask)();
                });
            }

            _cond.notify_one();
            return task;
        }
        
    private:
        int _numThreads = 0;
        bool _running = false;
        bool _initialized = false;
        
        std::vector<std::thread> _workers;
        std::mutex _mutex;
        std::condition_variable _cond;
        std::queue<std::function<void()>> _tasks;
    };
}

#endif
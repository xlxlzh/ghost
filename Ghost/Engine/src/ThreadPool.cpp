#include "ThreadPool.h"

namespace ghost
{
    ThreadPool::ThreadPool(int numThreads) : _numThreads(numThreads), _running(true)
    {
        for (int i = 0; i < numThreads; ++i)
        {
            _workers.emplace_back(
                [this]()
            {
                while (true)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(_mutex);
                        this->_cond.wait(lock, [this]
                        {
                            return !this->_running || !this->_tasks.empty();
                        });
                        
                        if (!this->_running && this->_tasks.empty())
                        {
                            return;
                        }
                        
                        task = std::move(this->_tasks.front());
                        this->_tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ThreadPool::~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _running = false;
        }
        
        _cond.notify_all();
        for (std::thread& worker : _workers)
        {
            worker.join();
        }
    }
}

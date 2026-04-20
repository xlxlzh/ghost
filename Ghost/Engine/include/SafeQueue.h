#include <queue>
#include <mutex>
#include "Ghost.h"

namespace ghost
{
    template <typename T>
    class GHOST_API SafeQueue
    {
    public:
        SafeQueue() = default;
        ~SafeQueue() = default;
        
        bool empty() const
        {
            std::unique_lock<std::mutex> lock(_mutex);
            return _queue.empty();
        }
        
        int size() const
        {
            std::unique_lock<std::mutex> lock(_mutex);
            return _queue.size();
        }
        
        void enqueue(T& item)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _queue.emplace(item);
        }
        
        bool dequeue(T& item)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            if (_queue.empty()) return false;
            
            item = std::move(_queue.front());
            _queue.pop();
            
            return true;
        }
        
    private:
        std::queue<T> _queue;
        std::mutex _mutex;
    };
}

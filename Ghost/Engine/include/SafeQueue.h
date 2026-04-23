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
        
        bool Empty() const
        {
            std::unique_lock<std::mutex> Lock(_mutex);
            return _queue.empty();
        }
        
        int Size() const
        {
            std::unique_lock<std::mutex> Lock(_mutex);
            return _queue.size();
        }
        
        void Enqueue(T& item)
        {
            std::unique_lock<std::mutex> Lock(_mutex);
            _queue.emplace(item);
        }
        
        bool Dequeue(T& item)
        {
            std::unique_lock<std::mutex> Lock(_mutex);
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

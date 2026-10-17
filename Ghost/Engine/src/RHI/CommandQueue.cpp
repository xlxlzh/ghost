#include "CommandQueue.h"

namespace ghost
{
    CommandQueue::CommandQueue()
    {
        
    }

    CommandQueue::CommandQueue(CommandQueueType type): _commandQueueType(type)
    {
        
    }

    CommandQueue::~CommandQueue()
    {
        
    }
    
    CommandQueueType CommandQueue::GetType() const
    {
        return _commandQueueType;
    }
}

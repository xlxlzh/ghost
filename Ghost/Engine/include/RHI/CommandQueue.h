#ifndef _COMMANDQUEUE_H_
#define _COMMANDQUEUE_H_

#include "Ghost.h"

enum class CommandQueueType
{
    CommandQueue_Graphics,
    CommandQueue_Compute,
    CommandQueue_Copy
};

namespace ghost
{
    class GHOST_API CommandQueue
    {
    public:
        CommandQueue();
        CommandQueue(CommandQueueType type);
        
        virtual ~CommandQueue();
        
        CommandQueueType GetType() const;
        
    protected:
        CommandQueueType _commandQueueType {CommandQueueType::CommandQueue_Graphics};
    };
}

#endif

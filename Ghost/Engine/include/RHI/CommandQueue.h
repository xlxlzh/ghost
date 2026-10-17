#ifndef _COMMANDQUEUE_H_
#define _COMMANDQUEUE_H_

#include "Ghost.h"
#include "RenderDevice.h"
#include "Fence.h"

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
        
        virtual bool CreateCommandQueue(const RenderDevicePtr& device) { return true; }
        
    protected:
        CommandQueueType _commandQueueType {CommandQueueType::CommandQueue_Graphics};
        
        FencePtr _fence;
    };
}

#endif

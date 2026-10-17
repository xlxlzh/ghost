#ifndef _D3D12_COMMANDQUEUE_H_
#define _D3D12_COMMANDQUEUE_H_

#include "CommandQueue.h"
#include "D3D12ComPtr.h"
#include "Ghost.h"

namespace ghost
{
    class GHOST_API D3D12CommandQueue : public CommandQueue
    {
    public:
        D3D12CommandQueue(CommandQueueType commandType);
        virtual ~D3D12CommandQueue();
        
        virtual bool CreateCommandQueue(const RenderDevicePtr& device) override;
        
    protected:
        ID3D12CommandQueuePtr _commandQueue;
    };
    
    DECLAR_SMART_POINTER(D3D12CommandQueue);
}

#endif

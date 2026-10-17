#include "D3D12CommandQueue.h"
#include "D3D12RenderDevice.h"

namespace ghost
{
    D3D12_COMMAND_LIST_TYPE GetD3D12CommandQueueType(CommandQueueType commandType)
    {
        switch (commandType)
        {
        case CommandQueueType::CommandQueue_Graphics:
            return D3D12_COMMAND_LIST_TYPE_DIRECT;
        case CommandQueueType::CommandQueue_Compute:
            return D3D12_COMMAND_LIST_TYPE_COMPUTE;
        case CommandQueueType::CommandQueue_Copy:
            return D3D12_COMMAND_LIST_TYPE_COPY;
        }
        
        return D3D12_COMMAND_LIST_TYPE_DIRECT;
    }
    
    D3D12CommandQueue::D3D12CommandQueue(CommandQueueType commandType) : CommandQueue(commandType)
    {
        
    }

    D3D12CommandQueue::~D3D12CommandQueue()
    {
        
    }
    
    bool D3D12CommandQueue::CreateCommandQueue(const RenderDevicePtr& device)
    {
        D3D12RenderDevicePtr d3d12RenderDevice = GHOST_SMARTPOINTER_CAST(D3D12RenderDevice, device);
        ID3D12DevicePtr d3d12Device = d3d12RenderDevice->GetDevice();
        
        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = GetD3D12CommandQueueType(_commandQueueType);
        desc.NodeMask = 1;
        
        HRESULT hr = d3d12Device->CreateCommandQueue(&desc, IID_PPV_ARGS(_commandQueue.GetAddressOf()));

        return SUCCEEDED(hr);
    }
}

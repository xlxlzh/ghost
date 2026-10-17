#include "D3D12Fence.h"

namespace ghost
{
    D3D12Fence::D3D12Fence()
    {
        
    }

    D3D12Fence::~D3D12Fence()
    {
        
    }
    
    bool D3D12Fence::Create(const RenderDevicePtr& device)
    {
        D3D12RenderDevicePtr d3d12RenderDevice = GHOST_SMARTPOINTER_CAST(D3D12RenderDevice, device);
        ID3D12DevicePtr d3d12Device = d3d12RenderDevice->GetDevice();
        
        HRESULT hr = d3d12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(_fence.GetAddressOf()));
        _fenceEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);
        
        return SUCCEEDED(hr) && _fenceEvent != nullptr;
    }
}

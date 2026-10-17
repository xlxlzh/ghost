#ifndef _D3D12_FENCE_H_
#define _D3D12_FENCE_H_
#include "Fence.h"
#include "D3D12ComPtr.h"
#include "D3D12RenderDevice.h"

namespace ghost
{
    class GHOST_API D3D12Fence : public Fence
    {
    public:
        D3D12Fence();
        virtual ~D3D12Fence();
        
        virtual bool Create(const RenderDevicePtr& device) override;
        
    private:
        ID3D12FencePtr _fence;
        HANDLE _fenceEvent;
    };
}

#endif

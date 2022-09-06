#ifndef _D3D12_RENDERDEVICE_H_
#define _D3D12_RENDERDEVICE_H_

#include "RenderDevice.h"
#include "D3D12ComPtr.h"

namespace ghost
{
    class GHOST_API D3D12RenderDevice : public RenderDevice
    {
        friend class D3D12RenderSystem;
    public:
        virtual bool initDevice(bool fullscreen, unsigned msaaCount) override;
        virtual bool checkSampleCount(unsigned checkCount) override;

    private:
        bool _initSwapchain();

    private:
        ID3D12DevicePtr _device;

        ID3D12CommandQueuePtr        _commandQueue;
        ID3D12GraphicsCommandListPtr _graphicsCommandList;
        ID3D12CommandAllocatorPtr    _commandAllocator;

        ID3D12FencePtr               _fence;
        unsigned long long           _currentFence{ 0 };

        unsigned                     _rtvDescriptorSize{ 0 };
        unsigned                     _dsvDescriptorSize{ 0 };
        unsigned                     _cbvAndsrvDescriptorSize{ 0 };
    };

    DECLAR_SMART_POINTER(D3D12RenderDevice)
}

#endif
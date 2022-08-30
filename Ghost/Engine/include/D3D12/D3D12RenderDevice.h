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
    };

    DECLAR_SMART_POINTER(D3D12RenderDevice)
}

#endif
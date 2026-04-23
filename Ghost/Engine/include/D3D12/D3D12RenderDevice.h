#ifndef _D3D12_RENDERDEVICE_H_
#define _D3D12_RENDERDEVICE_H_

#include "RenderDevice.h"
#include "D3D11ComPtr.h"

namespace ghost
{
    class GHOST_API D3D12RenderDevice : public RenderDevice
    {
        friend class D3D12RenderSystem;
    public:
        virtual bool InitDevice(bool fullscreen, unsigned msaaCount) override;
        virtual bool CheckSampleCount(unsigned checkCount) override;

    private:
        bool _InitSwapchain();

    private:


        D3D_FEATURE_LEVEL _featureLevel;
        D3D_DRIVER_TYPE _driverType;

        //dxgi
        IDXGISwapChainPtr _dxgiSwapchain;
        IDXGIDevicePtr _dxgiDevice;
        IDXGIFactoryPtr _dxgiFactory;
        IDXGIAdapterPtr _dxgiAdapter;
    };

    DECLAR_SMART_POINTER(D3D12RenderDevice)
}

#endif
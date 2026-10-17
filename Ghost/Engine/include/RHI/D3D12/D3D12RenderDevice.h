#ifndef _D3D12_RENDERDEVICE_H_
#define _D3D12_RENDERDEVICE_H_

#include "RenderDevice.h"
#include "D3D12ComPtr.h"
#include "D3D12CommandQueue.h"

namespace ghost
{
    constexpr int32_t GHOST_BACKBUFFER_COUNT = 2; 
    class GHOST_API D3D12RenderDevice : public RenderDevice
    {
        friend class D3D12RenderSystem;
    public:
        virtual bool InitDevice(bool fullscreen, unsigned msaaCount) override;
        virtual bool CheckSampleCount(unsigned checkCount) override;
        
        ID3D12DevicePtr GetDevice() const { return _device; }

    private:
        bool _InitSwapchain();
        void _CreateDescriptorHeap();
        void _GetDescriptorSize();
        
        D3D12_CPU_DESCRIPTOR_HANDLE _GetCpuDescriptorHandle(ID3D12DescriptorHeapPtr& heap, uint32_t index, uint32_t incrementSize);

    private:

        D3D_FEATURE_LEVEL _featureLevel;
        D3D_DRIVER_TYPE _driverType;
        
        ID3D12DevicePtr _device;
        
        ID3D12DescriptorHeapPtr _rtvDescriptorHeap;
        ID3D12DescriptorHeapPtr _dsvDescriptorHeap;
        ID3D12DescriptorHeapPtr _viewDescriptorHeap;
        
        std::array<ID3D12ResourcePtr, GHOST_BACKBUFFER_COUNT> _backBufferTextures;

        //dxgi
        IDXGISwapChainPtr1 _dxgiSwapchain1;
        IDXGIDevicePtr _dxgiDevice;
        IDXGIFactoryPtr6 _dxgiFactory6;
        IDXGIAdapterPtr _dxgiAdapter;
        
        bool _enableDebugLayer = false;
        bool _enableValidationLayer = false;
        
        uint32_t _rtvDescriptorSize;
        uint32_t _dsvDescriptorSize;
        uint32_t _viewDescriptorSize;
        
        D3D12CommandQueuePtr _graphicsCommandQueue;
    };

    DECLAR_SMART_POINTER(D3D12RenderDevice)
}

#endif
#include "D3D12RenderDevice.h"
#include "Engine.h"
#include "LogManager.h"

namespace ghost
{
    bool D3D12RenderDevice::InitDevice(bool fullscreen, unsigned msaaCount)
    {
#if (defined(DEBUG) || defined(_DEBUG))
        _enableValidationLayer = true;
        _enableDebugLayer = true;
#endif
        if (_enableDebugLayer)
        {
            ID3D12DebugPtr d3d12DebugInterface;
            if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(d3d12DebugInterface.GetAddressOf()))))
            {
                d3d12DebugInterface->EnableDebugLayer();
                if (_enableValidationLayer)
                {
                    ID3D12DebugPtr1 d3d12DebugInterface1;
                    if (SUCCEEDED(d3d12DebugInterface->QueryInterface(IID_PPV_ARGS(d3d12DebugInterface1.GetAddressOf()))))
                    {
                        d3d12DebugInterface1->SetEnableGPUBasedValidation(true);
                    }
                }
            }
            else
            {
                GHOST_LOG_FORMAT_ERROR("Failed to create D3D12 debug interface");
            }
        }
        
        DWORD dxgiFactoryFlags = 0;
        CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(_dxgiFactory6.GetAddressOf()));
        
        IDXGIAdapterPtr1 dxgiAdapter1;
        for (uint32_t adapterIndex = 0; DXGI_ERROR_NOT_FOUND != _dxgiFactory6->EnumAdapters1(adapterIndex, dxgiAdapter1.GetAddressOf()) ; ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
            dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);
            
            if (dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                continue;
            
            if (FAILED(D3D12CreateDevice(dxgiAdapter1.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(_device.GetAddressOf()))))
                continue;
        }
        
        _CreateDescriptorHeap();
        _InitSwapchain();
        
        return true;
    }

    void D3D12RenderDevice::_CreateDescriptorHeap()
    {
        D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc;
        rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvDescriptorHeapDesc.NumDescriptors = GHOST_BACKBUFFER_COUNT;
        rtvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        rtvDescriptorHeapDesc.NodeMask = 0;
        _device->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(_rtvDescriptorHeap.GetAddressOf()));
        
        D3D12_DESCRIPTOR_HEAP_DESC dsvDescriptorHeapDesc;
        dsvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        dsvDescriptorHeapDesc.NumDescriptors = 1;
        dsvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        dsvDescriptorHeapDesc.NodeMask = 0;
        _device->CreateDescriptorHeap(&dsvDescriptorHeapDesc, IID_PPV_ARGS(_dsvDescriptorHeap.GetAddressOf()));
    }

    bool D3D12RenderDevice::_InitSwapchain()
    {
        DXGI_SWAP_CHAIN_DESC1 dxgiSwapChainDesc = {};
        dxgiSwapChainDesc.Width =Engine::GetInstance()->GetWidth();
        dxgiSwapChainDesc.Height =Engine::GetInstance()->GetHeight();
        dxgiSwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        dxgiSwapChainDesc.Scaling = DXGI_SCALING_NONE;
        dxgiSwapChainDesc.SampleDesc.Count = 1;
        dxgiSwapChainDesc.SampleDesc.Quality = 0;
        dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        dxgiSwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
        dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        dxgiSwapChainDesc.BufferCount = GHOST_BACKBUFFER_COUNT;
        
        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullDesc;
        memset(&fullDesc, 0, sizeof(DXGI_SWAP_CHAIN_FULLSCREEN_DESC));
        fullDesc.Windowed = !_fullscreen;
        
        HWND window = (HWND)Engine::GetInstance()->GetWindow();
        HRESULT hr = _dxgiFactory6->CreateSwapChainForHwnd(
                _device.Get(),
                window,
                &dxgiSwapChainDesc,
                &fullDesc,
                nullptr,
                _dxgiSwapchain1.ReleaseAndGetAddressOf()
                );
        if (FAILED(hr))
        {
            return false;
        }

        for (uint32_t i = 0; i < GHOST_BACKBUFFER_COUNT; ++i)
        {
            _dxgiSwapchain1->GetBuffer(i, IID_PPV_ARGS(_backBufferTextures[i].ReleaseAndGetAddressOf()));
            
            D3D12_CPU_DESCRIPTOR_HANDLE rsvHandle = _GetCpuDescriptorHandle(_rtvDescriptorHeap, i, _rtvDescriptorSize);
            _device->CreateRenderTargetView(_backBufferTextures[i].Get(), nullptr, rsvHandle);
        }
        
        D3D12_RESOURCE_DESC dsvDesc = {};
        dsvDesc.Width = Engine::GetInstance()->GetWidth();
        dsvDesc.Height = Engine::GetInstance()->GetHeight();
        dsvDesc.MipLevels = 1;
        
        
        return true;
    }
    
    D3D12_CPU_DESCRIPTOR_HANDLE D3D12RenderDevice::_GetCpuDescriptorHandle(ID3D12DescriptorHeapPtr& heap, uint32_t index, uint32_t incrementSize)
    {
        D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += static_cast<uint64_t>(index) * incrementSize;
        return handle;
    }

    void D3D12RenderDevice::_GetDescriptorSize()
    {
        _rtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        _dsvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
        _viewDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }

    bool D3D12RenderDevice::CheckSampleCount(unsigned checkCount)
    {
        return true;
    }
}

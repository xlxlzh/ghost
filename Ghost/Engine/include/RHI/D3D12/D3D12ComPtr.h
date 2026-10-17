#ifndef _D3D12_COMPTR_H_
#define _D3D12_COMPTR_H_

#include <wrl/client.h>
#include <d3d12.h>
#include <dxgi.h>
#include <dxgi1_3.h>
#include <dxgi1_6.h>

using namespace Microsoft::WRL;

using ID3D12DevicePtr               = ComPtr<ID3D12Device>;
using ID3D12CommandQueuePtr         = ComPtr<ID3D12CommandQueue>;
using ID3D12CommandAllocatorPtr     = ComPtr<ID3D12CommandAllocator>;
using ID3D12GraphicsCommandListPtr  = ComPtr<ID3D12GraphicsCommandList>;
using ID3D12DescriptorHeapPtr       = ComPtr<ID3D12DescriptorHeap>;
using ID3D12ResourcePtr             = ComPtr<ID3D12Resource>;
using ID3D12FencePtr                = ComPtr<ID3D12Fence>;

using ID3D12DebugPtr                = ComPtr<ID3D12Debug>;
using ID3D12DebugPtr1               = ComPtr<ID3D12Debug1>;

using IDXGIAdapterPtr               = ComPtr<IDXGIAdapter>;
using IDXGIAdapterPtr1              = ComPtr<IDXGIAdapter1>;
using IDXGIAdapterPtr2              = ComPtr<IDXGIAdapter2>;

using IDXGIDevicePtr                = ComPtr<IDXGIDevice1>;

using IDXGIFactoryPtr               = ComPtr<IDXGIFactory>;
using IDXGIFactoryPtr1              = ComPtr<IDXGIFactory1>;
using IDXGIFactoryPtr2              = ComPtr<IDXGIFactory2>;
using IDXGIFactoryPtr3              = ComPtr<IDXGIFactory3>;
using IDXGIFactoryPtr4              = ComPtr<IDXGIFactory4>;
using IDXGIFactoryPtr5              = ComPtr<IDXGIFactory5>;
using IDXGIFactoryPtr6              = ComPtr<IDXGIFactory6>;

using IDXGISwapChainPtr             = ComPtr<IDXGISwapChain>;
using IDXGISwapChainPtr1            = ComPtr<IDXGISwapChain1>;

#endif // !_D3D11_COMPTR_H_

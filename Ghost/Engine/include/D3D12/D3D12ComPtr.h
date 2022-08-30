#ifndef _D3D12_COMPTR_H_
#define _D3D12_COMPTR_H_

#include <wrl/client.h>
#include <d3d12.h>

using namespace Microsoft::WRL;

using ID3D12DevicePtr               = ComPtr<ID3D12Device>;
using ID3D12CommandQueuePtr         = ComPtr<ID3D12CommandQueue>;
using ID3D12CommandAllocatorPtr     = ComPtr<ID3D12CommandAllocator>;
using ID3D12CommandListPtr          = ComPtr<ID3D12CommandList>;
using ID3D12GraphicsCommandListPtr  = ComPtr<ID3D12GraphicsCommandList>;
using ID3D12DescriptorHeapPtr       = ComPtr<ID3D12DescriptorHeap>;
using ID3D12ResourcePtr             = ComPtr<ID3D12Resource>;
using ID3D12FencePtr                = ComPtr<ID3D12Fence>;


#endif // !_D3D11_COMPTR_H_

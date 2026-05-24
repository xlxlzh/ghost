#ifndef _D3D11_COMPTR_H_
#define _D3D11_COMPTR_H_

#include <wrl/client.h>
#include <d3d11.h>
#include <d3dcompiler.h>

using namespace Microsoft::WRL;

#ifdef GHOST_USE_D3D_11_1

#include <d3d11_1.h>

using ID3D11DevicePtr               = ComPtr<ID3D11Device1>;
using ID3D11DeviceContextPtr        = ComPtr<ID3D11DeviceContext1>;
using ID3D11RasterizerStatePtr      = ComPtr<ID3D11RasterizerState1>;
using IDXGIAdapterPtr               = ComPtr<IDXGIAdapter1>;
using IDXGIDevicePtr                = ComPtr<IDXGIDevice2>;
using IDXGIFactoryPtr               = ComPtr<IDXGIFactory2>;
using IDXGISwapChainPtr             = ComPtr<IDXGISwapChain1>;
using ID3DUserDefinedAnnotationPtr  = ComPtr<ID3DUserDefinedAnnotation>;

#else

using ID3D11DevicePtr               = ComPtr<ID3D11Device>;
using ID3D11DeviceContextPtr        = ComPtr<ID3D11DeviceContext>;
using ID3D11RasterizerStatePtr      = ComPtr<ID3D11RasterizerState>;
using IDXGIAdapterPtr               = ComPtr<IDXGIAdapter1>;
using IDXGIDevicePtr                = ComPtr<IDXGIDevice1>;
using IDXGIFactoryPtr               = ComPtr<IDXGIFactory1>;
using IDXGISwapChainPtr             = ComPtr<IDXGISwapChain>;

#endif

using ID3D11BufferPtr               = ComPtr<ID3D11Buffer>;
using ID3D11Texture1DPtr            = ComPtr<ID3D11Texture1D>;
using ID3D11Texture2DPtr            = ComPtr<ID3D11Texture2D>;
using ID3D11Texture3DPtr            = ComPtr<ID3D11Texture3D>;
using ID3D11InputLayoutPtr          = ComPtr<ID3D11InputLayout>;
using ID3D11SamplerStatePtr         = ComPtr<ID3D11SamplerState>;

using ID3D11DepthStencilStatePtr    = ComPtr<ID3D11DepthStencilState>;
using ID3D11ShaderResourceViewPtr   = ComPtr<ID3D11ShaderResourceView>;
using ID3D11RenderTargetViewPtr     = ComPtr<ID3D11RenderTargetView>;
using ID3D11UnorderedAccessViewPtr  = ComPtr<ID3D11UnorderedAccessView>;
using ID3D11DepthStencilViewPtr     = ComPtr<ID3D11DepthStencilView>;
using ID3D11BlendStatePtr           = ComPtr<ID3D11BlendState>;

using ID3D11ShaderReflectionPtr     = ComPtr<ID3D11ShaderReflection>;

using ID3DBlobPtr                   = ComPtr<ID3DBlob>;


#endif // !_D3D11_COMPTR_H_

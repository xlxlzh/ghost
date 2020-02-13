#include "D3D11RenderDevice.h"

namespace ghost
{
    bool D3D11RenderDevice::initDevice(bool fullscreen, unsigned msaaCount)
    {
        HRESULT hr = S_OK;

        unsigned createFlag = 0;
#if defined(DEBUG) || defined(_DEBUG)
        createFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_DRIVER_TYPE driverTypes[] =
        {
            D3D_DRIVER_TYPE_HARDWARE,
            D3D_DRIVER_TYPE_WARP,
            D3D_DRIVER_TYPE_REFERENCE
        };

        unsigned numDriverTypes = ARRAYSIZE(driverTypes);

        D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0
        };

        unsigned featureNums = ARRAYSIZE(featureLevels);

        D3D_FEATURE_LEVEL featureLevel;
        D3D_DRIVER_TYPE driverType;
        for (unsigned i = 0; i < numDriverTypes; ++i)
        {
            driverType = driverTypes[i];
            hr = D3D11CreateDevice(nullptr, driverType, nullptr, createFlag, featureLevels, featureNums,
                D3D11_SDK_VERSION, _device.GetAddressOf(), &featureLevel, _context.GetAddressOf());

            if (SUCCEEDED(hr))
            {
                break;
            }
        }

        if (FAILED(hr))
        {
            return false;
        }

        _featureLevel = featureLevel;
        _driverType = driverType;

        unsigned msaaQuality = 0;
        hr = _device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, msaaCount, &msaaQuality);
        if (FAILED(hr))
        {
            msaaCount = 1;
            msaaQuality = 0;
        }

        _sampleCount = msaaCount;
        _sampleQulity = msaaQuality;
        _fullscreen = fullscreen;

        return _initSwapchain();
    }

    bool D3D11RenderDevice::_initSwapchain()
    {
        HRESULT hr = S_OK;

        _device.As(&_dxgiDevice);

        _dxgiDevice->GetAdapter(_dxgiAdapter.GetAddressOf());

        _dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(_dxgiFactory.GetAddressOf()));

        DXGI_SWAP_CHAIN_DESC swapchainDesc;
        memset(&swapchainDesc, 0, sizeof(DXGI_SWAP_CHAIN_DESC));
        swapchainDesc.BufferCount = 1;
        swapchainDesc.BufferDesc.Width = _width;
        swapchainDesc.BufferDesc.Height = _height;
        swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapchainDesc.BufferDesc.RefreshRate.Numerator = 60;
        swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapchainDesc.Flags = 0;
        swapchainDesc.OutputWindow = nullptr;
        swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapchainDesc.Windowed = _fullscreen;
        swapchainDesc.SampleDesc.Count = _sampleCount;
        swapchainDesc.SampleDesc.Quality = _sampleQulity;

        hr = _dxgiFactory->CreateSwapChain(_device.Get(), &swapchainDesc, _dxgiSwapchain.GetAddressOf());
        if (FAILED(hr))
        {
            return false;
        }

        hr = _dxgiSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(_defaultRenderTexture.GetAddressOf()));
        if (FAILED(hr))
        {
            return false;
        }

        hr = _device->CreateRenderTargetView(_defaultRenderTexture.Get(), nullptr, _defaultRenderView.GetAddressOf());
        if (FAILED(hr))
        {
            return false;
        }

        D3D11_TEXTURE2D_DESC depthDesc;
        depthDesc.ArraySize = 1;
        depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthDesc.CPUAccessFlags = 0;
        depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthDesc.Height = _height;
        depthDesc.MipLevels = 1;
        depthDesc.MiscFlags = 0;
        depthDesc.Usage = D3D11_USAGE_DEFAULT;
        depthDesc.Width = _width;

        if (_sampleCount > 1)
        {
            depthDesc.SampleDesc.Count = _sampleCount;
            depthDesc.SampleDesc.Quality = _sampleQulity - 1;
        }
        else
        {
            depthDesc.SampleDesc.Count = 1;
            depthDesc.SampleDesc.Quality = 0;
        }

        hr = _device->CreateTexture2D(&depthDesc, nullptr, _defaultDepthTexture.GetAddressOf());
        if (FAILED(hr))
        {
            return false;
        }

        hr = _device->CreateDepthStencilView(_defaultDepthTexture.Get(), nullptr, _defaultDepthView.GetAddressOf());
        if (FAILED(hr))
        {
            return false;
        }

        return true;
    }

    bool D3D11RenderDevice::checkSampleCount(unsigned checkCount)
    {
        unsigned checkQuality = 0;
        _device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, checkCount, &checkQuality);
        return checkQuality != 0;
    }
}
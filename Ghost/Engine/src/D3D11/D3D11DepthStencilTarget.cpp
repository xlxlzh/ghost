#include "D3D11DepthStencilTarget.h"
#include "D3D11Mappings.h"
#include "D3D11RenderDevice.h"
#include "Engine.h"
#include "LogManager.h"

namespace ghost
{
    D3D11DepthStencilTarget::D3D11DepthStencilTarget(unsigned w, unsigned h, bool msaa, bool srv) :
        DepthStencilTarget(w, h, msaa, srv)
    {
        _onCreateDepthStencilTarget();
    }

    D3D11DepthStencilTarget::~D3D11DepthStencilTarget()
    {
        _onDestoryDepthStencilTarget();
    }

    void D3D11DepthStencilTarget::_onCreateDepthStencilTarget()
    {
        D3D11RenderDevicePtr device = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, Engine::getInstance()->getRenderDevice());
        ID3D11DevicePtr d3d11Device = device->getDevice();

        D3D11_TEXTURE2D_DESC texDesc = { 0 };
        texDesc.Width = _width;
        texDesc.Height = _height;
        texDesc.Format = device->getFeatureLevel() > D3D_FEATURE_LEVEL_10_0 ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_D24_UNORM_S8_UINT;
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.CPUAccessFlags = 0;
        texDesc.ArraySize = 1;
        texDesc.MipLevels = 1;
        texDesc.MiscFlags = 0;
        texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        if (_msaa)
        {
            texDesc.SampleDesc.Count = device->getMSAACount();
            texDesc.SampleDesc.Quality = device->getMSAAQuality();
        }
        else
        {
            texDesc.SampleDesc.Count = 1;
            texDesc.SampleDesc.Quality = 0;
        }

        if (_srv)
            texDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;

        HRESULT hr = d3d11Device->CreateTexture2D(&texDesc, nullptr, _depthTexture.ReleaseAndGetAddressOf());
        if (FAILED(hr))
        {
            GHOST_LOG_FORMAT_ERROR_THROW("%s CreateTexture2D failed.", __FUNCTION__);
        }

        if (device->getFeatureLevel() > D3D_FEATURE_LEVEL_10_0 && _srv)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
            viewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
            viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            viewDesc.Texture2D.MostDetailedMip = 0;
            viewDesc.Texture2D.MipLevels = 1;
            HRESULT hr = d3d11Device->CreateShaderResourceView(_depthTexture.Get(), &viewDesc, _srvView.ReleaseAndGetAddressOf());
            if (FAILED(hr))
            {
                GHOST_LOG_FORMAT_ERROR_THROW("%s CreateShaderResourceView failed.", __FUNCTION__);
            }
        }

        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
        dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        dsvDesc.ViewDimension = _msaa ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
        dsvDesc.Texture2D.MipSlice = 0;
        dsvDesc.Flags = 0;

        hr = d3d11Device->CreateDepthStencilView(_depthTexture.Get(), &dsvDesc, _depthView.ReleaseAndGetAddressOf());
        if (FAILED(hr))
        {
            GHOST_LOG_FORMAT_ERROR_THROW("%s CreateDepthStencilView failed.", __FUNCTION__);
        }
    }

    void D3D11DepthStencilTarget::_onDestoryDepthStencilTarget()
    {
        _depthView.Reset();
        _depthTexture.Reset();
    }
}
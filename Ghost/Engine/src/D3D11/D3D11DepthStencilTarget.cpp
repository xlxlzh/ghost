#include "D3D11DepthStencilTarget.h"
#include "D3D11Mappings.h"
#include "D3D11RenderDevice.h"
#include "Engine.h"
#include "LogManager.h"

namespace ghost
{
    D3D11DepthStencilTarget::D3D11DepthStencilTarget(unsigned w, unsigned h, bool msaa, bool floatDepth) :
        DepthStencilTarget(w, h, msaa, floatDepth)
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
        texDesc.Format = D3D11Mappings::getFormat(_format);
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

        HRESULT hr = d3d11Device->CreateTexture2D(&texDesc, nullptr, _depthTexture.ReleaseAndGetAddressOf());
        if (FAILED(hr))
        {
            GHOST_LOG_FORMAT_ERROR_THROW("%s CreateTexture2D failed.", __FUNCTION__);
        }

        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
        dsvDesc.Format = texDesc.Format;
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
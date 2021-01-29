#include "D3D11RenderTarget.h"
#include "D3D11Texture2D.h"
#include "Engine.h"
#include "D3D11RenderDevice.h"
#include "D3D11Mappings.h"
#include "LogManager.h"

namespace ghost
{
    D3D11RenderTarget::D3D11RenderTarget(unsigned w, unsigned h, unsigned numRTs, GhostColorFormat* formats, bool srv, bool msaa, bool depth) :
        RenderTarget(w, h, numRTs, formats, srv, msaa, depth)
    {
        _onCreateRenderTarget();
    }

    void D3D11RenderTarget::_onCreateRenderTarget()
    {
        RenderSystemPtr renderSystem = Engine::getInstance()->getRenderSystem();
        if (_numRTs > 0 && _width > 0 && _height)
        {
            D3D11RenderDevicePtr device = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, Engine::getInstance()->getRenderDevice());
            ID3D11DevicePtr d3d11Device = device->getDevice();

            for (unsigned i = 0; i < _numRTs; ++i)
            {
                D3D11_TEXTURE2D_DESC texDesc = { };
                texDesc.Width = _width;
                texDesc.Height = _height;
                texDesc.Format = D3D11Mappings::getFormat(_formats[i]);
                texDesc.ArraySize = 1;
                texDesc.MipLevels = 1;
                texDesc.CPUAccessFlags = 0;
                texDesc.BindFlags = _srv ? D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE : D3D11_BIND_RENDER_TARGET;
                texDesc.Usage = D3D11_USAGE_DEFAULT;
                
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

                HRESULT hr = d3d11Device->CreateTexture2D(&texDesc, nullptr, _renderTextures[i].ReleaseAndGetAddressOf());
                if (FAILED(hr))
                {
                    GHOST_LOG_FORMAT_ERROR_THROW("%s CreateTexture2D failed", __FUNCTION__);
                }

                D3D11_RENDER_TARGET_VIEW_DESC rtDecs;
                rtDecs.Format = texDesc.Format;
                rtDecs.Texture2D.MipSlice = 0;
                rtDecs.ViewDimension = _msaa ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;

                hr = d3d11Device->CreateRenderTargetView(_renderTextures[i].Get(),
                    &rtDecs, _renderTargets[i].ReleaseAndGetAddressOf());
                if (FAILED(hr))
                {
                    GHOST_LOG_FORMAT_ERROR_THROW("%s CreateRenderTargetView failed", __FUNCTION__);
                }

                if (_srv)
                {
                    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
                    srvDesc.Format = texDesc.Format;
                    srvDesc.Texture2D.MipLevels = 0;
                    srvDesc.ViewDimension = _msaa ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;

                    hr = d3d11Device->CreateShaderResourceView(_renderTextures[i].Get(),
                        &srvDesc, _srvs[i].ReleaseAndGetAddressOf());
                    if (FAILED(hr))
                    {
                        GHOST_LOG_FORMAT_ERROR_THROW("%s CreateShaderResourceView failed", __FUNCTION__);
                    }
                }
            }

            if (_depthAttach)
            {
                _depthBuffer = Engine::getInstance()->getRenderDevice()->createDepthStencilTarget(_width, _height, false, _msaa, false);
            }
        }
    }

    void D3D11RenderTarget::_onDestoryRenderTarget()
    {
        for (unsigned i = 0; i < _numRTs; ++i)
        {
            _renderTargets[i].Reset();
            _renderTextures[i].Reset();
        }

        _numRTs = 0;
    }

    void D3D11RenderTarget::_onUpdateRenderTarget()
    {

    }

    unsigned D3D11RenderTarget::getNumOfViews() const
    {
        return _numRTs;
    }

    ID3D11RenderTargetViewPtr D3D11RenderTarget::getRenderTargetViewByIndex(unsigned index /* = 0 */)
    {
        ID3D11RenderTargetViewPtr rt = _renderTargets[index];
        return rt ? rt : nullptr;
    }

    ID3D11Texture2DPtr D3D11RenderTarget::getSurface(unsigned index /* = 0 */)
    {
        ID3D11Texture2DPtr rt = _renderTextures[index];
        return rt ? rt : nullptr;
    }
}


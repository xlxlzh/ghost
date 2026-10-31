#include "DataStream.h"
#include "LogManager.h"
#include "D3D11Texture2D.h"
#include "D3D11RenderDevice.h"
#include "D3D11Mappings.h"

namespace ghost
{
    ID3D11Texture2DPtr D3D11Texture2D::_whiteTexture = nullptr;

    D3D11Texture2D::D3D11Texture2D()
    {

    }

    D3D11Texture2D::D3D11Texture2D(const Texture2DResPtr& texRes) : Texture2D(texRes)
    {
        InternalCreateTexture();
    }

    D3D11Texture2D::~D3D11Texture2D()
    {

    }

    void D3D11Texture2D::InternalCreateTexture()
    {
        if (!_rawTexture || _width <= 0 || _height <= 0)
            return;

        DXGI_FORMAT format = D3D11Mappings::GetFormat(_format);
        if (format == DXGI_FORMAT_UNKNOWN)
        {
            GHOST_LOG_FORMAT_ERROR("D3D11Texture2D::InternalCreateTexture failed, unsupported pixel format, texture name[%s]", _name.c_str());
            return;
        }

        D3D11RenderDevicePtr device = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, Engine::GetInstance()->GetRenderDevice());
        ID3D11DevicePtr d3d11Device = device->GetDevice();

        D3D11_TEXTURE2D_DESC texDesc{ 0 };
        texDesc.Width = _width;
        texDesc.Height = _height;
        texDesc.ArraySize = 1;
        texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        texDesc.Format = format;
        texDesc.Usage = D3D11Mappings::GetUsage(_usage);
        texDesc.CPUAccessFlags = D3D11Mappings::GetAccessFlags(_usage);
        texDesc.SampleDesc.Count = 1;
        texDesc.SampleDesc.Quality = 0;
        texDesc.MiscFlags = 0;
        texDesc.MipLevels = 1;

        D3D11_SUBRESOURCE_DATA subData{ 0 };
        if (_rawTexture->IsHdr())
        {
            subData.pSysMem = _rawTexture->GetHdrDatas().data();
        }
        else if (_rawTexture->Is16Bit())
        {
            subData.pSysMem = _rawTexture->Get16BitDatas().data();
        }
        else
        {
            subData.pSysMem = _rawTexture->GetDatas().data();
        }

        subData.SysMemPitch = _width * PixelFormatUtilies::GetBytesPerPixel(_format);

        HRESULT hr = S_OK;
        hr = d3d11Device->CreateTexture2D(&texDesc, &subData, _texture.ReleaseAndGetAddressOf());
        if (FAILED(hr))
        {
            GHOST_LOG_FORMAT_ERROR("D3D11Texture2D::InternalCreateTexture failed, texture name[%s]", _name.c_str());
            return;
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc{ };
        viewDesc.Format = format;
        viewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
        viewDesc.Texture2D.MipLevels = texDesc.MipLevels;
        viewDesc.Texture2D.MostDetailedMip = 0;
        hr = d3d11Device->CreateShaderResourceView(_texture.Get(), &viewDesc, _shaderResourceView.ReleaseAndGetAddressOf());
        if (FAILED(hr))
        {
            GHOST_LOG_FORMAT_ERROR("D3D11Texture2D::InternalCreateTexture failed, texture name[%s]", _name.c_str());
            return;
        }
    }
}
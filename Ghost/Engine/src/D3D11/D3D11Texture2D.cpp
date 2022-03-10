#include "DataStream.h"
#include "LogManager.h"
#include "D3D11Texture2D.h"
#include "D3D11RenderDevice.h"
#include "D3D11Mappings.h"

namespace ghost
{
    D3D11Texture2D::D3D11Texture2D()
    {

    }

    D3D11Texture2D::~D3D11Texture2D()
    {

    }

    void D3D11Texture2D::_createTextureInternal()
    {
        D3D11RenderDevicePtr device = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, Engine::getInstance()->getRenderDevice());
        ID3D11DevicePtr d3d11Device = device->getDevice();

        D3D11_TEXTURE2D_DESC texDesc{ 0 };
        texDesc.Width = _width;
        texDesc.Height = _height;
        texDesc.ArraySize = 1;
        texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        texDesc.Format = D3D11Mappings::getFormat(_format);
        texDesc.Usage = D3D11Mappings::getUsage(_usage);
        texDesc.CPUAccessFlags = D3D11Mappings::getAccessFlags(_usage);
        texDesc.SampleDesc.Count = 1;
        texDesc.SampleDesc.Quality = 0;

        D3D11_SUBRESOURCE_DATA subData{ 0 };
        subData.pSysMem = _datas;
        subData.SysMemPitch = _width * FormatUtilies::getFormatSizeInByte(_format);

        HRESULT hr = S_OK;
        hr = d3d11Device->CreateTexture2D(&texDesc, &subData, _texture.ReleaseAndGetAddressOf());
        if (FAILED(hr))
        {
            GHOST_LOG_FORMAT_ERROR("D3D11Texture2D::_createTextureInternal failed, texture name[%s]", _name.c_str());
            return;
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc{ };
        viewDesc.Format = D3D11Mappings::getFormat(_format);
        viewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
        viewDesc.Texture2D.MipLevels = 0;
        viewDesc.Texture2D.MostDetailedMip = 0;
        hr = d3d11Device->CreateShaderResourceView(_texture.Get(), &viewDesc, _shaderResourceView.ReleaseAndGetAddressOf());
        if (FAILED(hr))
        {
            GHOST_LOG_FORMAT_ERROR("D3D11Texture2D::_createTextureInternal failed, texture name[%s]", _name.c_str());
            return;
        }
    }
}
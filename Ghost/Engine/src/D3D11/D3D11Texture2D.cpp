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

        HRESULT hr = S_OK;
        D3D11_TEXTURE2D_DESC texDesc{ 0 };
        texDesc.Width = _width;
        texDesc.Height = _height;
        texDesc.ArraySize = 1;
        texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        texDesc.Format = D3D11Mappings::getFormat(_format);
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; //temp
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


    }
}
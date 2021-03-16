#ifndef _D3D11_TEXTURE2D_H_
#define _D3D11_TEXTURE2D_H_

#include "Texture2D.h"
#include "D3D11ComPtr.h"

namespace ghost
{
    class GHOST_API D3D11Texture2D : public Texture2D
    {
    public:
        D3D11Texture2D();
        ~D3D11Texture2D();

        ID3D11Texture2DPtr getD3D11Texture2D() const { return _texture; }

        ID3D11ShaderResourceViewPtr getD3D11ShaderResourceView() const { return _shaderResourceView; }
        ID3D11DepthStencilViewPtr getD3D11DepthStencilView() const { return _depthStencilView; }
        ID3D11RenderTargetViewPtr getD3D11RenderTargetView() const { return _rendertargetView; }

    private:
        ID3D11Texture2DPtr _texture;

        ID3D11ShaderResourceViewPtr _shaderResourceView;
        ID3D11RenderTargetViewPtr _rendertargetView;
        ID3D11DepthStencilViewPtr _depthStencilView;
    };

    DECLAR_SMART_POINTER(D3D11Texture2D)
}

#endif

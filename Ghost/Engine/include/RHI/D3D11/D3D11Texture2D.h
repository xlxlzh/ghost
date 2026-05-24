#ifndef _D3D11_TEXTURE2D_H_
#define _D3D11_TEXTURE2D_H_

#include "Engine.h"
#include "Texture2D.h"
#include "D3D11ComPtr.h"

namespace ghost
{
    class GHOST_API D3D11Texture2D : public Texture2D
    {
    public:
        D3D11Texture2D();
        ~D3D11Texture2D();

        ID3D11Texture2DPtr GetD3D11Texture2D() const { return _texture; }

        ID3D11ShaderResourceViewPtr GetD3D11ShaderResourceView() const { return _shaderResourceView; }
        ID3D11DepthStencilViewPtr GetD3D11DepthStencilView() const { return _depthStencilView; }
        ID3D11RenderTargetViewPtr GetD3D11RenderTargetView() const { return _rendertargetView; }

    protected:
        virtual void InternalCreateTexture() override;

    private:
        ID3D11Texture2DPtr _texture;

        ID3D11ShaderResourceViewPtr _shaderResourceView;
        ID3D11RenderTargetViewPtr _rendertargetView;
        ID3D11DepthStencilViewPtr _depthStencilView;

        static ID3D11Texture2DPtr _whilteTexture;
    };

    DECLAR_SMART_POINTER(D3D11Texture2D)
}

#endif

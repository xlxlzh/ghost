#ifndef _D3D11_RENDERTARGET_H_
#define _D3D11_RENDERTARGET_H_

#include "RenderTarget.h"
#include "D3D11ComPtr.h"
#include "RenderConfig.h"

namespace ghost
{
    class GHOST_API D3D11RenderTarget : public RenderTarget
    {
    public:
        D3D11RenderTarget() { }
        virtual ~D3D11RenderTarget() { }

        unsigned getNumOfViews() const;
        ID3D11RenderTargetViewPtr getRenderTargetViewByIndex(unsigned index = 0);
        ID3D11Texture2DPtr getSurface(unsigned index = 0);

    protected:
        virtual void _onCreateRenderTarget() override;
        virtual void _onDestoryRenderTarget() override;

    protected:
        ID3D11RenderTargetViewPtr _renderTargets[GHOST_MAX_RENDERTARGETS] = {0};
        ID3D11Texture2DPtr _renderTextures[GHOST_MAX_RENDERTARGETS] = { 0 };
    };

    DECLAR_SMART_POINTER(D3D11RenderTarget)
}

#endif // !_RENDERTARGET_H_

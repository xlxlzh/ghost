#ifndef _D3D11_RENDERTARGET_H_
#define _D3D11_RENDERTARGET_H_

#include "RenderTarget.h"
#include "D3D11ComPtr.h"

namespace ghost
{
    class GHOST_API D3D11RenderTarget : public RenderTarget
    {
    public:
        D3D11RenderTarget() { }
        virtual ~D3D11RenderTarget() { }

    protected:
        virtual void _onCreateRenderTarget() override;
        virtual void _onDestoryRenderTarget() override;

    protected:
        ID3D11RenderTargetViewPtr _view;
    };

    DECLAR_SMART_POINTER(RenderTarget)
}

#endif // !_RENDERTARGET_H_

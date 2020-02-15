#ifndef _D3D11_RENDERSYSTEM_H_
#define _D3D11_RENDERSYSTEM_H_

#include <d3d11.h>
#include "RenderSystem.h"
#include "D3D11RenderDevice.h"

namespace ghost
{
    class D3D11RenderSystem : public RenderSystem
    {
    public:
        virtual bool initRendersystem(MSAA msaa = _4x) override;

        virtual void setRenderTarget() override;
        virtual void setRenderTargets() override;
        virtual void setDepthstencil() override;
        virtual void setClearColor(Color cl = Color::Black) override;
        virtual void clearRenderTarget() override;
        virtual void clearRenderTargets() override;

        virtual void drawPrimitive() override;
        virtual void drawPrimitiveIndexed() override;
        virtual void drawPrimitiveInstance() override;

        virtual void endScene() override;

    private:

    };
}

#endif

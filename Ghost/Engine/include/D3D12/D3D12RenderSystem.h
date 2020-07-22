#ifndef _D3D11_RENDERSYSTEM_H_
#define _D3D11_RENDERSYSTEM_H_

#include <d3d11.h>
#include "RenderSystem.h"
#include "D3D12RenderDevice.h"

namespace ghost
{
    class D3D12RenderSystem : public RenderSystem
    {
    public:
        virtual void setRenderTarget(RenderTargetPtr rt) override;
        virtual void clearRenderTarget(TargetClear clearFlag = CLEAR_ALL, Color col = Color::Black, float z = 1.0, unsigned stencil = 0.0) override;

        virtual void drawPrimitive() override;
        virtual void drawPrimitiveIndexed() override;
        virtual void drawPrimitiveInstance() override;

        virtual void endScene() override;

    private:

    };
}

#endif

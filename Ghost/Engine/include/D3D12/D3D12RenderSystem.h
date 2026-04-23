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
        virtual void SetRenderTarget(RenderTargetPtr rt) override;
        virtual void ClearRenderTarget(TargetClear clearFlag = CLEAR_ALL, Color col = Color::Black, float z = 1.0, unsigned stencil = 0.0) override;

        virtual void DrawPrimitive(unsigned numVertices, unsigned startIndex) override;
        virtual void DrawPrimitiveIndexed(unsigned numIndices, unsigned indexLocation, int baseVertIndex) override;
        virtual void DrawPrimitiveInstance() override;

        virtual void EndScene() override;

    private:

    };
}

#endif

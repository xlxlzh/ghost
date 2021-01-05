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
        virtual void setRenderTarget(RenderTargetPtr rt) override;
        virtual void clearRenderTarget(TargetClear clearFlag = CLEAR_ALL, Color col = Color::Black, float z = 1.0, unsigned stencil = 0.0) override;

        virtual void setVertexBuffer(VertexBufferPtr vBuffer) override;
        virtual void setVertexBufferBinding(VertexBufferBinding* binding) override;
        virtual void setIndexBuffer(IndexBufferPtr iBuffer) override;
        virtual void setVertexDeclaration(VertexDeclarationPtr vDecl) override;

        virtual void setPrimitiveType(PrimitiveType pType) override;

        virtual void setShader(const Shader* shader) override;
        virtual void drawPrimitive(unsigned numVertices, unsigned startIndex) override;
        virtual void drawPrimitiveIndexed(unsigned numIndices, unsigned indexLocation, int baseVertIndex) override;
        virtual void drawPrimitiveInstance() override;

        virtual void useDefaultRenderTarget() override;

        virtual void endScene() override;

    protected:
        void _clearRenderTarget(Color cl);

    protected:
        std::map<InputSignatureList*, ID3D11InputLayoutPtr> _inputlayouts;
    };
}

#endif

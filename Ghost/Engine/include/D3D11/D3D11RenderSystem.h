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
        virtual void setConstBuffer(ShaderType shaderType, ConstBufferPtr constBuffer) override;

        virtual void setPrimitiveType(PrimitiveType pType) override;

        virtual void setShader(const Shader* shader) override;
        virtual void drawPrimitive(unsigned numVertices, unsigned startIndex) override;
        virtual void drawPrimitiveIndexed(unsigned numIndices, unsigned indexLocation, int baseVertIndex) override;
        virtual void drawPrimitiveInstance() override;

        virtual void useDefaultRenderTarget() override;

        virtual void endScene() override;

        virtual void setCullMode(CullMode cull) override;
        virtual void setFillMode(FillMode fillMode) override;
        virtual void setDepthBufferParams(bool depthTest, bool depthWrite, CompareFunction depthFunction) override;
        virtual void setDepthTestEnable(bool enable) override;
        virtual void setDepthWriteEnable(bool enable) override;
        virtual void setDepthFunction(CompareFunction fun) override;
        virtual void setColorBufferEnable(bool r, bool g, bool b, bool a) override;

    protected:
        void _clearRenderTarget(Color cl);

    protected:
        std::map<InputSignatureList*, ID3D11InputLayoutPtr> _inputlayouts;

        bool _rasterizerDescChagned = false;
        D3D11_RASTERIZER_DESC _rasterizer;

        bool _depthStencilDescChanged = false;
        D3D11_DEPTH_STENCIL_DESC _depthStencilDesc;

        bool _blendDescChanged = false;
        D3D11_BLEND_DESC _blendDesc;
    };
}

#endif

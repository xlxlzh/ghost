#ifndef _RENDERSYSTEM_H_
#define _RENDERSYSTEM_H_

#include <memory>
#include <array>
#include "RenderDevice.h"
#include "Color.h"
#include "RenderTarget.h"
#include "DepthStencilTarget.h"
#include "RenderConfig.h"
#include "Material.h"
#include "VertexBufferBinding.h"
#include "RenderCommon.h"
#include "RenderOperation.h"
#include "TextureUnitState.h"

namespace ghost
{
    class RenderSystem
    {
    public:
        RenderSystem(RenderDevicePtr device);
        virtual ~RenderSystem();

        virtual bool initRenderSystem() = 0;

        virtual void setClearColor(Color cl = Color::Black);
        Color getClearColor() const { return _clearColor; }

        virtual void setRenderTarget(RenderTargetPtr rt) = 0;
        virtual void clearRenderTarget(TargetClear clearFlag = CLEAR_ALL, Color col = Color::Black, float z = 1.0, unsigned stencil = 0.0) = 0;

        virtual void setVertexBuffer(VertexBufferPtr vBuffer) = 0;
        virtual void setVertexBufferBinding(VertexBufferBindingPtr binding) = 0;
        virtual void setIndexBuffer(IndexBufferPtr iBuffer) = 0;
        virtual void setVertexDeclaration(VertexDeclarationPtr vDecl) = 0;
        virtual void setConstBuffer(ShaderType shaderType, ConstBufferPtr constBuffer) = 0;

        virtual void setPrimitiveType(PrimitiveType pType) = 0;

        virtual void setMaterial(const MaterialPtr& mat);

        virtual void setShader(const Shader* shader) = 0;
        virtual void drawPrimitive(unsigned numVertices, unsigned startIndex) = 0;
        virtual void drawPrimitiveIndexed(unsigned numIndices, unsigned indexLocation, int baseVertIndex) = 0;
        virtual void drawPrimitiveInstance() = 0;

        virtual void endScene() = 0;

        virtual void setCullMode(CullMode cull) = 0;
        virtual void setFillMode(FillMode fillMode) = 0;
        virtual void setDepthBufferParams(bool depthTest, bool depthWrite, CompareFunction depthFunction) = 0;
        virtual void setDepthTestEnable(bool enable) = 0;
        virtual void setDepthWriteEnable(bool enable) = 0;
        virtual void setDepthFunction(CompareFunction fun) = 0;
        virtual void setColorBufferEnable(bool r, bool g, bool b, bool a) = 0;

        virtual void setTextureAddressingMode(unsigned slot, const Sampler::UVWAddressingMode& uvwMode) = 0;
        virtual void setSamplerState(unsigned slot, const Sampler& sampler) = 0;
        virtual void setTexture(unsigned slot, Texture2DPtr tex2D) = 0;

        virtual void render(const RenderOperation& op) = 0;

        //Test interface
        virtual void useDefaultRenderTarget() { }

        void setRenderPass(RenderPass pass) { _currentRenderPass = pass; }
        RenderPass getRenderPass() const { return _currentRenderPass; }

    protected:
        Color _clearColor;

        RenderTargetPtr _activeRenerTarget = nullptr;
        RenderDevicePtr _renderDevice = nullptr;

        MaterialPtr _currentMaterial = nullptr;

        CullMode _cullingMode;
        FillMode _fillMode;

        RenderPass _currentRenderPass;
    };

    DECLAR_SMART_POINTER(RenderSystem)
}

#endif
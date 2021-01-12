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

namespace ghost
{
    enum RendersystemType
    {
        RENDER_D3D11,
        RENDER_D3D12,
        RENDER_OPENGL4,
        RENDER_VULKAN
    };

    enum MSAA
    {
        _1x,
        _2x,
        _4x,
        _8x,
        _16x,
    };

    GHOST_API unsigned GetMSAASampleCount(MSAA ms);

    enum TargetClear
    {
        CLEAR_COLOR = 0x01,
        CLEAR_DEPTH = 0x02,
        CLEAR_STENCIL = 0x04,
        CLEAR_DEPTH_STENCIL = CLEAR_DEPTH | CLEAR_STENCIL,
        CLEAR_ALL = CLEAR_COLOR | CLEAR_DEPTH | CLEAR_STENCIL
    };

    enum PrimitiveType
    {
        PRIMITIVE_UNDEFINED             = 0,
        PRIMITIVE_POINTLIST             = 1,

        PRIMITIVE_LINELIST              = 2,
        PRIMITIVE_LINESTRIP             = 3,

        PRIMITIVE_TRIANGLELIST          = 4,
        PRIMITIVE_TRIANGLESTRIP         = 5
    };

    enum CullMode
    {
        CULL_CW,
        CULL_CCW,
        CULL_NONE,
    };

    enum FillMode
    {
        FILL_SOLID,
        FILL_WIREFRAME,
    };

    enum CompareFunction
    {
        COMPARISON_NEVER,
        COMPARISON_ALWAYS,
        COMPARISON_LESS,
        COMPARISON_LESS_EQUAL,
        COMPARISON_EQUAL,
        COMPARISON_NOT_EQUAL,
        COMPARISON_GREATER_EQUAL,
        COMPARISON_GREATER
    };

    class RenderSystem
    {
    public:
        RenderSystem();
        virtual ~RenderSystem();

        void attachRenderDevice(RenderDevicePtr device) { _renderDevice = device; }

        virtual void setClearColor(Color cl = Color::Black);
        Color getClearColor() const { return _clearColor; }

        virtual void setRenderTarget(RenderTargetPtr rt) = 0;
        virtual void clearRenderTarget(TargetClear clearFlag = CLEAR_ALL, Color col = Color::Black, float z = 1.0, unsigned stencil = 0.0) = 0;

        virtual void setVertexBuffer(VertexBufferPtr vBuffer) = 0;
        virtual void setVertexBufferBinding(VertexBufferBinding* binding) = 0;
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

        //Test interface
        virtual void useDefaultRenderTarget() { }

    protected:
        Color _clearColor;

        RenderTargetPtr _activeRenerTarget = nullptr;
        RenderDevicePtr _renderDevice = nullptr;

        MaterialPtr _currentMaterial = nullptr;

        CullMode _cullingMode;
        FillMode _fillMode;

    };

    DECLAR_SMART_POINTER(RenderSystem)
}

#endif
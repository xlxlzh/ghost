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
    class GHOST_API RenderSystem
    {
    public:
        RenderSystem(RenderDevicePtr device);
        virtual ~RenderSystem();

        virtual bool InitRenderSystem() = 0;

        virtual void SetClearColor(Color cl = Color::Black);
        Color GetClearColor() const { return _clearColor; }

        virtual void SetRenderTarget(RenderTargetPtr rt) = 0;
        virtual void ClearRenderTarget(TargetClear clearFlag = CLEAR_ALL, Color col = Color::Black, float z = 1.0, unsigned stencil = 0.0) = 0;

        virtual void SetVertexBuffer(VertexBufferPtr vBuffer) = 0;
        virtual void SetVertexBufferBinding(VertexBufferBindingPtr binding) = 0;
        virtual void SetIndexBuffer(IndexBufferPtr iBuffer) = 0;
        virtual void SetVertexDeclaration(VertexDeclarationPtr vDecl) = 0;
        virtual void SetConstBuffer(ShaderType shaderType, ConstBufferPtr constBuffer) = 0;

        virtual void SetPrimitiveType(PrimitiveType pType) = 0;

        virtual void SetMaterial(const MaterialPtr& mat);

        virtual void SetShader(const Shader* shader) = 0;
        virtual void DrawPrimitive(unsigned numVertices, unsigned startIndex) = 0;
        virtual void DrawPrimitiveIndexed(unsigned numIndices, unsigned indexLocation, int baseVertIndex) = 0;
        virtual void DrawPrimitiveInstance() = 0;

        virtual void BeginScene() = 0;
        virtual void EndScene() = 0;

        virtual void SetCullMode(CullMode cull) = 0;
        virtual void SetFillMode(FillMode fillMode) = 0;
        virtual void SetDepthBufferParams(bool depthTest, bool depthWrite, CompareFunction depthFunction) = 0;
        virtual void SetDepthTestEnable(bool enable) = 0;
        virtual void SetDepthWriteEnable(bool enable) = 0;
        virtual void SetDepthFunction(CompareFunction fun) = 0;
        virtual void SetColorBufferEnable(bool r, bool g, bool b, bool a) = 0;

        virtual void SetTextureAddressingMode(unsigned slot, const Sampler::UVWAddressingMode& uvwMode) = 0;
        virtual void SetSamplerState(unsigned slot, const Sampler& sampler) = 0;
        virtual void SetTexture(unsigned slot, Texture2DPtr tex2D) = 0;

        virtual void PushGPUEvent(const std::wstring& name) = 0;
        virtual void PopGPUEvent() = 0;

        virtual void Render(const RenderOperation& op) = 0;

        //Test interface
        virtual void UseDefaultRenderTarget() { }

        void SetRenderPass(RenderPass pass);
        RenderPass GetRenderPass() const { return _currentRenderPass; }

    protected:
        Color _clearColor;

        RenderTargetPtr _activeRenerTarget = nullptr;
        RenderDevicePtr _renderDevice = nullptr;

        MaterialPtr _currentMaterial = nullptr;

        CullMode _cullingMode;
        FillMode _fillMode;

        RenderPass _currentRenderPass;
        bool _renderpassChanged{ true };
    };

    DECLAR_SMART_POINTER(RenderSystem)

    class GHOST_API GPUEventScope
    {
    public:
        GPUEventScope(const RenderSystemPtr &rs, const std::wstring& eventName) : _rs(rs)
        {
            _rs->PushGPUEvent(eventName);
        }

        ~GPUEventScope()
        {
            _rs->PopGPUEvent();
        }

    private:
        const RenderSystemPtr _rs;
    };

    #define GHOST_GPU_EVENT_SCOPE(rs, eventName) GPUEventScope GPU_EVENT_##eventName(rs, L#eventName);
    #define GHOST_GPU_EVENT_BEGIN(rs, eventName) rs->PushGPUEvent(L#eventName);
    #define GHOST_GPU_EVENT_END(rs) rs->PopGPUEvent();
}

#endif
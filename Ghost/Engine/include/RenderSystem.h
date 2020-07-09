#ifndef _RENDERSYSTEM_H_
#define _RENDERSYSTEM_H_

#include <memory>
#include <array>
#include "RenderDevice.h"
#include "Color.h"
#include "RenderTarget.h"
#include "DepthStencilTarget.h"

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
        _2x,
        _4x,
        _8x,
    };

    enum TargetClear
    {
        CLEAR_COLOR = 0x01,
        CLEAR_DEPTH = 0x02,
        CLEAR_STENCIL = 0x04
    };

    static const int MAX_RENDERTARGETS = 8;

    class RenderSystem
    {
    public:
        virtual bool initRendersystem() = 0;

        virtual void setRenderTarget(int index, RenderTargetPtr rt) = 0;
        virtual void setRenderTargets() = 0;
        virtual void setDepthstencil(DepthStencilTargetPtr depth) = 0;
        virtual void setClearColor(Color cl = Color::Black) = 0;
        virtual void clearRenderTarget() = 0;
        virtual void clearRenderTargets() = 0;

        virtual void drawPrimitive() = 0;
        virtual void drawPrimitiveIndexed() = 0;
        virtual void drawPrimitiveInstance() = 0;

        virtual void endScene() = 0;

    protected:
        Color _clearColor;

        unsigned _currentRenderTargets = 0;
        std::array<RenderTargetPtr, MAX_RENDERTARGETS> _renderTargets;
        DepthStencilTargetPtr _depthStencil = nullptr;
    };

    DECLAR_SMART_POINTER(RenderSystem)
}

#endif
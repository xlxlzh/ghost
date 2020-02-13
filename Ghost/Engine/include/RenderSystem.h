#ifndef _RENDERSYSTEM_H_
#define _RENDERSYSTEM_H_

#include <memory>
#include "RenderDevice.h"

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

    class RenderSystem
    {
    public:
        virtual bool initRendersystem() = 0;

        virtual void setRenderTarget() = 0;
        virtual void setRenderTargets() = 0;
        virtual void setDepthstencil() = 0;
        virtual void setClearColor() = 0;
        virtual void clearRenderTarget() = 0;
        virtual void clearRenderTargets() = 0;

        virtual void drawPrimitive() = 0;
        virtual void drawPrimitiveIndexed() = 0;
        virtual void drawPrimitiveInstance() = 0;

        virtual void endScene() = 0;

    protected:
        RenderDevicePtr _device;
    };

    using RenderSystemPtr = std::shared_ptr<RenderSystem>;
}

#endif
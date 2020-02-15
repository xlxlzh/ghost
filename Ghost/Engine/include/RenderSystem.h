#ifndef _RENDERSYSTEM_H_
#define _RENDERSYSTEM_H_

#include <memory>
#include "RenderDevice.h"
#include "Color.h"

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
        virtual bool initRendersystem(MSAA msaa = _4x) = 0;

        virtual void setRenderTarget() = 0;
        virtual void setRenderTargets() = 0;
        virtual void setDepthstencil() = 0;
        virtual void setClearColor(Color cl = Color::Black) = 0;
        virtual void clearRenderTarget() = 0;
        virtual void clearRenderTargets() = 0;

        virtual void drawPrimitive() = 0;
        virtual void drawPrimitiveIndexed() = 0;
        virtual void drawPrimitiveInstance() = 0;

        virtual void endScene() = 0;

    protected:
        unsigned _getMsaaCount(MSAA msaa)
        {
            switch (msaa)
            {
            case ghost::_2x:
                return 2;
            case ghost::_4x:
                return 4;
            case ghost::_8x:
                return 8;
            default:
                return 1;
            }
        }

    protected:
        RenderDevicePtr _device;
        Color _clearColor;
    };

    using RenderSystemPtr = std::shared_ptr<RenderSystem>;
}

#endif
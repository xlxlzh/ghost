#ifndef _RENDERTARGET_H_
#define _RENDERTARGET_H_

#include "Ghost.h"
#include "Texture2D.h"
#include "DepthStencilTarget.h"

namespace ghost
{
    class GHOST_API RenderTarget
    {
    public:
        RenderTarget() { }
        virtual ~RenderTarget() { }

        void setSize(unsigned w, unsigned h, unsigned d = 1);
        unsigned getWidth() const { return _width; }
        unsigned getHeight() const { return _height; }
        unsigned getDepth() const { return _depth; }

        void attachDepthBuffer(DepthStencilTargetPtr depth) { _depthBuffer = depth; }
        DepthStencilTargetPtr getAttachDepthBuffer() { return _depthBuffer; }

    protected:
        virtual void _onCreateRenderTarget() = 0;
        virtual void _onDestoryRenderTarget() = 0;

    protected:
        unsigned _width;
        unsigned _height;
        unsigned _depth;

        DepthStencilTargetPtr _depthBuffer = nullptr;
    };

    DECLAR_SMART_POINTER(RenderTarget)
}

#endif // !_RENDERTARGET_H_

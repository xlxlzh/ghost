#ifndef _RENDERTARGET_H_
#define _RENDERTARGET_H_

#include "Ghost.h"
#include "Texture2D.h"
#include "DepthStencilTarget.h"
#include "Color.h"
#include "RenderConfig.h"
#include "Viewport.h"

namespace ghost
{
    struct RenderTargetDesc
    {
        GhostColorFormat Format;
    };

    class GHOST_API RenderTarget
    {
    public:
        RenderTarget() { }
        RenderTarget(unsigned w, unsigned h, unsigned numRTs, GhostColorFormat* formats, bool msaa, bool depth = true);
        virtual ~RenderTarget();

        void setSize(unsigned w, unsigned h, unsigned d = 1);
        unsigned getWidth() const { return _width; }
        unsigned getHeight() const { return _height; }
        unsigned getDepth() const { return _depth; }

        void attachDepthBuffer(DepthStencilTargetPtr depth) { _depthBuffer = depth; }
        DepthStencilTargetPtr getAttachDepthBuffer() { return _depthBuffer; }
        void detachDepthBuffer() { _depthBuffer = nullptr; }

        void attachRenderTexture(Texture2DPtr tex);

        const Viewport& getAttachViewport() const { return _viewport; }
        void attachViewport(const Viewport& vp) { _viewport = vp; }

    protected:
        unsigned _width;
        unsigned _height;
        unsigned _depth = 1;

        unsigned _numRTs = 0;

        bool _msaa = false;
        bool _depthAttach = false;
        GhostColorFormat _formats[GHOST_MAX_RENDERTARGETS];
        DepthStencilTargetPtr _depthBuffer = nullptr;

        bool _update = true;

        Viewport _viewport;
    };

    DECLAR_SMART_POINTER(RenderTarget)
}

#endif // !_RENDERTARGET_H_

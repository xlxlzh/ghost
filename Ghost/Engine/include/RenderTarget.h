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
        RenderTarget(unsigned w, unsigned h, unsigned numRTs, GhostColorFormat* formats, bool srv = false, bool msaa = false, bool depth = true);
        virtual ~RenderTarget();

        void SetSize(unsigned w, unsigned h, unsigned d = 1);
        unsigned GetWidth() const { return _width; }
        unsigned GetHeight() const { return _height; }
        unsigned GetDepth() const { return _depth; }

        void AttachDepthBuffer(DepthStencilTargetPtr depth) { _depthBuffer = depth; }
        DepthStencilTargetPtr GetAttachDepthBuffer() { return _depthBuffer; }
        void DetachDepthBuffer() { _depthBuffer = nullptr; }

        void AttachRenderTexture(Texture2DPtr tex);

        const Viewport& GetAttachViewport() const { return _viewport; }
        void AttachViewport(const Viewport& vp) { _viewport = vp; }

        bool IsSRV() const { return _srv; }

    protected:
        unsigned _width;
        unsigned _height;
        unsigned _depth = 1;

        unsigned _numRTs = 0;

        bool _msaa = false;
        bool _depthAttach = false;
        bool _srv = false;
        GhostColorFormat _formats[GHOST_MAX_RENDERTARGETS];
        DepthStencilTargetPtr _depthBuffer = nullptr;

        bool _update = true;

        Viewport _viewport;
    };

    DECLAR_SMART_POINTER(RenderTarget)
}

#endif // !_RENDERTARGET_H_

#ifndef _DEPTHSTENCILTARGET_H_
#define _DEPTHSTENCILTARGET_H_

#include "Ghost.h"
#include "Texture2D.h"

namespace ghost
{
    class GHOST_API DepthStencilTarget
    {
    public:
        DepthStencilTarget(unsigned w, unsigned h, bool msaa, bool floatDepth);
        virtual ~DepthStencilTarget();

        unsigned getWidth() const { return _width; }
        unsigned getHeight() const { return _height; }

    protected:
        unsigned _width = 0;
        unsigned _height = 0;

        bool _msaa;
        GhostColorFormat _format;
    };

    DECLAR_SMART_POINTER(DepthStencilTarget)
}

#endif

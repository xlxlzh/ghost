#ifndef _DEPTHSTENCILTARGET_H_
#define _DEPTHSTENCILTARGET_H_

#include "Ghost.h"
#include "Texture2D.h"

namespace ghost
{
    class GHOST_API DepthStencilTarget
    {
    public:
        DepthStencilTarget(unsigned w, unsigned h, bool msaa, bool srv);
        virtual ~DepthStencilTarget();

        unsigned getWidth() const { return _width; }
        unsigned getHeight() const { return _height; }

        bool isSRV() const { return _srv; }

    protected:
        unsigned _width = 0;
        unsigned _height = 0;

        bool _msaa;
        bool _srv;
    };

    DECLAR_SMART_POINTER(DepthStencilTarget)
}

#endif

#ifndef _DEPTHSTENCILTARGET_H_
#define _DEPTHSTENCILTARGET_H_

#include "Ghost.h"
#include "Texture2D.h"

namespace ghost
{
    class GHOST_API DepthStencilTarget
    {
    public:
        DepthStencilTarget() = default;
        virtual ~DepthStencilTarget() { }

        unsigned getWidth() const { return _width; }
        unsigned getHeight() const { return _height; }

    protected:
        virtual void _onCreateDepthStencilTarget() = 0;
        virtual void _onDestoryDepthStencilTarget() = 0;

    protected:
        unsigned _width = 0;
        unsigned _height = 0;
    };

    DECLAR_SMART_POINTER(DepthStencilTarget)
}

#endif

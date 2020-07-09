#ifndef _DEPTHSTENCILTARGET_H_
#define _DEPTHSTENCILTARGET_H_

#include "Ghost.h"
#include "Texture2D.h"

namespace ghost
{
    class GHOST_API DepthStencilTarget
    {
    public:
        DepthStencilTarget(Texture2D* tex) : _linkTexture(tex) { }
        virtual ~DepthStencilTarget() { }

        unsigned getWidth() const { return _linkTexture->getWidth(); }
        unsigned getHeight() const { return _linkTexture->getHeight(); }

        Texture2D* getTexture() const { return _linkTexture; }

    protected:
        virtual void _onCreateDepthStencilTarget() = 0;
        virtual void _onDestoryDepthStencilTarget() = 0;

    protected:
        Texture2D* _linkTexture;
    };

    DECLAR_SMART_POINTER(DepthStencilTarget)
}

#endif

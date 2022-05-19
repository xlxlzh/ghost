#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

#include "Ghost.h"

namespace ghost
{
    class GHOST_API Viewport
    {
    public:
        Viewport();
        Viewport(float x, float y, float w, float h);
        ~Viewport() { }

        float getViewportX() const { return _viewportX; }
        float getViewportY() const { return _viewportY; }

        float getViewportWidth() const { return _viewportWidth; }
        float getViewportHeight() const { return _viewportHeight; }

        float getMinZ() const { return _minZ; }
        float getMaxZ() const { return _maxZ; }

    private:
        float _viewportX, _viewportY;
        float _viewportWidth, _viewportHeight;

        float _minZ, _maxZ;
    };

    DECLAR_SMART_POINTER(Viewport);
}

#endif

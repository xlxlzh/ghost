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

        float GetViewportX() const { return _viewportX; }
        float GetViewportY() const { return _viewportY; }

        float GetViewportWidth() const { return _viewportWidth; }
        float GetViewportHeight() const { return _viewportHeight; }

        float GetMinZ() const { return _minZ; }
        float GetMaxZ() const { return _maxZ; }

    private:
        float _viewportX, _viewportY;
        float _viewportWidth, _viewportHeight;

        float _minZ, _maxZ;
    };

    DECLAR_SMART_POINTER(Viewport);
}

#endif

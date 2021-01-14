#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

#include "Ghost.h"

namespace ghost
{
    class GHOST_API Viewport
    {
    public:
        Viewport();
        Viewport(int x, int y, int w, int h);
        ~Viewport() { }

        int getViewportX() const { return _viewportX; }
        int getViewportY() const { return _viewportY; }

        int getViewportWidth() const { return _viewportWidth; }
        int getViewportHeight() const { return _viewportHeight; }

        float getMinZ() const { return _minZ; }
        float getMaxZ() const { return _maxZ; }

    private:
        int _viewportX, _viewportY;
        int _viewportWidth, _viewportHeight;

        float _minZ, _maxZ;
    };

    DECLAR_SMART_POINTER(Viewport);
}

#endif

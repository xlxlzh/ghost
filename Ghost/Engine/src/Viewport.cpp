#include "Viewport.h"

namespace ghost
{
    Viewport::Viewport()
    {
        _viewportX = _viewportY = 0;
        _viewportWidth = _viewportHeight = 0;

        _minZ = 0.0f;
        _maxZ = 1.0f;
    }

    Viewport::Viewport(int x, int y, int w, int h) :
        _viewportX(x), _viewportY(y), _viewportWidth(w), _viewportHeight(h),
        _minZ(0.0f), _maxZ(1.0f)
    {

    }
}
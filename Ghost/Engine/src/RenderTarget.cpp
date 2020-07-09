#include "RenderTarget.h"

namespace ghost
{
    void RenderTarget::setSize(unsigned w, unsigned h, unsigned d /* = 1 */)
    {
        _width = w;
        _height = h;
        _depth = d;
    }
}
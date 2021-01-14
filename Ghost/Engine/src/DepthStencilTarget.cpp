#include "DepthStencilTarget.h"

namespace ghost
{
    DepthStencilTarget::DepthStencilTarget(unsigned w, unsigned h, bool msaa, bool floatDepth) :
        _width(w), _height(h), _msaa(msaa)
    {
        _format = floatDepth ? GHOST_FORMAT_D24S8 : GHOST_FORMAT_D24S8;
    }

    DepthStencilTarget::~DepthStencilTarget()
    {

    }
}
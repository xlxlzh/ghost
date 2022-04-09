#include "DepthStencilTarget.h"

namespace ghost
{
    DepthStencilTarget::DepthStencilTarget(unsigned w, unsigned h, bool msaa, bool srv) :
        _width(w), _height(h), _msaa(msaa), _srv(srv)
    {
    }

    DepthStencilTarget::~DepthStencilTarget()
    {

    }
}
#include "RenderDevice.h"

namespace ghost
{
    RenderTargetPtr RenderDevice::createSingleRenderTarget(unsigned w, unsigned h, GhostColorFormat format, bool msaa, bool depth /* = true */)
    {
        return createRenderTargets(w, h, 1, &format, msaa, depth);
    }
}
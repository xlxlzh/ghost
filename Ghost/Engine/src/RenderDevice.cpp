#include "RenderDevice.h"

namespace ghost
{
    RenderTargetPtr RenderDevice::CreateSingleRenderTarget(unsigned w, unsigned h, GhostColorFormat format, bool srv, bool msaa, bool depth /* = true */)
    {
        return CreateRenderTargets(w, h, 1, &format, srv, msaa, depth);
    }
}
#include "D3D12RenderDevice.h"
#include "Engine.h"

namespace ghost
{
    bool D3D12RenderDevice::initDevice(bool fullscreen, unsigned msaaCount)
    {
        return true;
    }

    bool D3D12RenderDevice::_initSwapchain()
    {
        return true;
    }

    bool D3D12RenderDevice::checkSampleCount(unsigned checkCount)
    {
        return true;
    }
}
#include "D3D12RenderDevice.h"
#include "Engine.h"

#include "imgui_impl_dx12.h"

namespace ghost
{
    bool D3D12RenderDevice::initDevice(bool fullscreen, unsigned msaaCount)
    {
        HRESULT hr = S_OK;

        D3D_DRIVER_TYPE driverTypes[] =
        {
            D3D_DRIVER_TYPE_HARDWARE,
            D3D_DRIVER_TYPE_WARP,
            D3D_DRIVER_TYPE_REFERENCE
        };

        unsigned numDriverTypes = ARRAYSIZE(driverTypes);

        D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3,
        };

        unsigned featureNums = ARRAYSIZE(featureLevels);

        D3D_FEATURE_LEVEL featureLevel;

        unsigned msaaQuality = 0;
        if (FAILED(hr))
        {
            msaaCount = 1;
            msaaQuality = 0;
        }

        _sampleCount = msaaCount;
        _sampleQulity = msaaQuality;
        _fullscreen = fullscreen;

        _width = Engine::getInstance()->getWidth();
        _height = Engine::getInstance()->getHeight();

        //IMGUI
        //ImGui_ImplDX12_Init(_device.Get(), _context.Get());

        return _initSwapchain();
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
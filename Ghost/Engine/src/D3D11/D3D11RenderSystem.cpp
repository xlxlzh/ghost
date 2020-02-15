#include "D3D11RenderSystem.h"

namespace ghost
{
    bool D3D11RenderSystem::initRendersystem(MSAA msaa /* = _4x */)
    {
        _device = std::make_shared<D3D11RenderDevice>();
        _device->initDevice(false, _getMsaaCount(msaa));

        return true;
    }

    void D3D11RenderSystem::setRenderTarget()
    {

    }

    void D3D11RenderSystem::setRenderTargets()
    {

    }
    void D3D11RenderSystem::setDepthstencil()
    {

    }

    void D3D11RenderSystem::setClearColor(Color cl /* = Color::Black */)
    {
        _clearColor = cl;
    }

    void D3D11RenderSystem::clearRenderTarget()
    {

    }

    void D3D11RenderSystem::clearRenderTargets()
    {

    }

    void D3D11RenderSystem::drawPrimitive()
    {

    }

    void D3D11RenderSystem::drawPrimitiveIndexed()
    {

    }

    void D3D11RenderSystem::drawPrimitiveInstance()
    {

    }

    void D3D11RenderSystem::endScene()
    {

    }
}
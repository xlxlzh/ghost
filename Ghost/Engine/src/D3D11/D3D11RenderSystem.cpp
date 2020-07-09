#include "D3D11RenderSystem.h"
#include "Engine.h"

namespace ghost
{
    bool D3D11RenderSystem::initRendersystem()
    {
        return true;
    }

    void D3D11RenderSystem::setRenderTarget(int index, RenderTargetPtr rt)
    {

    }

    void D3D11RenderSystem::setRenderTargets()
    {

    }
    void D3D11RenderSystem::setDepthstencil(DepthStencilTargetPtr depth)
    {

    }

    void D3D11RenderSystem::setClearColor(Color cl /* = Color::Black */)
    {
        _clearColor = cl;
    }

    void D3D11RenderSystem::clearRenderTarget()
    {
        D3D11RenderDevicePtr d3dDevice = std::dynamic_pointer_cast<D3D11RenderDevice>(Engine::getInstance()->getRenderDevice());
        d3dDevice->_context->ClearRenderTargetView(d3dDevice->_defaultRenderView.Get(), _clearColor.getColorPtr());
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
        D3D11RenderDevicePtr d3dDevice = std::dynamic_pointer_cast<D3D11RenderDevice>(Engine::getInstance()->getRenderDevice());
        d3dDevice->_dxgiSwapchain->Present(0, 0);
    }
}
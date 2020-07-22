#include "D3D11RenderSystem.h"
#include "Engine.h"
#include "D3D11RenderTarget.h"
#include "D3D11DepthStencilTarget.h"

namespace ghost
{
    void D3D11RenderSystem::setRenderTarget(RenderTargetPtr rt)
    {
        _activeRenerTarget = rt;

        D3D11RenderTargetPtr targetPtr = GHOST_SMARTPOINTER_CAST(D3D11RenderTarget, rt);
        if (rt && targetPtr)
        {
            ID3D11RenderTargetView* renderViews[GHOST_MAX_RENDERTARGETS] = {0};

            unsigned numViews = targetPtr->getNumOfViews();
            for (unsigned i = 0; i < GHOST_MAX_RENDERTARGETS; ++i)
            {
                renderViews[i] = targetPtr->getRenderTargetViewByIndex(i).Get();
                if (!renderViews[i])
                {
                    break;;
                }
            }

            ID3D11DepthStencilView* depthView = nullptr;
            D3D11DepthStencilTargetPtr depthBuffer = GHOST_SMARTPOINTER_CAST(D3D11DepthStencilTarget, rt->getAttachDepthBuffer());
            if (depthBuffer)
            {
                depthView = depthBuffer->getDepthView().Get();
            }


        }
    }

    void D3D11RenderSystem::clearRenderTarget(TargetClear clearFlag /* = CLEAR_ALL */, Color col /* = Color::Black */, float z /* = 1.0 */, unsigned stencil /* = 0.0 */)
    {
        D3D11RenderDevicePtr d3dDevice = std::dynamic_pointer_cast<D3D11RenderDevice>(Engine::getInstance()->getRenderDevice());
        d3dDevice->_context->ClearRenderTargetView(d3dDevice->_defaultRenderView.Get(), _clearColor.getColorPtr());
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
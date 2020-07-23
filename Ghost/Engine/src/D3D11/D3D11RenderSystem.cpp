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
                    break;
                }
            }

            ID3D11DepthStencilView* depthView = nullptr;
            D3D11DepthStencilTargetPtr depthBuffer = GHOST_SMARTPOINTER_CAST(D3D11DepthStencilTarget, rt->getAttachDepthBuffer());
            if (depthBuffer)
            {
                depthView = depthBuffer->getDepthView().Get();
            }

            D3D11RenderDevicePtr devicePtr = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, _renderDevice);
            devicePtr->_context->OMSetRenderTargets(numViews, renderViews, depthView);
        }
    }

    void D3D11RenderSystem::clearRenderTarget(TargetClear clearFlag /* = CLEAR_ALL */, Color col /* = Color::Black */, float z /* = 1.0 */, unsigned stencil /* = 0.0 */)
    {
        if (clearFlag & TargetClear::CLEAR_COLOR)
        {
            _clearRenderTarget(col);
        }

        D3D11RenderDevicePtr devicePtr = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, _renderDevice);

        unsigned flags = 0;
        if (clearFlag & TargetClear::CLEAR_DEPTH)
        {
            flags |= D3D11_CLEAR_DEPTH;
        }

        if (clearFlag & TargetClear::CLEAR_STENCIL)
        {
            flags |= D3D11_CLEAR_STENCIL;
        }

        if (_activeRenerTarget)
        {
            D3D11DepthStencilTargetPtr depthPtr = GHOST_SMARTPOINTER_CAST(D3D11DepthStencilTarget, _activeRenerTarget->getAttachDepthBuffer());
            if (depthPtr)
            {
                devicePtr->_context->ClearDepthStencilView(depthPtr->getDepthView().Get(), flags, z, stencil);
            }
        }
        else
        {
            devicePtr->_context->ClearDepthStencilView(devicePtr->_defaultDepthView.Get(), flags, z, stencil);
        }
    }

    void D3D11RenderSystem::_clearRenderTarget(Color cl)
    {
        D3D11RenderDevicePtr devicePtr = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, _renderDevice);
        if (_activeRenerTarget)
        {
            D3D11RenderTargetPtr targetPtr = GHOST_SMARTPOINTER_CAST(D3D11RenderTarget, _activeRenerTarget);
            if (targetPtr)
            {
                ID3D11RenderTargetView* renderViews[GHOST_MAX_RENDERTARGETS] = { 0 };

                unsigned numViews = targetPtr->getNumOfViews();
                for (unsigned i = 0; i < GHOST_MAX_RENDERTARGETS; ++i)
                {
                    renderViews[i] = targetPtr->getRenderTargetViewByIndex(i).Get();
                    if (!renderViews[i])
                    {
                        break;
                    }
                }

                const float* clearColor = cl.getColorPtr();
                for (unsigned i = 0; i < numViews; ++i)
                {
                    devicePtr->_context->ClearRenderTargetView(renderViews[i], clearColor);
                }
            }
        }
        else
        {
            devicePtr->_context->ClearRenderTargetView(devicePtr->_defaultRenderView.Get(), cl.getColorPtr());
        }
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
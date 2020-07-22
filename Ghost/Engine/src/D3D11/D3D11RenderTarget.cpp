#include "D3D11RenderTarget.h"
#include "D3D11Texture2D.h"
#include "Engine.h"

namespace ghost
{
    void D3D11RenderTarget::_onCreateRenderTarget()
    {
        RenderSystemPtr renderSystem = Engine::getInstance()->getRenderSystem();
    }

    void D3D11RenderTarget::_onDestoryRenderTarget()
    {

    }

    unsigned D3D11RenderTarget::getNumOfViews() const
    {
        return _numViews;
    }

    ID3D11RenderTargetViewPtr D3D11RenderTarget::getRenderTargetViewByIndex(unsigned index /* = 0 */)
    {
        ID3D11RenderTargetViewPtr rt = _renderTargets[index];
        return rt ? rt : nullptr;
    }

    ID3D11Texture2DPtr D3D11RenderTarget::getSurface(unsigned index /* = 0 */)
    {
        ID3D11Texture2DPtr rt = _renderTextures[index];
        return rt ? rt : nullptr;
    }
}


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
}


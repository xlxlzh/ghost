#include "Engine.h"
#include "D3D11RenderSystem.h"

namespace ghost
{
    bool Engine::initEngine(RendersystemType type)
    {
        switch (type)
        {
        case ghost::RENDER_D3D11:
            _renderSystem = std::make_shared<D3D11RenderSystem>();
            break;
        default:
            _renderSystem = std::make_shared<D3D11RenderSystem>();
            break;
        }

        _renderSystem->initRendersystem();

        return true;
    }
}
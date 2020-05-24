#include "Engine.h"
#include "LogManager.h"
#include "D3D11RenderSystem.h"

namespace ghost
{
    bool Engine::initEngine(void* window, RendersystemType type, MSAA msaa, unsigned w, unsigned h)
    {
        LogManager::getInstance()->addLog("GhostDebug.txt");

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
        _window = window;
        _width = w;
        _height = h;

        return true;
    }
}
#include "Engine.h"
#include "LogManager.h"
#include "Resource.h"
#include "D3D11RenderSystem.h"
#include "D3D11Texture2D.h"

namespace ghost
{
    bool Engine::initEngine(void* window, RendersystemType type, MSAA msaa, unsigned w, unsigned h)
    {
        LogManager::getInstance()->addLog("GhostDebug.txt");

        _window = window;
        _width = w;
        _height = h;

        auto resMan = ResourceManager::getInstance();
        switch (type)
        {
        case ghost::RENDER_D3D11:
            _renderDevice = std::make_shared<D3D11RenderDevice>();
            _renderSystem = std::make_shared<D3D11RenderSystem>();

            resMan->registerResourceFactory(new D3D11TextureFactory());
            break;
        default:
            _renderDevice = std::make_shared<D3D11RenderDevice>();
            _renderSystem = std::make_shared<D3D11RenderSystem>();
            break;
        }

        _renderDevice->initDevice(false, msaa);
        _renderSystem->attachRenderDevice(_renderDevice);
        return true;
    }
}
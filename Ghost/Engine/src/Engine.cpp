#include "Engine.h"
#include "LogManager.h"
#include "Resource.h"
#include "D3D11RenderSystem.h"
#include "D3D11Texture2D.h"
#include "FileSystemWin32.h"
#include "Material.h"

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

            resMan->registerResourceFactory(new ResourceFactoryIml<D3D11Texture2D>());

            _fileSystem = std::make_shared<FileSystemWin32>();
            break;
        default:
            _renderDevice = std::make_shared<D3D11RenderDevice>();
            _renderSystem = std::make_shared<D3D11RenderSystem>();
            _fileSystem = std::make_shared<FileSystemWin32>();
            break;
        }

        resMan->registerResourceFactory(new ResourceFactoryIml<Material>());

        _renderDevice->initDevice(false, msaa);
        _renderSystem->attachRenderDevice(_renderDevice);
        return true;
    }
}
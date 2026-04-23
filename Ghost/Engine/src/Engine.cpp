#include "Engine.h"
#include "LogManager.h"
#include "Resource.h"
#include "D3D11RenderSystem.h"
#include "D3D11Texture2D.h"
#include "FileSystemWin32.h"
#include "Material.h"
#include "ShaderResource.h"
#include "Model.h"
#include "GhostConfig.h"

namespace ghost
{
    bool Engine::InitEngine(void* window, RendersystemType type, MSAA msaa, unsigned w, unsigned h)
    {
        _window = window;
        _width = w;
        _height = h;

        auto resMan = ResourceManager::GetInstance();
        switch (type)
        {
        case ghost::RENDER_D3D11:
            _renderDevice = std::make_shared<D3D11RenderDevice>();
            _renderSystem = std::make_shared<D3D11RenderSystem>(_renderDevice);

            resMan->RegisterResourceFactory(new ResourceFactoryIml<D3D11Texture2D>());

            _fileSystem = std::make_shared<FileSystemWin32>();
            break;
        default:
            _renderDevice = std::make_shared<D3D11RenderDevice>();
            _renderSystem = std::make_shared<D3D11RenderSystem>(_renderDevice);
            _fileSystem = std::make_shared<FileSystemWin32>();
            break;
        }

        resMan->RegisterResourceFactory(new ResourceFactoryIml<Material>());
        resMan->RegisterResourceFactory(new ResourceFactoryIml<ShaderResource>());
        resMan->RegisterResourceFactory(new ResourceFactoryIml<Model>());

        _renderDevice->InitDevice(false, GetMSAASampleCount(msaa));
        _renderSystem->InitRenderSystem();
        return true;
    }
}
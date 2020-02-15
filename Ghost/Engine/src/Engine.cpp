#include "Engine.h"
#include "LogManager.h"
#include "D3D11RenderSystem.h"

namespace ghost
{
    bool Engine::initEngine(RendersystemType type, ApplicationPtr app, MSAA msaa)
    {
        LogManager::getInstance()->addLog("GhostDebug.txt");

        ApplicationType appType = app->getApplicationType();

        switch (type)
        {
        case ghost::RENDER_D3D11:
            if (appType != ApplicationType::APP_WIN32)
            {
                LogManager::getInstance()->logError("Platform can not match");
                return false;
            }
            _renderSystem = std::make_shared<D3D11RenderSystem>();
            break;
        default:
            _renderSystem = std::make_shared<D3D11RenderSystem>();
            break;
        }

        _application = app;
        _renderSystem->initRendersystem();

        return true;
    }
}
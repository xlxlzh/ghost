#include "Application.h"
#include "RenderWindowWin32.h"
#include "TimerWin32.h"
#include "Engine.h"
#include "LogManager.h"

namespace ghost
{
    bool Application::initialize(const std::string& wname, const std::string& configFile)
    {
        GhostConfig::getInstance()->loadConfig(configFile);

        const auto& globalConfig = GhostConfig::getInstance()->getGlobalConfig();
        const auto& renderConfig = GhostConfig::getInstance()->getRenderConfig();
        _platformType = globalConfig._platform;

        switch (_platformType)
        {
        case ghost::PLATFORM_WIN32:
            _window = new RenderWindowWin32(this);
            _applicationTimer = std::make_shared<TimerWin32>();
            break;
        default:
            _window = new RenderWindowWin32(this);
            break;
        }

        _initialize = _window->initialize(globalConfig._width, globalConfig._height, wname, globalConfig._fullscreen);

        //Initialize engine
        Engine::getInstance()->initEngine(_window->getWindowHandle(), globalConfig._renderType, renderConfig._msaa, globalConfig._width, globalConfig._height);
        LogManager::getInstance()->addLog(globalConfig._logFile);

        return _initialize;
    }

    float Application::getFPS() const
    {
        if (_msPerFrame != 0.0)
        {
            return 1000.0 / _msPerFrame;
        }

        return 0.0;
    }

	void Application::run()
	{
		if (!_initialize)
			return;

        onInit();

        _applicationTimer->start();
        static float beginTime = _applicationTimer->getElapsedTimeMS();
		while (!_exit)
		{
			_messageLoop();
            onUpdate();

            float currentTime = _applicationTimer->getElapsedTimeMS();
            onTick(currentTime - beginTime);
            _updateFrameTime(currentTime - beginTime);
            beginTime = currentTime;
		}

        onExit();
	}

	void Application::show()
	{
		if (_window)
		{
			_window->updateWindow();
			_window->showWindow();
		}
	}
}
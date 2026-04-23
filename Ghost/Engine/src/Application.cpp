#include "Application.h"
#include "RenderWindowWin32.h"
#include "TimerWin32.h"
#include "Engine.h"
#include "LogManager.h"
#include "SDLRenderWindow.h"

namespace ghost
{
    bool Application::Initialize(const std::string& wname, const std::string& configFile)
    {
        GhostConfig::GetInstance()->LoadConfig(configFile);

        const auto& globalConfig = GhostConfig::GetInstance()->GetGlobalConfig();
        const auto& renderConfig = GhostConfig::GetInstance()->GetRenderConfig();
        _platformType = globalConfig._platform;

        LogManager::GetInstance()->AddLog(globalConfig._logFile);

        switch (_platformType)
        {
        case ghost::PLATFORM_WIN32:
            _window = new SDLRenderWindow(this);
            _applicationTimer = std::make_shared<TimerWin32>();
            break;
        default:
            _window = new RenderWindowWin32(this);
            break;
        }

        _initialize = _window->Initialize(globalConfig._width, globalConfig._height, wname, globalConfig._fullscreen);

        //Initialize engine
        Engine::GetInstance()->InitEngine(_window->GetWindowHandle(), globalConfig._renderType, renderConfig._msaa, globalConfig._width, globalConfig._height);

        return _initialize;
    }

    float Application::GetFPS() const
    {
        if (_msPerFrame != 0.0)
        {
            return 1000.0f / _msPerFrame;
        }

        return 0.0f;
    }

	void Application::Run()
	{
		if (!_initialize)
			return;

        OnInit();

        _applicationTimer->Start();
        static float beginTime = _applicationTimer->GetElapsedTimeMS();
		while (!_exit)
		{
			InternalMessageLoop();
            OnUpdate();

            float currentTime = _applicationTimer->GetElapsedTimeMS();
            OnTick(currentTime - beginTime);
            UpdateFrameTime(currentTime - beginTime);
            beginTime = currentTime;
		}

        OnExit();
	}

	void Application::Show()
	{
		if (_window)
		{
			_window->UpdateWindow();
			_window->ShowWindow();
		}
	}
}
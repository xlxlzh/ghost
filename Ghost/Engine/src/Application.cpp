#include "Application.h"
#include "RenderWindowWin32.h"
#include "TimerWin32.h"

namespace ghost
{
    bool Application::initialize(ApplicationType appType, int w, int h, const std::string& wname, bool fullscreen /* = false */)
    {
        switch (appType)
        {
        case ghost::APP_WIN32:
            _window = new RenderWindowWin32(this);
            _applicationTimer = std::make_shared<TimerWin32>();
            break;
        default:
            _window = new RenderWindowWin32(this);
            break;
        }

        _initialize = _window->initialize(w, h, wname, fullscreen);
        _appType = appType;

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
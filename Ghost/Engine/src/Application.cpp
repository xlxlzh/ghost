#include "Application.h"
#include "RenderWindowWin32.h"

namespace ghost
{
    bool Application::initialize(ApplicationType appType, int w, int h, const std::string& wname, bool fullscreen /* = false */)
    {
        switch (appType)
        {
        case ghost::APP_WIN32:
            _window = new RenderWindowWin32(this);
            break;
        default:
            _window = new RenderWindowWin32(this);
            break;
        }

        _initialize = _window->initialize(w, h, wname, fullscreen);
        _appType = appType;

        return _initialize;
    }

	void Application::run()
	{
		if (!_initialize)
			return;

        onInit();

		while (!_exit)
		{
			_messageLoop();
            onUpdate();
			tick(0.0f);
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
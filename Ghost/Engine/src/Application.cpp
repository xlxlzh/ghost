#include "Application.h"

namespace ghost
{
	void Application::run()
	{
		if (!_initialize)
			return;

		while (!_exit)
		{
			_messageLoop();
			tick(0.0f);
		}
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
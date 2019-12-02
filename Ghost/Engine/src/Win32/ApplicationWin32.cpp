#include "ApplicationWin32.h"
#include "RenderWindowWin32.h"

namespace ghost
{
	bool ApplicationWin32::initialize(int w, int h, const std::string& wname, bool fullscreen)
	{
		_window = new RenderWindowWin32(this);
		if (_window)
		{
			_initialize = _window->initialize(w, h, wname, fullscreen);
			_exit = !_initialize;
			return _initialize;
		}

		return false;
	}
}
#include "RenderWindow.h"

namespace ghost
{
	bool RenderWindow::initialize(int width, int height, const std::string windowTitle, bool fullscreen)
	{
		_width = width;
		_height = height;
		_windowTitle = windowTitle;
		_fullscreen = fullscreen;

		return _createWindow();
	}

	void RenderWindow::showWindow() const
	{
		_showWindow();
	}

	void RenderWindow::updateWindow() const
	{
		_updateWindow();
	}
}
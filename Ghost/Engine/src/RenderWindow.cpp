#include "RenderWindow.h"

namespace ghost
{
	bool RenderWindow::Initialize(int width, int height, const std::string windowTitle, bool fullscreen)
	{
		_width = width;
		_height = height;
		_windowTitle = windowTitle;
		_fullscreen = fullscreen;

		return InternalCreateWindow();
	}

	void RenderWindow::ShowWindow() const
	{
		InternalShowWindow();
	}

	void RenderWindow::UpdateWindow() const
	{
		InternalUpdateWindow();
	}
}
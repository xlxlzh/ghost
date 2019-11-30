#ifndef _RENDER_WINDOW_H_
#define _RENDER_WINDOW_H_

#include <string>

namespace ghost
{
	class RenderWindow
	{
	public:
		bool initialize(int width, int height, const std::string windowName, bool fullscreen);

		int getWidth() const { return _width; }
		int getHeight() const { return _height; }

		const std::string& getWindowTitle() const { return _windowTitle; }

		void showWindow() const;
		void updateWindow() const;

		virtual const void* getWindowHandle() const = 0;

	protected:
		virtual bool _createWindow() = 0;
		virtual void _showWindow() const = 0;
		virtual void _updateWindow() const = 0;

	protected:
		std::string _windowTitle;
		int _width, _height;
		bool _fullscreen;
	};
}

#endif

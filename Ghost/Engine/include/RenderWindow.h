#ifndef _RENDER_WINDOW_H_
#define _RENDER_WINDOW_H_

#include <string>
#include "Ghost.h"

namespace ghost
{
	class Application;

	class GHOST_API RenderWindow
	{
		friend class Application;

	public:
		RenderWindow(Application* app) : _app(app) { }
        virtual ~RenderWindow(){ }

		bool Initialize(int width, int height, const std::string windowName, bool fullscreen);

		int GetWidth() const { return _width; }
		int GetHeight() const { return _height; }

		const std::string& GetWindowTitle() const { return _windowTitle; }

		void ShowWindow() const;
		void UpdateWindow() const;

		virtual void* GetWindowHandle() const = 0;
        virtual void SetWindowTitle(const std::string& title) = 0;
        virtual void SetWindowPos(int posx, int posy) = 0;
        virtual void SetWindowIcon(const std::string& iconName) = 0;

	protected:
		virtual bool InternalCreateWindow() = 0;
		virtual void InternalShowWindow() const = 0;
		virtual void InternalUpdateWindow() const = 0;

		virtual void InternalMessageLoop() { }

	protected:
		std::string _windowTitle;
		int _width, _height;
        int _posx, _posy;
		bool _fullscreen;

		Application* _app;
	};
}

#endif

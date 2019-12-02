#ifndef _RENDER_WINDOW_WIN32_H_
#define _RENDER_WINDOW_WIN32_H_

#include "RenderWindow.h"

#include <windows.h>

namespace ghost
{
	class RenderWindowWin32 : public RenderWindow
	{
	public:
		RenderWindowWin32(Application* app) : RenderWindow(app) { }

		virtual const void* getWindowHandle() const override;

	protected:
		virtual void _showWindow() const override;
		virtual void _updateWindow() const override;
		virtual bool _createWindow() override;

		virtual void _messageLoop() override;

		static LRESULT CALLBACK WndPro(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HWND _windowHandle;
	};
}

#endif

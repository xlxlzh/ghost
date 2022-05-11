#ifndef _RENDER_WINDOW_WIN32_H_
#define _RENDER_WINDOW_WIN32_H_

#include "RenderWindow.h"

#include <windows.h>

namespace ghost
{
	class GHOST_API RenderWindowWin32 : public RenderWindow
	{
	public:
		RenderWindowWin32(Application* app);
        ~RenderWindowWin32();

		virtual void* getWindowHandle() const override;
        virtual void setWindowTitle(const std::string& title) override;
        virtual void setWindowPos(int posx, int posy) override;
        virtual void setWindowIcon() override;

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

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

		virtual void* GetWindowHandle() const override;
        virtual void SetWindowTitle(const std::string& title) override;
        virtual void SetWindowPos(int posx, int posy) override;
        virtual void SetWindowIcon(const std::string& iconName) override;

	protected:
		virtual void InternalShowWindow() const override;
		virtual void InternalUpdateWindow() const override;
		virtual bool InternalCreateWindow() override;

		virtual void InternalMessageLoop() override;

		static LRESULT CALLBACK WndPro(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HWND _windowHandle;
	};
}

#endif

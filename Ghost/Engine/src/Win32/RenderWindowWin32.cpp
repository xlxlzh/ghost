#include "RenderWindowWin32.h"
#include "Application.h"

namespace ghost
{
	static Application* application = nullptr;

	RenderWindowWin32::RenderWindowWin32(Application* app) : RenderWindow(app)
	{
		application = app;
	}

	void* RenderWindowWin32::getWindowHandle() const
	{
		return static_cast<void*>(_windowHandle);
	}

    void RenderWindowWin32::setWindowTitle(const std::string& title)
    {
        _windowTitle = title;
        ::SetWindowText(_windowHandle, _windowTitle.c_str());
    }

	bool RenderWindowWin32::_createWindow()
	{
		HINSTANCE hIn = ::GetModuleHandle(nullptr);

		WNDCLASSEX wndex;
		wndex.cbClsExtra = 0;
		wndex.cbSize = sizeof(WNDCLASSEX);
		wndex.cbWndExtra = 0;
		wndex.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
		wndex.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
		wndex.hIcon = ::LoadIcon(nullptr, IDI_APPLICATION);
		wndex.hIconSm = nullptr;
		wndex.hInstance = hIn;
		wndex.lpfnWndProc = WndPro;
		wndex.lpszClassName = "RenderWindowWin32";
		wndex.lpszMenuName = nullptr;
		wndex.style = CS_HREDRAW | CS_VREDRAW;

		if (!RegisterClassEx(&wndex))
		{
			::MessageBox(nullptr, "RenderWindow register failed.", "Error", MB_OK);
			return false;
		}

        RECT rect = { 0.0, 0.0, _width, _height};
        ::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

		_windowHandle = ::CreateWindowEx(0, "RenderWindowWin32", _windowTitle.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hIn, nullptr);
		if (!_windowHandle)
		{
			::MessageBox(nullptr, "RenderWindow create failed.", "Error", MB_OK);
			return false;
		}

		return true;
	}

	LRESULT CALLBACK RenderWindowWin32::WndPro(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_KEYDOWN:
			application->onKeydown();
			switch (wParam)
			{
			case VK_ESCAPE:
				application->exitApplication();
				break;
			}
			break;

		case WM_QUIT:
			break;

		case WM_DESTROY:
			application->exitApplication();
			::DestroyWindow((HWND)application->getAttachWindow());
			break;
		}
		return ::DefWindowProc(hWnd, message, wParam, lParam);
	}

	void RenderWindowWin32::_showWindow() const
	{
		if (_windowHandle)
		{
			::ShowWindow(_windowHandle, SW_SHOW);
		}
	}

	void RenderWindowWin32::_updateWindow() const
	{
		if (_windowHandle)
		{
			::UpdateWindow(_windowHandle);
		}
	}

	void RenderWindowWin32::_messageLoop()
	{
		MSG msg;
		memset(&msg, 0, sizeof(msg));

		if (::PeekMessage(&msg, _windowHandle, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
}
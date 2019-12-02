#include "RenderWindowWin32.h"

namespace ghost
{
	const void* RenderWindowWin32::getWindowHandle() const
	{
		return static_cast<void*>(_windowHandle);
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

		_windowHandle = ::CreateWindowEx(0, "RenderWindowWin32", _windowTitle.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			_width, _height, nullptr, nullptr, hIn, nullptr);
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
			switch (wParam)
			{
			case VK_ESCAPE:
				break;
			}
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

		::DispatchMessage(&msg);
		::TranslateMessage(&msg);
	}
}
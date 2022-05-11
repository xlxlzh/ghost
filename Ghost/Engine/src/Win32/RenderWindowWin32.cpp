#include "RenderWindowWin32.h"
#include "Application.h"

//Imgui
#include "imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam);

namespace ghost
{
	static Application* application = nullptr;

	RenderWindowWin32::RenderWindowWin32(Application* app) : RenderWindow(app)
	{
		application = app;
	}

    RenderWindowWin32::~RenderWindowWin32()
    {
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
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

    void RenderWindowWin32::setWindowPos(int posx, int posy)
    {
        //TODO
    }

    void RenderWindowWin32::setWindowIcon()
    {
        //TODO
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

        //IMGUI
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui_ImplWin32_Init(_windowHandle);
        ImGui_ImplWin32_EnableDpiAwareness();

        ImGui::StyleColorsDark();

		return true;
	}

	LRESULT CALLBACK RenderWindowWin32::WndPro(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
        HRESULT hr = ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);

		switch (message)
		{
		case WM_KEYDOWN:
			application->onKeydown(GhostKey::GHOST_KEY_NONE);
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

        default:
            hr = ::DefWindowProc(hWnd, message, wParam, lParam);
		}


        return hr;
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
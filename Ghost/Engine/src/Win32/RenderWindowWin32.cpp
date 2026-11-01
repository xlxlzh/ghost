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
	static GhostKey Win32KeyToGhostKey(WPARAM wParam)
	{
		switch (wParam)
		{
		case VK_TAB: return GhostKey::GHOST_KEY_TAB;
		case VK_LEFT: return GhostKey::GHOST_KEY_LEFTARROW;
		case VK_RIGHT: return GhostKey::GHOST_KEY_RIGHTARROW;
		case VK_UP: return GhostKey::GHOST_KEY_UPARROW;
		case VK_DOWN: return GhostKey::GHOST_KEY_DOWNARROW;
		case VK_PRIOR: return GhostKey::GHOST_KEY_PAGEUP;
		case VK_NEXT: return GhostKey::GHOST_KEY_PAGEDOWN;
		case VK_HOME: return GhostKey::GHOST_KEY_HOME;
		case VK_END: return GhostKey::GHOST_KEY_END;
		case VK_INSERT: return GhostKey::GHOST_KEY_INSERT;
		case VK_DELETE: return GhostKey::GHOST_KEY_DELETE;
		case VK_BACK: return GhostKey::GHOST_KEY_BACKSPACE;
		case VK_SPACE: return GhostKey::GHOST_KEY_SPACE;
		case VK_RETURN: return GhostKey::GHOST_KEY_ENTER;
		case VK_ESCAPE: return GhostKey::GHOST_KEY_ESCAPE;
		case VK_LCONTROL: return GhostKey::GHOST_KEY_LEFTCTRL;
		case VK_LSHIFT: return GhostKey::GHOST_KEY_LEFTSHIFT;
		case VK_LMENU: return GhostKey::GHOST_KEY_LEFTALT;
		case VK_LWIN: return GhostKey::GHOST_KEY_LEFTSUPER;
		case VK_RCONTROL: return GhostKey::GHOST_KEY_RIGHTCTRL;
		case VK_RSHIFT: return GhostKey::GHOST_KEY_RIGHTSHIFT;
		case VK_RMENU: return GhostKey::GHOST_KEY_RIGHTALT;
		case VK_RWIN: return GhostKey::GHOST_KEY_RIGHTSUPER;
		case VK_APPS: return GhostKey::GHOST_KEY_MENU;
		case '0': return GhostKey::GHOST_KEY_0;
		case '1': return GhostKey::GHOST_KEY_1;
		case '2': return GhostKey::GHOST_KEY_2;
		case '3': return GhostKey::GHOST_KEY_3;
		case '4': return GhostKey::GHOST_KEY_4;
		case '5': return GhostKey::GHOST_KEY_5;
		case '6': return GhostKey::GHOST_KEY_6;
		case '7': return GhostKey::GHOST_KEY_7;
		case '8': return GhostKey::GHOST_KEY_8;
		case '9': return GhostKey::GHOST_KEY_9;
		case 'A': return GhostKey::GHOST_KEY_A;
		case 'B': return GhostKey::GHOST_KEY_B;
		case 'C': return GhostKey::GHOST_KEY_C;
		case 'D': return GhostKey::GHOST_KEY_D;
		case 'E': return GhostKey::GHOST_KEY_E;
		case 'F': return GhostKey::GHOST_KEY_F;
		case 'G': return GhostKey::GHOST_KEY_G;
		case 'H': return GhostKey::GHOST_KEY_H;
		case 'I': return GhostKey::GHOST_KEY_I;
		case 'J': return GhostKey::GHOST_KEY_J;
		case 'K': return GhostKey::GHOST_KEY_K;
		case 'L': return GhostKey::GHOST_KEY_L;
		case 'M': return GhostKey::GHOST_KEY_M;
		case 'N': return GhostKey::GHOST_KEY_N;
		case 'O': return GhostKey::GHOST_KEY_O;
		case 'P': return GhostKey::GHOST_KEY_P;
		case 'Q': return GhostKey::GHOST_KEY_Q;
		case 'R': return GhostKey::GHOST_KEY_R;
		case 'S': return GhostKey::GHOST_KEY_S;
		case 'T': return GhostKey::GHOST_KEY_T;
		case 'U': return GhostKey::GHOST_KEY_U;
		case 'V': return GhostKey::GHOST_KEY_V;
		case 'W': return GhostKey::GHOST_KEY_W;
		case 'X': return GhostKey::GHOST_KEY_X;
		case 'Y': return GhostKey::GHOST_KEY_Y;
		case 'Z': return GhostKey::GHOST_KEY_Z;
		case VK_F1: return GhostKey::GHOST_KEY_F1;
		case VK_F2: return GhostKey::GHOST_KEY_F2;
		case VK_F3: return GhostKey::GHOST_KEY_F3;
		case VK_F4: return GhostKey::GHOST_KEY_F4;
		case VK_F5: return GhostKey::GHOST_KEY_F5;
		case VK_F6: return GhostKey::GHOST_KEY_F6;
		case VK_F7: return GhostKey::GHOST_KEY_F7;
		case VK_F8: return GhostKey::GHOST_KEY_F8;
		case VK_F9: return GhostKey::GHOST_KEY_F9;
		case VK_F10: return GhostKey::GHOST_KEY_F10;
		case VK_F11: return GhostKey::GHOST_KEY_F11;
		case VK_F12: return GhostKey::GHOST_KEY_F12;
		case VK_OEM_7: return GhostKey::GHOST_KEY_APOSTROPHE;
		case VK_OEM_COMMA: return GhostKey::GHOST_KEY_COMMA;
		case VK_OEM_MINUS: return GhostKey::GHOST_KEY_MINUS;
		case VK_OEM_PERIOD: return GhostKey::GHOST_KEY_PERIOD;
		case VK_OEM_2: return GhostKey::GHOST_KEY_SLASH;
		case VK_OEM_1: return GhostKey::GHOST_KEY_SEMICOLON;
		case VK_OEM_PLUS: return GhostKey::GHOST_KEY_EQUAL;
		case VK_OEM_4: return GhostKey::GHOST_KEY_LEFTBRACKET;
		case VK_OEM_5: return GhostKey::GHOST_KEY_BACKSLASH;
		case VK_OEM_6: return GhostKey::GHOST_KEY_RIGHTBRACKET;
		case VK_OEM_3: return GhostKey::GHOST_KEY_GRAVEACCENT;
		case VK_CAPITAL: return GhostKey::GHOST_KEY_CAPSLOCK;
		case VK_SCROLL: return GhostKey::GHOST_KEY_SCROLLLOCK;
		case VK_NUMLOCK: return GhostKey::GHOST_KEY_NUMLOCK;
		case VK_SNAPSHOT: return GhostKey::GHOST_KEY_PRINTSCREEN;
		case VK_PAUSE: return GhostKey::GHOST_KEY_PAUSE;
		case VK_NUMPAD0: return GhostKey::GHOST_KEY_KEYPAD0;
		case VK_NUMPAD1: return GhostKey::GHOST_KEY_KEYPAD1;
		case VK_NUMPAD2: return GhostKey::GHOST_KEY_KEYPAD2;
		case VK_NUMPAD3: return GhostKey::GHOST_KEY_KEYPAD3;
		case VK_NUMPAD4: return GhostKey::GHOST_KEY_KEYPAD4;
		case VK_NUMPAD5: return GhostKey::GHOST_KEY_KEYPAD5;
		case VK_NUMPAD6: return GhostKey::GHOST_KEY_KEYPAD6;
		case VK_NUMPAD7: return GhostKey::GHOST_KEY_KEYPAD7;
		case VK_NUMPAD8: return GhostKey::GHOST_KEY_KEYPAD8;
		case VK_NUMPAD9: return GhostKey::GHOST_KEY_KEYPAD9;
		case VK_DECIMAL: return GhostKey::GHOST_KEY_KEYPADDECIMAL;
		case VK_DIVIDE: return GhostKey::GHOST_KEY_KEYPADDIVIDE;
		case VK_MULTIPLY: return GhostKey::GHOST_KEY_KEYPADMULTIPLY;
		case VK_SUBTRACT: return GhostKey::GHOST_KEY_KEYPADSUBTRACT;
		case VK_ADD: return GhostKey::GHOST_KEY_KEYPADADD;
		case VK_OEM_NEC_EQUAL: return GhostKey::GHOST_KEY_KEYPADEQUAL;
		}

		return GhostKey::GHOST_KEY_NONE;
	}
	
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

	void* RenderWindowWin32::GetWindowHandle() const
	{
		return static_cast<void*>(_windowHandle);
	}

    void RenderWindowWin32::ImGuiNewFrame()
    {
        ImGui_ImplWin32_NewFrame();
    }

    void RenderWindowWin32::SetWindowTitle(const std::string& title)
    {
        _windowTitle = title;
        ::SetWindowText(_windowHandle, _windowTitle.c_str());
    }

    void RenderWindowWin32::SetWindowPos(int posx, int posy)
    {
        //TODO
    }

    void RenderWindowWin32::SetWindowIcon(const std::string& iconName)
    {
        //TODO
    }

	bool RenderWindowWin32::InternalCreateWindow()
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

        RECT rect = { 0, 0, _width, _height};
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
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

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
			application->OnKeydown(Win32KeyToGhostKey(wParam));
			break;

		case WM_KEYUP:
			application->OnKeyup(Win32KeyToGhostKey(wParam));
			break;
			
		case WM_SIZE:
		{
			int32_t width = LOWORD(lParam);
			int32_t height = HIWORD(lParam);
			application->OnResize(width, height);
			break;
		}

			
		case WM_LBUTTONDOWN:
			application->OnMouseLeftClick();
			break;
			
		case WM_LBUTTONUP:
			application->OnMouseLeftRelease();
			break;
			
		case WM_RBUTTONDOWN:
			application->OnMouseRightClick();
			break;
			
		case WM_RBUTTONUP:
			application->OnMouseRightRelease();
			break;
			
		case WM_MOUSEMOVE:
			application->OnMouseMove();
			break;

		case WM_QUIT:
		case WM_DESTROY:
			application->ExitApplication();
			::DestroyWindow((HWND)application->GetAttachWindow());
			break;

        default:
            hr = ::DefWindowProc(hWnd, message, wParam, lParam);
		}


        return hr;
	}

	void RenderWindowWin32::InternalShowWindow() const
	{
		if (_windowHandle)
		{
			::ShowWindow(_windowHandle, SW_SHOW);
		}
	}

	void RenderWindowWin32::InternalUpdateWindow() const
	{
		if (_windowHandle)
		{
			::UpdateWindow(_windowHandle);
		}
	}

	void RenderWindowWin32::InternalMessageLoop()
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
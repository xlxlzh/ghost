#include "SDLRenderWindow.h"
#include "SDL_syswm.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_sdl.h"

namespace ghost
{
    static HWND GetWindowHwnd(SDL_Window* w)
    {
        SDL_SysWMinfo sysInfo;

        SDL_VERSION(&sysInfo.version);
        SDL_GetWindowWMInfo(w, &sysInfo);

        return sysInfo.info.win.window;
    }

    SDLRenderWindow::~SDLRenderWindow()
    {
        _destroySDL();
    }

    bool SDLRenderWindow::_createWindow()
    {
        if (!_initSDL())
            return false;

        //IMGUI
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui_ImplWin32_Init(GetWindowHwnd(_window));
        ImGui_ImplWin32_EnableDpiAwareness();

        ImGui::StyleColorsDark();

        return true;
    }

    void* SDLRenderWindow::getWindowHandle() const
    {
        SDL_SysWMinfo sysInfo;
        
        SDL_VERSION(&sysInfo.version);
        SDL_GetWindowWMInfo(_window, &sysInfo);
        
        return sysInfo.info.win.window;
    }
    
    void SDLRenderWindow::setWindowTitle(const std::string& title)
    {
        //TODO
    }

    bool SDLRenderWindow::_initSDL()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
            return false;

        _window = SDL_CreateWindow(_windowTitle.c_str(), 0, 0, _width, _height, 0);

        //Now just support D3D
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplSDL2_InitForD3D(_window);
        
        return true;
    }

    void SDLRenderWindow::_destroySDL()
    {
        SDL_DestroyWindow(_window);
        SDL_Quit();
    }

    void SDLRenderWindow::_showWindow() const
    {

    }

    void SDLRenderWindow::_updateWindow() const
    {

    }

    void SDLRenderWindow::_messageLoop()
    {
        if (SDL_PollEvent(&_event))
        {
            ImGui_ImplSDL2_ProcessEvent(&_event);
            if (_event.type == SDL_KEYDOWN)
            {
                _app->onKeydown(Key::KEY_NONE);
            }
            else if (_event.type == SDL_KEYUP)
            {
                _app->onKeyup((Key::KEY_NONE));
            }
        }
    }
}
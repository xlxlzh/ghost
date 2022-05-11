#include "SDLRenderWindow.h"
#include "SDL_syswm.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_sdl.h"
#include "MessageHandler.h"

namespace ghost
{
    static GhostKey SDL2_KeycodeToGhostKey(int keycode)
    {
        switch (keycode)
        {
        case SDLK_TAB: return GhostKey::GHOST_KEY_TAB;
        case SDLK_LEFT: return GhostKey::GHOST_KEY_LEFTARROW;
        case SDLK_RIGHT: return GhostKey::GHOST_KEY_RIGHTARROW;
        case SDLK_UP: return GhostKey::GHOST_KEY_UPARROW;
        case SDLK_DOWN: return GhostKey::GHOST_KEY_DOWNARROW;
        case SDLK_PAGEUP: return GhostKey::GHOST_KEY_PAGEUP;
        case SDLK_PAGEDOWN: return GhostKey::GHOST_KEY_PAGEDOWN;
        case SDLK_HOME: return GhostKey::GHOST_KEY_HOME;
        case SDLK_END: return GhostKey::GHOST_KEY_END;
        case SDLK_INSERT: return GhostKey::GHOST_KEY_INSERT;
        case SDLK_DELETE: return GhostKey::GHOST_KEY_DELETE;
        case SDLK_BACKSPACE: return GhostKey::GHOST_KEY_BACKSPACE;
        case SDLK_SPACE: return GhostKey::GHOST_KEY_SPACE;
        case SDLK_RETURN: return GhostKey::GHOST_KEY_ENTER;
        case SDLK_ESCAPE: return GhostKey::GHOST_KEY_ESCAPE;
        case SDLK_QUOTE: return GhostKey::GHOST_KEY_APOSTROPHE;
        case SDLK_COMMA: return GhostKey::GHOST_KEY_COMMA;
        case SDLK_MINUS: return GhostKey::GHOST_KEY_MINUS;
        case SDLK_PERIOD: return GhostKey::GHOST_KEY_PERIOD;
        case SDLK_SLASH: return GhostKey::GHOST_KEY_SLASH;
        case SDLK_SEMICOLON: return GhostKey::GHOST_KEY_SEMICOLON;
        case SDLK_EQUALS: return GhostKey::GHOST_KEY_EQUAL;
        case SDLK_LEFTBRACKET: return GhostKey::GHOST_KEY_LEFTBRACKET;
        case SDLK_BACKSLASH: return GhostKey::GHOST_KEY_BACKSLASH;
        case SDLK_RIGHTBRACKET: return GhostKey::GHOST_KEY_RIGHTBRACKET;
        case SDLK_BACKQUOTE: return GhostKey::GHOST_KEY_GRAVEACCENT;
        case SDLK_CAPSLOCK: return GhostKey::GHOST_KEY_CAPSLOCK;
        case SDLK_SCROLLLOCK: return GhostKey::GHOST_KEY_SCROLLLOCK;
        case SDLK_NUMLOCKCLEAR: return GhostKey::GHOST_KEY_NUMLOCK;
        case SDLK_PRINTSCREEN: return GhostKey::GHOST_KEY_PRINTSCREEN;
        case SDLK_PAUSE: return GhostKey::GHOST_KEY_PAUSE;
        case SDLK_KP_0: return GhostKey::GHOST_KEY_KEYPAD0;
        case SDLK_KP_1: return GhostKey::GHOST_KEY_KEYPAD1;
        case SDLK_KP_2: return GhostKey::GHOST_KEY_KEYPAD2;
        case SDLK_KP_3: return GhostKey::GHOST_KEY_KEYPAD3;
        case SDLK_KP_4: return GhostKey::GHOST_KEY_KEYPAD4;
        case SDLK_KP_5: return GhostKey::GHOST_KEY_KEYPAD5;
        case SDLK_KP_6: return GhostKey::GHOST_KEY_KEYPAD6;
        case SDLK_KP_7: return GhostKey::GHOST_KEY_KEYPAD7;
        case SDLK_KP_8: return GhostKey::GHOST_KEY_KEYPAD8;
        case SDLK_KP_9: return GhostKey::GHOST_KEY_KEYPAD9;
        case SDLK_KP_PERIOD: return GhostKey::GHOST_KEY_KEYPADDECIMAL;
        case SDLK_KP_DIVIDE: return GhostKey::GHOST_KEY_KEYPADDIVIDE;
        case SDLK_KP_MULTIPLY: return GhostKey::GHOST_KEY_KEYPADMULTIPLY;
        case SDLK_KP_MINUS: return GhostKey::GHOST_KEY_KEYPADSUBTRACT;
        case SDLK_KP_PLUS: return GhostKey::GHOST_KEY_KEYPADADD;
        case SDLK_KP_ENTER: return GhostKey::GHOST_KEY_KEYPADENTER;
        case SDLK_KP_EQUALS: return GhostKey::GHOST_KEY_KEYPADEQUAL;
        case SDLK_LCTRL: return GhostKey::GHOST_KEY_LEFTCTRL;
        case SDLK_LSHIFT: return GhostKey::GHOST_KEY_LEFTSHIFT;
        case SDLK_LALT: return GhostKey::GHOST_KEY_LEFTALT;
        case SDLK_LGUI: return GhostKey::GHOST_KEY_LEFTSUPER;
        case SDLK_RCTRL: return GhostKey::GHOST_KEY_RIGHTCTRL;
        case SDLK_RSHIFT: return GhostKey::GHOST_KEY_RIGHTSHIFT;
        case SDLK_RALT: return GhostKey::GHOST_KEY_RIGHTALT;
        case SDLK_RGUI: return GhostKey::GHOST_KEY_RIGHTSUPER;
        case SDLK_APPLICATION: return GhostKey::GHOST_KEY_MENU;
        case SDLK_0: return GhostKey::GHOST_KEY_0;
        case SDLK_1: return GhostKey::GHOST_KEY_1;
        case SDLK_2: return GhostKey::GHOST_KEY_2;
        case SDLK_3: return GhostKey::GHOST_KEY_3;
        case SDLK_4: return GhostKey::GHOST_KEY_4;
        case SDLK_5: return GhostKey::GHOST_KEY_5;
        case SDLK_6: return GhostKey::GHOST_KEY_6;
        case SDLK_7: return GhostKey::GHOST_KEY_7;
        case SDLK_8: return GhostKey::GHOST_KEY_8;
        case SDLK_9: return GhostKey::GHOST_KEY_9;
        case SDLK_a: return GhostKey::GHOST_KEY_A;
        case SDLK_b: return GhostKey::GHOST_KEY_B;
        case SDLK_c: return GhostKey::GHOST_KEY_C;
        case SDLK_d: return GhostKey::GHOST_KEY_D;
        case SDLK_e: return GhostKey::GHOST_KEY_E;
        case SDLK_f: return GhostKey::GHOST_KEY_F;
        case SDLK_g: return GhostKey::GHOST_KEY_G;
        case SDLK_h: return GhostKey::GHOST_KEY_H;
        case SDLK_i: return GhostKey::GHOST_KEY_I;
        case SDLK_j: return GhostKey::GHOST_KEY_J;
        case SDLK_k: return GhostKey::GHOST_KEY_K;
        case SDLK_l: return GhostKey::GHOST_KEY_L;
        case SDLK_m: return GhostKey::GHOST_KEY_M;
        case SDLK_n: return GhostKey::GHOST_KEY_N;
        case SDLK_o: return GhostKey::GHOST_KEY_O;
        case SDLK_p: return GhostKey::GHOST_KEY_P;
        case SDLK_q: return GhostKey::GHOST_KEY_Q;
        case SDLK_r: return GhostKey::GHOST_KEY_R;
        case SDLK_s: return GhostKey::GHOST_KEY_S;
        case SDLK_t: return GhostKey::GHOST_KEY_T;
        case SDLK_u: return GhostKey::GHOST_KEY_U;
        case SDLK_v: return GhostKey::GHOST_KEY_V;
        case SDLK_w: return GhostKey::GHOST_KEY_W;
        case SDLK_x: return GhostKey::GHOST_KEY_X;
        case SDLK_y: return GhostKey::GHOST_KEY_Y;
        case SDLK_z: return GhostKey::GHOST_KEY_Z;
        case SDLK_F1: return GhostKey::GHOST_KEY_F1;
        case SDLK_F2: return GhostKey::GHOST_KEY_F2;
        case SDLK_F3: return GhostKey::GHOST_KEY_F3;
        case SDLK_F4: return GhostKey::GHOST_KEY_F4;
        case SDLK_F5: return GhostKey::GHOST_KEY_F5;
        case SDLK_F6: return GhostKey::GHOST_KEY_F6;
        case SDLK_F7: return GhostKey::GHOST_KEY_F7;
        case SDLK_F8: return GhostKey::GHOST_KEY_F8;
        case SDLK_F9: return GhostKey::GHOST_KEY_F9;
        case SDLK_F10: return GhostKey::GHOST_KEY_F10;
        case SDLK_F11: return GhostKey::GHOST_KEY_F11;
        case SDLK_F12: return GhostKey::GHOST_KEY_F12;
        }

        return GhostKey::GHOST_KEY_NONE;
    }

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

        //Now just support D3D
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplSDL2_InitForD3D(_window);

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
        SDL_SetWindowTitle(_window, title.c_str());
    }

    void SDLRenderWindow::setWindowPos(int posx, int posy)
    {
        SDL_SetWindowPosition(_window, posx, posy);
    }

    void SDLRenderWindow::setWindowIcon()
    {

    }

    bool SDLRenderWindow::_initSDL()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
            return false;

        _window = SDL_CreateWindow(_windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        
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
            
            switch (_event.type)
            {
                case SDL_KEYDOWN:
                {
                    _app->onKeydown(SDL2_KeycodeToGhostKey(_event.key.keysym.sym));
                    break;
                }

                case SDL_KEYUP:
                {
                    _app->onKeyup(SDL2_KeycodeToGhostKey(_event.key.keysym.sym));
                    break;
                }

                case SDL_MOUSEMOTION:
                {
                    _app->onMouseMove();
                    break;
                }

                case SDL_MOUSEBUTTONDOWN:
                {
                    if (_event.button.button == SDL_BUTTON_LEFT)
                        _app->onMouseLeftClick();
                    else if (_event.button.button == SDL_BUTTON_RIGHT)
                        _app->onMouseRightClick();
                    break;
                }

                case SDL_MOUSEBUTTONUP:
                {
                    if (_event.button.button == SDL_BUTTON_LEFT)
                        _app->onMouseLeftRelease();
                    else if (_event.button.button == SDL_BUTTON_RIGHT)
                        _app->onMouseRightRelease();
                    break;
                }

                default:
                {
                    if (_event.type == SDL_QUIT
                        || (_event.type == SDL_WINDOWEVENT && _event.window.event == SDL_WINDOWEVENT_CLOSE))
                    {
                        _app->exitApplication();
                    }

                    break;
                }
            }
        }
    }
}
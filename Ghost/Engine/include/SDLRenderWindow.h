#ifndef _RENDERWINDOW_SDL_H_
#define _RENDERWINDOW_SDL_H_

#include "Ghost.h"
#include "RenderWindow.h"
#include "SDL.h"
#include "Application.h"

namespace ghost
{
    class GHOST_API SDLRenderWindow final : public RenderWindow
    {
    public:
        SDLRenderWindow(Application* app) : RenderWindow(app) { }
        ~SDLRenderWindow();

        virtual void* GetWindowHandle() const override;
        virtual void SetWindowTitle(const std::string& title) override;
        virtual void SetWindowPos(int posx, int posy) override;
        virtual void SetWindowIcon(const std::string& iconName) override;

    protected:
        virtual bool InternalCreateWindow() override;
        virtual void InternalShowWindow() const override;
        virtual void InternalUpdateWindow() const override;

        virtual void InternalMessageLoop() override;

        bool InitSDL();
        void DestroySDL();

    protected:
        SDL_Window* _window{ nullptr };
        SDL_Surface* _icon{ nullptr };
        SDL_Event _event;
    };
}

#endif

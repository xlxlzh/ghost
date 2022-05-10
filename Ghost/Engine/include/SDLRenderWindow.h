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

        virtual void* getWindowHandle() const override;
        virtual void setWindowTitle(const std::string& title) override;

    protected:
        virtual bool _createWindow() override;
        virtual void _showWindow() const override;
        virtual void _updateWindow() const override;

        virtual void _messageLoop() override;

        bool _initSDL();
        void _destroySDL();

    protected:
        SDL_Window* _window;
        SDL_Event _event;
    };
}

#endif

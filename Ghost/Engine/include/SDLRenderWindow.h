#ifndef _RENDERWINDOW_SDL_H_
#define _RENDERWINDOW_SDL_H_

#include "Ghost.h"
#include "RenderWindow.h"
#include "SDL.h"

namespace ghost
{
    class GHOST_API SDLRenderWindow final : public RenderWindow
    {
    public:
        SDLRenderWindow(Application* app) : RenderWindow(app) { }
        ~SDLRenderWindow() { }

    protected:
        virtual bool _createWindow() override;
        virtual void _showWindow() const override;
        virtual void _updateWindow() const override;

        virtual void _messageLoop() override;

    protected:

    };
}

#endif

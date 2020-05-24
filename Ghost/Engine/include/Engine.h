#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "Application.h"
#include "RenderSystem.h"

namespace ghost
{
    class GHOST_API Engine : public SingleTon<Engine>
    {
    public:
        bool initEngine(void* window, RendersystemType type, MSAA msaa, unsigned w, unsigned h);

        RenderSystemPtr getRenderSystem() const { return _renderSystem; }

        void* getWindow() const { return _window; }
        unsigned getWidth() const { return _width; }
        unsigned getHeight() const { return _height; }

    private:
        RenderSystemPtr _renderSystem;
        void* _window;
        unsigned _width, _height;
    };
}

#endif

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "Application.h"
#include "RenderSystem.h"

namespace ghost
{
    class GHOST_API Engine : public SingleTon<Engine>
    {
    public:
        bool initEngine(RendersystemType type, ApplicationPtr app, MSAA msaa);

        RenderSystemPtr getRenderSystem() const { return _renderSystem; }
        ApplicationPtr getApplication() const { return _application; }

    private:
        RenderSystemPtr _renderSystem;
        ApplicationPtr _application;
    };
}

#endif

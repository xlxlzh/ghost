#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "Application.h"
#include "RenderSystem.h"
#include "FileSystem.h"

namespace ghost
{
    class GHOST_API Engine : public SingleTon<Engine>
    {
    public:
        bool InitEngine(void* window, RendersystemType type, MSAA msaa, unsigned w, unsigned h);

        RenderSystemPtr GetRenderSystem() const { return _renderSystem; }
        RenderDevicePtr GetRenderDevice() const { return _renderDevice; }

        FileSystemPtr GetFileSystem() const { return _fileSystem; }

        void* GetWindow() const { return _window; }
        unsigned GetWidth() const { return _width; }
        unsigned GetHeight() const { return _height; }

        unsigned _GetMsaaCount(MSAA msaa)
        {
            switch (msaa)
            {
            case ghost::_2x:
                return 2;
            case ghost::_4x:
                return 4;
            case ghost::_8x:
                return 8;
            default:
                return 1;
            }
        }

    private:
        RenderSystemPtr _renderSystem;
        RenderDevicePtr _renderDevice;
        void* _window;
        unsigned _width, _height;

        FileSystemPtr _fileSystem;
    };
}

#endif

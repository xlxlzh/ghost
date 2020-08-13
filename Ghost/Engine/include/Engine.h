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
        bool initEngine(void* window, RendersystemType type, MSAA msaa, unsigned w, unsigned h);

        RenderSystemPtr getRenderSystem() const { return _renderSystem; }
        RenderDevicePtr getRenderDevice() const { return _renderDevice; }

        FileSystemPtr getFileSystem() const { return _fileSystem; }

        void* getWindow() const { return _window; }
        unsigned getWidth() const { return _width; }
        unsigned getHeight() const { return _height; }

        unsigned _getMsaaCount(MSAA msaa)
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

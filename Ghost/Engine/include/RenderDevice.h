#ifndef _RENDERDEVICE_H_
#define _RENDERDEVICE_H_

#include <memory>
#include "Ghost.h"

namespace ghost
{
    class GHOST_API RenderDevice
    {
    public:
        virtual bool initDevice(bool fullscreen, unsigned msaaCount) = 0;
        virtual bool checkSampleCount(unsigned checkCount) = 0;

    protected:
        unsigned _sampleCount;
        unsigned _sampleQulity;

        unsigned _width, _height;
        bool _fullscreen;
    };

    using RenderDevicePtr = std::shared_ptr<RenderDevice>;
}

#endif

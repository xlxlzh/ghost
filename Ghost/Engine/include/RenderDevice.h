#ifndef _RENDERDEVICE_H_
#define _RENDERDEVICE_H_

#include <memory>
#include "Ghost.h"
#include "ShaderResource.h"

namespace ghost
{
    class Shader;

    class GHOST_API RenderDevice
    {
    public:
        virtual bool initDevice(bool fullscreen, unsigned msaaCount) = 0;
        virtual bool checkSampleCount(unsigned checkCount) = 0;
        
        virtual bool compileShader(ShaderType type, const char* entry, const std::unordered_map<std::string, std::string>& defines, ShaderResource& shader) = 0;
        virtual Shader* createShader(const ShaderResource* shadersRes) = 0;

    protected:
        unsigned _sampleCount;
        unsigned _sampleQulity;

        unsigned _width, _height;
        bool _fullscreen;
    };

    DECLAR_SMART_POINTER(RenderDevice)
}

#endif

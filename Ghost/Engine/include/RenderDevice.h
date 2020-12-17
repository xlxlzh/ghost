#ifndef _RENDERDEVICE_H_
#define _RENDERDEVICE_H_

#include <memory>
#include "Ghost.h"
#include "ShaderResource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstBuffer.h"

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

        virtual VertexBufferPtr createVertexBuffer(unsigned VertexSize, unsigned numVertices, BufferUsage usage) = 0;
        virtual IndexBufferPtr createIndexBuffer(IndexBuffer::IndexType iType, unsigned numIndexes, BufferUsage usage) = 0;
        virtual ConstBufferPtr createConstBuffer(unsigned bufferSize, BufferUsage usage, const std::string& name) = 0;

    protected:
        unsigned _sampleCount;
        unsigned _sampleQulity;

        unsigned _width, _height;
        bool _fullscreen;
    };

    DECLAR_SMART_POINTER(RenderDevice)
}

#endif

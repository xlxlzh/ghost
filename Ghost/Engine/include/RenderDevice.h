#ifndef _RENDERDEVICE_H_
#define _RENDERDEVICE_H_

#include <memory>
#include "Ghost.h"
#include "ShaderResource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstBuffer.h"
#include "VertexDeclaration.h"
#include "Material.h"
#include "RenderTarget.h"

namespace ghost
{
    class Shader;

    class GHOST_API RenderDevice
    {
    public:
        virtual bool initDevice(bool fullscreen, unsigned msaaCount) = 0;
        virtual bool checkSampleCount(unsigned checkCount) = 0;
        
        virtual bool compileShader(ShaderType type, const char* entry, const std::unordered_map<std::string, std::string>& defines, ShaderResource& shader) = 0;
        virtual Shader* createShader(const ShaderResourcePtr& shadersRes) = 0;
        virtual void reflectShader(const ShaderResourcePtr& shadersRes, ShaderParamsList& params) = 0;

        virtual VertexBufferPtr createVertexBuffer(unsigned VertexSize, unsigned numVertices, BufferUsage usage) = 0;
        virtual IndexBufferPtr createIndexBuffer(IndexBuffer::IndexType iType, unsigned numIndexes, BufferUsage usage) = 0;
        virtual ConstBufferPtr createConstBuffer(unsigned bufferSize, BufferUsage usage, const std::string& name) = 0;
        virtual VertexDeclarationPtr createVertexDeclaration() = 0;

        virtual RenderTargetPtr createRenderTargets(unsigned w, unsigned h, unsigned numRTS, GhostColorFormat* formats, bool srv = false, bool msaa = false, bool depth = true) = 0;
        virtual RenderTargetPtr createSingleRenderTarget(unsigned w, unsigned h, GhostColorFormat format, bool srv = false, bool msaa = false, bool depth = true);
        virtual DepthStencilTargetPtr createDepthStencilTarget(unsigned w, unsigned h, bool srv, bool msaa, bool floatDepth) = 0;

        unsigned getMSAACount() const { return _sampleCount; }
        unsigned getMSAAQuality() const { return _sampleQulity; }

    protected:
        unsigned _sampleCount;
        unsigned _sampleQulity;

        unsigned _width, _height;
        bool _fullscreen;
    };

    DECLAR_SMART_POINTER(RenderDevice)
}

#endif

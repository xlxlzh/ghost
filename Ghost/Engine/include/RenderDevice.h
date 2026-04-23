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
        virtual bool InitDevice(bool fullscreen, unsigned msaaCount) = 0;
        virtual bool CheckSampleCount(unsigned checkCount) = 0;
        
        virtual bool CompileShader(ShaderType type, const char* entry, const std::unordered_map<std::string, std::string>& defines, ShaderResource& shader) = 0;
        virtual Shader* CreateShader(const ShaderResourcePtr& shadersRes) = 0;
        virtual void ReflectShader(const ShaderResourcePtr& shadersRes, ShaderParams& params) = 0;

        virtual VertexBufferPtr CreateVertexBuffer(unsigned VertexSize, unsigned numVertices, ResourceUsage usage) = 0;
        virtual IndexBufferPtr CreateIndexBuffer(IndexBuffer::IndexType iType, unsigned numIndexes, ResourceUsage usage) = 0;
        virtual ConstBufferPtr CreateConstBuffer(unsigned bufferSize, ResourceUsage usage, const std::string& name) = 0;
        virtual VertexDeclarationPtr CreateVertexDeclaration() = 0;

        virtual RenderTargetPtr CreateRenderTargets(unsigned w, unsigned h, unsigned numRTS, GhostColorFormat* formats, bool srv = false, bool msaa = false, bool depth = true) = 0;
        virtual RenderTargetPtr CreateSingleRenderTarget(unsigned w, unsigned h, GhostColorFormat format, bool srv = false, bool msaa = false, bool depth = true);
        virtual DepthStencilTargetPtr CreateDepthStencilTarget(unsigned w, unsigned h, bool msaa, bool srv) = 0;

        unsigned GetMSAACount() const { return _sampleCount; }
        unsigned GetMSAAQuality() const { return _sampleQulity; }

    protected:
        unsigned _sampleCount;
        unsigned _sampleQulity;

        unsigned _width, _height;
        bool _fullscreen;
    };

    DECLAR_SMART_POINTER(RenderDevice)
}

#endif

#ifndef _D3D11_RENDERDEVICE_H_
#define _D3D11_RENDERDEVICE_H_

#include "RenderDevice.h"
#include "D3D11ComPtr.h"

namespace ghost
{
    class GHOST_API D3D11RenderDevice final : public RenderDevice
    {
        friend class D3D11RenderSystem;
    public:
        virtual bool initDevice(bool fullscreen, unsigned msaaCount) override;
        virtual bool checkSampleCount(unsigned checkCount) override;

        virtual bool compileShader(ShaderType type, const char* entry, const std::unordered_map<std::string, std::string>& defines, ShaderResource& shader) override;
        virtual Shader* createShader(const ShaderResourcePtr& shadersRes) override;
        virtual void reflectShader(const ShaderResourcePtr& shadersRes, ShaderParams& params) override;

        virtual VertexBufferPtr createVertexBuffer(unsigned VertexSize, unsigned numVertices, ResourceUsage usage) override;
        virtual IndexBufferPtr createIndexBuffer(IndexBuffer::IndexType iType, unsigned numIndexes, ResourceUsage usage) override;
        virtual ConstBufferPtr createConstBuffer(unsigned bufferSize, ResourceUsage usage, const std::string& name) override;
        virtual VertexDeclarationPtr createVertexDeclaration() override;

        virtual RenderTargetPtr createRenderTargets(unsigned w, unsigned h, unsigned numRTS, GhostColorFormat* formats, bool srv = false, bool msaa = false, bool depth = true) override;
        virtual DepthStencilTargetPtr createDepthStencilTarget(unsigned w, unsigned h, bool msaa, bool srv) override;

        ID3D11DevicePtr getDevice() const { return _device; }
        ID3D11DeviceContextPtr getContext() const { return _context; }

        D3D_FEATURE_LEVEL getFeatureLevel() const { return _featureLevel; }

    private:
        bool _initSwapchain();

    private:
        ID3D11DevicePtr _device;
        ID3D11DeviceContextPtr _context;

        ID3D11Texture2DPtr _defaultRenderTexture;
        ID3D11Texture2DPtr _defaultDepthTexture;
        ID3D11RenderTargetViewPtr _defaultRenderView;
        ID3D11DepthStencilViewPtr _defaultDepthView;

        D3D_FEATURE_LEVEL _featureLevel;
        D3D_DRIVER_TYPE _driverType;

        //dxgi
        IDXGISwapChainPtr _dxgiSwapchain;
        IDXGIDevicePtr _dxgiDevice;
        IDXGIFactoryPtr _dxgiFactory;
        IDXGIAdapterPtr _dxgiAdapter;

#ifdef GHOST_USE_D3D_11_1
        ID3DUserDefinedAnnotationPtr _annotaion;
#endif // GHOST_USE_D3D_11_1

    };

    DECLAR_SMART_POINTER(D3D11RenderDevice)
}

#endif
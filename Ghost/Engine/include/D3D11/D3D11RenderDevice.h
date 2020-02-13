#include "RenderDevice.h"
#include "D3D11ComPtr.h"

namespace ghost
{
    class GHOST_API D3D11RenderDevice : public RenderDevice
    {
    public:
        virtual bool initDevice(bool fullscreen, unsigned msaaCount) override;
        virtual bool checkSampleCount(unsigned checkCount) override;

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
    };
}
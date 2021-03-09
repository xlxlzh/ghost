#ifndef _DEPTHSTENCIL_TARGET_H_
#define _DEPTHSTENCIL_TARGET_H_

#include "Ghost.h"
#include "DepthStencilTarget.h"
#include "D3D11ComPtr.h"

namespace ghost
{
    class GHOST_API D3D11DepthStencilTarget : public DepthStencilTarget
    {
    public:
        D3D11DepthStencilTarget(unsigned w, unsigned h, bool msaa, bool floatDepth, bool srv);
        virtual ~D3D11DepthStencilTarget();

        ID3D11DepthStencilViewPtr getDepthView() const { return _depthView; }
        ID3D11Texture2DPtr getDepthTexture() const { return _depthTexture; }

    protected:
        void _onCreateDepthStencilTarget();
        void _onDestoryDepthStencilTarget();

    private:
        ID3D11DepthStencilViewPtr _depthView = nullptr;
        ID3D11Texture2DPtr _depthTexture = nullptr;
    };

    DECLAR_SMART_POINTER(D3D11DepthStencilTarget);
}

#endif // !_RENDERTARGET_H_

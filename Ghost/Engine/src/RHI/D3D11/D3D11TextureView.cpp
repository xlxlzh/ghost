#include "D3D11TextureView.h"

namespace ghost
{
    D3D11TextureView::D3D11TextureView(const ID3D11ShaderResourceViewPtr& shaderResourceView,
        unsigned width, unsigned height, GhostColorFormat format) :
        _shaderResourceView(shaderResourceView),
        _width(width),
        _height(height),
        _format(format)
    {
    }
}

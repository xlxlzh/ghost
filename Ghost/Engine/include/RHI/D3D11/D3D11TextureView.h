#ifndef _D3D11_TEXTURE_VIEW_H_
#define _D3D11_TEXTURE_VIEW_H_

#include "TextureView.h"
#include "D3D11ComPtr.h"

namespace ghost
{
    class GHOST_API D3D11TextureView final : public TextureView
    {
    public:
        D3D11TextureView(const ID3D11ShaderResourceViewPtr& shaderResourceView,
            unsigned width, unsigned height, GhostColorFormat format);

        unsigned GetWidth() const override { return _width; }
        unsigned GetHeight() const override { return _height; }
        GhostColorFormat GetFormat() const override { return _format; }
        TextureViewType GetViewType() const override { return TextureViewType::TEXTURE_VIEW_SHADER_RESOURCE; }
        uint64_t GetNativeHandle() const override { return static_cast<uint64_t>(reinterpret_cast<uintptr_t>(_shaderResourceView.Get())); }

        ID3D11ShaderResourceView* GetShaderResourceView() const { return _shaderResourceView.Get(); }

    private:
        ID3D11ShaderResourceViewPtr _shaderResourceView;
        unsigned _width;
        unsigned _height;
        GhostColorFormat _format;
    };
}

#endif

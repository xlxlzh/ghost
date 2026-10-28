#ifndef _TEXTURE_VIEW_H_
#define _TEXTURE_VIEW_H_

#include <cstdint>

#include "Ghost.h"
#include "Color.h"

namespace ghost
{
    enum class TextureViewType
    {
        TEXTURE_VIEW_SHADER_RESOURCE,
        TEXTURE_VIEW_RENDER_TARGET,
        TEXTURE_VIEW_DEPTH_STENCIL,
        TEXTURE_VIEW_UNORDERED_ACCESS
    };

    class GHOST_API TextureView
    {
    public:
        virtual ~TextureView() = default;

        virtual unsigned GetWidth() const = 0;
        virtual unsigned GetHeight() const = 0;
        virtual GhostColorFormat GetFormat() const = 0;
        virtual TextureViewType GetViewType() const = 0;
        virtual uint64_t GetNativeHandle() const = 0;
    };

    DECLAR_SMART_POINTER(TextureView)
}

#endif

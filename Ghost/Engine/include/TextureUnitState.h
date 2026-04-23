#ifndef _TEXTUREUNITSTATE_H_
#define _TEXTUREUNITSTATE_H_

#include "Ghost.h"
#include "RenderCommon.h"
#include "Color.h"

namespace ghost
{
    class GHOST_API Sampler
    {
    public:
        struct UVWAddressingMode
        {
            TextureAddressingMode u, v, w;
        };

        Sampler();
        ~Sampler() { }

        void SetAddressingMode(TextureAddressingMode u, TextureAddressingMode v, TextureAddressingMode w) { SetAddressingMode({ u, v, w }); }
        void SetAddressingMode(TextureAddressingMode mode) { SetAddressingMode({ mode, mode, mode }); }
        void SetAddressingMode(const UVWAddressingMode& mode);

        void SetFilter(TextureFilterOptions filterType);
        void SetFilter(FilterType type, FilterOptions opts);
        void SetFilter(FilterOptions minFilter, FilterOptions magFilter, FilterOptions mipFilter);
        FilterOptions GetFilter(FilterType type) const;

        UVWAddressingMode GetAddressingMode() const { return _addressingMode; }


    protected:
        UVWAddressingMode _addressingMode;

        FilterOptions _minFilter;
        FilterOptions _magFilter;
        FilterOptions _mipFilter;

        Color _borderColor;
    };

    DECLAR_SMART_POINTER(Sampler);
}

#endif

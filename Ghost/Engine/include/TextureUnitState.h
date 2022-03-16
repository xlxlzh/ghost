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

        void setAddressingMode(TextureAddressingMode u, TextureAddressingMode v, TextureAddressingMode w) { setAddressingMode({ u, v, w }); }
        void setAddressingMode(TextureAddressingMode mode) { setAddressingMode({ mode, mode, mode }); }
        void setAddressingMode(const UVWAddressingMode& mode);

        void setFilter(TextureFilterOptions filterType);
        void setFilter(FilterType type, FilterOptions opts);
        void setFilter(FilterOptions minFilter, FilterOptions magFilter, FilterOptions mipFilter);
        FilterOptions getFilter(FilterType type) const;

        UVWAddressingMode getAddressingMode() const { return _addressingMode; }


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

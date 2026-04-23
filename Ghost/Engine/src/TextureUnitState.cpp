#include "TextureUnitState.h"

namespace ghost
{
    Sampler::Sampler()
        : _borderColor(Color::Black)
        , _minFilter(FO_LINEAR)
        , _magFilter(FO_LINEAR)
        , _mipFilter(FO_POINT)
    {
        SetAddressingMode(ADDRESSING_WRAP);
    }

    void Sampler::SetAddressingMode(const UVWAddressingMode& mode)
    {
        _addressingMode = mode;
    }

    void Sampler::SetFilter(FilterOptions minFilter, FilterOptions magFilter, FilterOptions mipFilter)
    {
        _minFilter = minFilter;
        _magFilter = magFilter;
        _mipFilter = minFilter;
    }

    void Sampler::SetFilter(FilterType type, FilterOptions opts)
    {
        switch (type)
        {
        case FilterType::FT_MIN:
            _minFilter = opts;
            break;
        case FilterType::FT_MAG:
            _magFilter = opts;
            break;
        case FilterType::FT_MIP:
            _mipFilter = opts;
            break;
        }
    }

    void Sampler::SetFilter(TextureFilterOptions filterType)
    {
        switch (filterType)
        {
        case TextureFilterOptions::FILTER_NONE:
            SetFilter(FO_POINT, FO_POINT, FO_NONE);
            break;
        case TextureFilterOptions::FILTER_BILINEAR:
            SetFilter(FO_LINEAR, FO_LINEAR, FO_NONE);
            break;
        case TextureFilterOptions::FILTER_TRILINEAR:
            SetFilter(FO_LINEAR, FO_LINEAR, FO_LINEAR);
            break;
        case TextureFilterOptions::FILTER_ANISOTROPIC:
            SetFilter(FO_ANISOTROPIC, FO_ANISOTROPIC, FO_LINEAR);
            break;
        }
    }

    FilterOptions Sampler::GetFilter(FilterType type) const
    {
        switch (type)
        {
        case ghost::FT_MIN:
            return _minFilter;
        case ghost::FT_MAG:
            return _magFilter;
        case ghost::FT_MIP:
            return _mipFilter;
        }

        return _minFilter;
    }
}
#include "TextureUnitState.h"

namespace ghost
{
    Sampler::Sampler()
        : _borderColor(Color::Black)
        , _minFilter(FO_LINEAR)
        , _magFilter(FO_LINEAR)
        , _mipFilter(FO_POINT)
    {
        setAddressingMode(ADDRESSING_WRAP);
    }

    void Sampler::setAddressingMode(const UVWAddressingMode& mode)
    {
        _addressingMode = mode;
    }

    void Sampler::setFilter(FilterOptions minFilter, FilterOptions magFilter, FilterOptions mipFilter)
    {
        _minFilter = minFilter;
        _magFilter = magFilter;
        _mipFilter = minFilter;
    }

    void Sampler::setFilter(FilterType type, FilterOptions opts)
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

    void Sampler::setFilter(TextureFilterOptions filterType)
    {
        switch (filterType)
        {
        case TextureFilterOptions::FILTER_NONE:
            setFilter(FO_POINT, FO_POINT, FO_NONE);
            break;
        case TextureFilterOptions::FILTER_BILINEAR:
            setFilter(FO_LINEAR, FO_LINEAR, FO_NONE);
            break;
        case TextureFilterOptions::FILTER_TRILINEAR:
            setFilter(FO_LINEAR, FO_LINEAR, FO_LINEAR);
            break;
        case TextureFilterOptions::FILTER_ANISOTROPIC:
            setFilter(FO_ANISOTROPIC, FO_ANISOTROPIC, FO_LINEAR);
            break;
        }
    }

    FilterOptions Sampler::getFilter(FilterType type) const
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
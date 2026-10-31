#ifndef _TEXTURE2DRES_H_
#define _TEXTURE2DRES_H_

#include <cstdint>
#include "Resource.h"
#include "Color.h"

namespace ghost
{
    enum class PixelFormat
    {
        PF_UNKNOWN,
        PF_R8,
        PF_R8G8,
        PF_R8G8B8,
        PF_R8G8B8A8,
        PF_R16,
        PF_R16G16,
        PF_R16G16B16,
        PF_R16G16B16A16,
        PF_R32F,
        PF_RG32F,
        PF_RGB32F,
        PF_RGBA32F
    };

    class GHOST_API PixelFormatUtilies
    {
    public:
        static int GetBytesPerPixel(PixelFormat fmt)
        {
            switch (fmt)
            {
            case PixelFormat::PF_R8:
                return 1;
            case PixelFormat::PF_R8G8:
                return 2;
            case PixelFormat::PF_R8G8B8:
                return 3;
            case PixelFormat::PF_R8G8B8A8:
                return 4;
            case PixelFormat::PF_R16:
                return 2;
            case PixelFormat::PF_R16G16:
                return 4;
            case PixelFormat::PF_R16G16B16:
                return 6;
            case PixelFormat::PF_R16G16B16A16:
                return 8;
            case PixelFormat::PF_R32F:
                return 4;
            case PixelFormat::PF_RG32F:
                return 8;
            case PixelFormat::PF_RGB32F:
                return 12;
            case PixelFormat::PF_RGBA32F:
                return 16;
            case PixelFormat::PF_UNKNOWN:
                return 0;
            }

            return 0;
        }
    };

    class GHOST_API Texture2DRes : public Resource
    {
    public:
        Texture2DRes();
        Texture2DRes(const std::string& name, int flag);
        ~Texture2DRes();
        
        int GetWidth() const { return _width; }
        int GetHeight() const { return _height; }
        
        bool IsHdr() const { return _isHdr; }
        bool Is16Bit() const { return _is16Bit; }
        bool IsCompressed() const { return _compressed; }
        
        std::vector<uint8_t>& GetDatas() { return _datas; }
        std::vector<uint16_t>& Get16BitDatas() { return _16bitDatas; }
        std::vector<float>& GetHdrDatas() { return _hdrDatas; }
        int32_t GetComponents() const { return _components; }
        PixelFormat GetFormat() const { return _format; }
        bool IsSRGB() const { return _sRgb; }
        void Reset();

        virtual bool Load(DataStream& dataStream) override;
        virtual bool Save(DataStream& dataStream) override;

        static int GetTypeStatic() { return RESOURCE_TEXTURE2D; }
        
    protected:
        void UpdatePixelFormat(int32_t components);
        
    protected:
        int32_t _width = 0;
        int32_t _height = 0;
        int32_t _components = 0;

        bool _mipmap = false;
        int _mipmapLevel = 0;
        
        bool _sRgb = false;
        bool _isHdr = false;
        bool _is16Bit = false;
        bool _compressed = false;

        PixelFormat _format = PixelFormat::PF_UNKNOWN;
        std::vector<uint8_t> _datas;
        std::vector<uint16_t> _16bitDatas;
        std::vector<float> _hdrDatas;
    };

    DECLAR_SMART_POINTER(Texture2DRes)
}

#endif
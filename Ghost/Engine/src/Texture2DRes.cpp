#include "Texture2DRes.h"

#include "Engine.h"
#include "RenderDevice.h"
#include "STBWrapper.h"

namespace ghost
{
    Texture2DRes::Texture2DRes() : Texture2DRes("", 0)
    {

    }

    Texture2DRes::Texture2DRes(const std::string& name, int flag) : 
            Resource(RESOURCE_TEXTURE2D, name, flag)
    {

    }

    void Texture2DRes::Reset()
    {
        _width = 0;
        _height = 0;
        _components = 0;
        _isHdr = false;
        _is16Bit = false;
        _format = PixelFormat::PF_UNKNOWN;
        _datas.clear();
        _16bitDatas.clear();
        _hdrDatas.clear();
    }

    bool Texture2DRes::Load(DataStream& dataStream)
    {
        int dataSize = static_cast<int>(dataStream.GetSize());
        if (dataSize <= 0)
            return false;

        unsigned char* buffer = new unsigned char[dataSize];
        if (dataStream.Read(buffer, dataSize) != static_cast<std::size_t>(dataSize))
        {
            SAFE_DELETE_ARRAY(buffer);
            return false;
        }
        
        Reset();
        if (stbi_is_hdr_from_memory(buffer, dataSize))
        {
            _isHdr = true;
            float* pixelData = stbi_loadf_from_memory(buffer, dataSize, &_width, &_height, &_components, 4);
            if (!pixelData)
            {
                SAFE_DELETE_ARRAY(buffer);
                return false;   
            }
            
            _components = 4;
            uint32_t texDataSize = _width * _height * _components * sizeof(float);
            _hdrDatas.resize(_width * _height * _components);
            memcpy(_hdrDatas.data(), pixelData, texDataSize);
            stbi_image_free(pixelData);
        }
        else if (stbi_is_16_bit_from_memory(buffer, dataSize))
        {
            _is16Bit = true;
            uint16_t* pixelData = stbi_load_16_from_memory(buffer, dataSize, &_width, &_height, &_components, 4);
            if (!pixelData)
            {
                SAFE_DELETE_ARRAY(buffer);
                return false;   
            }
            
            _components = 4;
            uint32_t texDataSize = _width * _height * _components * sizeof(uint16_t);
            _16bitDatas.resize(_width * _height * _components);
            memcpy(_16bitDatas.data(), pixelData, texDataSize);
            stbi_image_free(pixelData);
        }
        else
        {
            uint8_t* pixelData = stbi_load_from_memory(buffer, dataSize, &_width, &_height, &_components, 4);
            if (!pixelData)
            {
                SAFE_DELETE_ARRAY(buffer);
                return false;
            }

            _components = 4;
            uint32_t texDataSize = _width * _height * _components;
            _datas.resize(texDataSize);
            memcpy(_datas.data(), pixelData, texDataSize);
            stbi_image_free(pixelData);
        }
        
        UpdatePixelFormat(_components);
        
        SAFE_DELETE_ARRAY(buffer);
        
        return true;
    }

    bool Texture2DRes::Save(DataStream& dataStream)
    {
        //TODO
        (void)dataStream;
        return true;
    }

    void Texture2DRes::UpdatePixelFormat(int32_t components)
    {
        if (_isHdr)
        {
            switch (components)
            {
            case 1:
                _format = PixelFormat::PF_R32F;
                break;
            case 2:
                _format = PixelFormat::PF_RG32F;
                break;
            case 3:
                _format = PixelFormat::PF_RGB32F;
                break;
            default:
                _format = PixelFormat::PF_RGBA32F;
                break;
            }
        }
        else if (_is16Bit)
        {
            switch (components)
            {
            case 1:
                _format = PixelFormat::PF_R16;
                break;
            case 2:
                _format = PixelFormat::PF_R16G16;
                break;
            case 3:
                _format = PixelFormat::PF_R16G16B16;
                break;
            default:
                _format = PixelFormat::PF_R16G16B16A16;
                break;
            }
        }
        else
        {
            switch (components)
            {
            case 1:
                _format = PixelFormat::PF_R8;
                break;
            case 2:
                _format = PixelFormat::PF_R8G8;
                break;
            case 3:
                _format = PixelFormat::PF_R8G8B8;
                break;
            default:
                _format = PixelFormat::PF_R8G8B8A8;
                break;
            }
        }
    }

    Texture2DRes::~Texture2DRes()
    {

    }
}
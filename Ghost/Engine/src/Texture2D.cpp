#include "Texture2D.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

namespace ghost
{
    Texture2D::Texture2D() : Resource(RESOURCE_TEXTURE2D, "", 0)
    {

    }

    Texture2D::Texture2D(const std::string& name, int flag) : Resource(RESOURCE_TEXTURE2D, name, flag)
    {

    }

    bool Texture2D::load(DataStream& dataStream)
    {
        int dataSize = dataStream.getSize();
        unsigned char* buffer = new unsigned char[dataSize];
        dataStream.read(buffer, dataSize);

        const unsigned char* pixelData = stbi_load_from_memory(buffer, dataSize, &_width, &_height, &_components, 0);
        _setData(pixelData);

        SAFE_DELETE_ARRAY(buffer);

        return true;
    }

    void Texture2D::_setData(const unsigned char* data)
    {
        _dataSize = _width * _height * _components;

        _datas = new unsigned char[_dataSize];
        memcpy(_datas, data, _dataSize);
    }

    Texture2D::~Texture2D()
    {

    }
}
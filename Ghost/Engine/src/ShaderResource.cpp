#include "ShaderResource.h"

namespace ghost
{
    ShaderResource::ShaderResource(int type, const std::string& name, int flag) :
        Resource(RESOURCE_SHADER, name, flag)
    {

    }

    bool ShaderResource::load(DataStream& dataStream)
    {
        int dataSize = dataStream.getSize();
        if (dataSize <= 0)
            return false;

        _rawDataSize = dataSize;
        _rawData = new unsigned char[_rawDataSize];
        dataStream.read(_rawData, _rawDataSize);

        return true;
    }
}
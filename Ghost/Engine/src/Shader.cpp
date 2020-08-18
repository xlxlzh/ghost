#include "Shader.h"

namespace ghost
{
    Shader::Shader(int type, const std::string& name, int flag) :
        Resource(RESOURCE_SHADER, name, flag)
    {

    }

    bool Shader::load(DataStream& dataStream)
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
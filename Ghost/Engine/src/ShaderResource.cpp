#include "ShaderResource.h"

namespace ghost
{
    ShaderType GetShaderTypeByName(const char* typeName)
    {
        for (int i = 0; i < SHADER_TYPE_NUM; ++i)
        {
            if (strcmp(typeName, ShaderTypeName[i]) == 0)
                return (ShaderType)i;
        }

        return SHADER_TYPE_NUM;
    }

    ShaderResource::ShaderResource(int type, const std::string& name, int flag) :
        Resource(RESOURCE_SHADER, name, flag)
    {

    }

    bool ShaderResource::Load(DataStream& dataStream)
    {
        int dataSize = dataStream.GetSize();
        if (dataSize <= 0)
            return false;

        _rawDataSize = dataSize;
        _rawData = new unsigned char[_rawDataSize];
        dataStream.Read(_rawData, _rawDataSize);

        return true;
    }

    void ShaderResource::Save(DataStream& dataStream)
    {
        //TODO
    }

    const ShaderByteCode* ShaderResource::GetByteCodeByType(ShaderType type) const
    {
        //The code maybe cause memory crash, will be fix later.
        auto byteCode = _byteCodes.find(type);
        return byteCode == _byteCodes.end() ? nullptr : &byteCode->second;
    }

    void ShaderResource::UpdateByteCodes(ShaderType type, unsigned char* byteCodes, int codeSize)
    {
        if (!byteCodes || codeSize < 0 || type == SHADER_TYPE_NUM)
            return;

        auto currentByteCode = _byteCodes.find(type);
        if (currentByteCode != _byteCodes.end())
        {
            SAFE_DELETE_ARRAY(currentByteCode->second.ByteCode);
            currentByteCode->second.ByteCodeSize = 0;

            _byteCodes[type].ByteCodeSize = codeSize;
            _byteCodes[type].ByteCode = new unsigned char[codeSize];
            memcpy(_byteCodes[type].ByteCode, byteCodes, codeSize);
        }
        else
        {
            ShaderByteCode newByteCode{};
            newByteCode.ByteCodeSize = codeSize;
            newByteCode.ByteCode = new unsigned char[codeSize];
            memcpy(newByteCode.ByteCode, byteCodes, codeSize);
            _byteCodes[type] = newByteCode;
        }
    }
}
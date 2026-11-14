#include "ShaderResource.h"

#include <cstring>

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

    bool ShaderResource::Save(DataStream& dataStream)
    {
        //TODO
        (void)dataStream;
        return true;
    }

    CompiledShader::~CompiledShader()
    {
        for (ShaderByteCode& byteCode : _byteCodes)
            SAFE_DELETE_ARRAY(byteCode.ByteCode);
    }

    void CompiledShader::UpdateByteCode(ShaderType type, const void* byteCode, int codeSize)
    {
        if (!byteCode || codeSize <= 0 || type >= SHADER_TYPE_NUM)
            return;

        ShaderByteCode& target = _byteCodes[static_cast<std::size_t>(type)];
        SAFE_DELETE_ARRAY(target.ByteCode);

        target.ByteCode = new unsigned char[codeSize];
        target.ByteCodeSize = codeSize;
        memcpy(target.ByteCode, byteCode, codeSize);
    }

    const ShaderByteCode* CompiledShader::GetByteCodeByType(ShaderType type) const
    {
        if (type >= SHADER_TYPE_NUM)
            return nullptr;

        const ShaderByteCode& byteCode = _byteCodes[static_cast<std::size_t>(type)];
        return byteCode.ByteCode ? &byteCode : nullptr;
    }
}
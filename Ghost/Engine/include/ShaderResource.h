#ifndef _SHADER_RESOURCE_H_
#define _SHADER_RESOURCE_H_

#include "Resource.h"

namespace ghost
{
    enum ShaderType
    {
        SHADER_VS,
        SHADER_PS,
        SHADER_GS,
        SHADER_HS,
        SHADER_DS,
        SHADER_NONE
    };

    static const char* ShaderTypeName[] =
    {
        "VS",
        "PS",
        "GS",
        "HS",
        "DS",
        "None"
    };

    GHOST_API ShaderType GetShaderTypeByName(const char* typeName);

    struct GHOST_API ShaderByteCode
    {
        unsigned char* ByteCode{ nullptr };
        int ByteCodeSize{ 0 };
    };

    class GHOST_API ShaderResource : public Resource
    {
    public:
        ShaderResource() : ShaderResource(RESOURCE_SHADER, "", 0) { }
        ShaderResource(int type, const std::string& name, int flag);

        virtual bool load(DataStream& dataStream) override;

        static int getTypeStatic() { return RESOURCE_SHADER; }

        unsigned char* getRawdata() const { return _rawData; }
        int getRawdataSize() const { return _rawDataSize; }

        void updateByteCodes(ShaderType type, unsigned char* byteCodes, int codeSize);

    protected:
        unsigned char* _rawData = nullptr;
        int _rawDataSize = 0;

        std::unordered_map<ShaderType, ShaderByteCode> _byteCodes{};
    };
}

#endif

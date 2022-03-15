#ifndef _SHADER_RESOURCE_H_
#define _SHADER_RESOURCE_H_

#include "Resource.h"

namespace ghost
{
    enum ShaderType
    {
        SHADER_VS,
        SHADER_GS,
        SHADER_HS,
        SHADER_DS,
        SHADER_PS,
        SHADER_TYPE_NUM
    };

    static const char* ShaderTypeName[] =
    {
        "VS",
        "GS",
        "HS",
        "DS",
        "PS",
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

        const ShaderByteCode* getByteCodeByType(ShaderType type) const;

        int getValidShaderCount() const;

    protected:
        unsigned char* _rawData = nullptr;
        int _rawDataSize = 0;

        std::unordered_map<ShaderType, ShaderByteCode> _byteCodes{};
    };

    DECLAR_SMART_POINTER(ShaderResource);
}

#endif

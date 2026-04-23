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

        virtual bool Load(DataStream& dataStream) override;
        virtual void Save(DataStream& dataStream) override;

        static int GetTypeStatic() { return RESOURCE_SHADER; }

        unsigned char* GetRawdata() const { return _rawData; }
        int GetRawdataSize() const { return _rawDataSize; }

        void UpdateByteCodes(ShaderType type, unsigned char* byteCodes, int codeSize);

        const ShaderByteCode* GetByteCodeByType(ShaderType type) const;

        int GetValidShaderCount() const;

    protected:
        unsigned char* _rawData = nullptr;
        int _rawDataSize = 0;

        std::unordered_map<ShaderType, ShaderByteCode> _byteCodes{};
    };

    DECLAR_SMART_POINTER(ShaderResource);
}

#endif

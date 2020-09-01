#ifndef _SHADER_RESOURCE_H_
#define _SHADER_RESOURCE_H_

#include "Resource.h"

namespace ghost
{
    class GHOST_API ShaderResource : public Resource
    {
    public:
        ShaderResource() : ShaderResource(RESOURCE_SHADER, "", 0) { }
        ShaderResource(int type, const std::string& name, int flag);

        virtual bool load(DataStream& dataStream) override;

        static int getTypeStatic() { return RESOURCE_SHADER; }

    protected:
        unsigned char* _rawData = nullptr;
        int _rawDataSize = 0;
    };
}

#endif

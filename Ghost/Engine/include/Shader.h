#ifndef _SHADER_H_
#define _SHADER_H_

#include "Resource.h"

namespace ghost
{
    class GHOST_API Shader : public Resource
    {
    public:
        Shader() : Shader(RESOURCE_SHADER, "", 0) { }
        Shader(int type, const std::string& name, int flag);

        virtual bool load(DataStream& dataStream) override;

        static int getTypeStatic() { return RESOURCE_SHADER; }

    private:
        unsigned char* _rawData = nullptr;
        int _rawDataSize = 0;
    };
}

#endif

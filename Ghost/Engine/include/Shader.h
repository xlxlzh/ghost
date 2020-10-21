#ifndef _SHADER_H_
#define _SHADER_H_

#include "Ghost.h"
#include "ShaderResource.h"

namespace ghost
{
    class GHOST_API Shader
    {
    public:
        Shader() { _type = SHADER_NONE; }

        ShaderType getType() const { return _type; }

    protected:
        ShaderType _type;
    };
}

#endif

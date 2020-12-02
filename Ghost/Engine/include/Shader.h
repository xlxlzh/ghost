#ifndef _SHADER_H_
#define _SHADER_H_

#include "Ghost.h"
#include "ShaderResource.h"

namespace ghost
{
    class GHOST_API Shader
    {
    public:
        Shader() = default;
        virtual ~Shader() { }

        void* getRawShaderPointer(ShaderType type) const;

    protected:
        std::unordered_map<ShaderType, void*> _shaders;
    };
}

#endif

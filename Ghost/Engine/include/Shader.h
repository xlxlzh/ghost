#ifndef _SHADER_H_
#define _SHADER_H_

#include <array>
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
        void updateRawShaderPointer(ShaderType type, void* shader);

        bool isShaderTypeValid(ShaderType type) const;

        bool isValid() const;

    protected:
        std::array<void*, (std::size_t)SHADER_NONE > _shaders;
    };
}

#endif

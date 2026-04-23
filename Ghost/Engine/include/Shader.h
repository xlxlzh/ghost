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

        void* GetRawShaderPointer(ShaderType type) const;
        void UpdateRawShaderPointer(ShaderType type, void* shader);

        bool IsShaderTypeValid(ShaderType type) const;

        bool IsValid() const;

    protected:
        std::array<void*, (std::size_t)SHADER_TYPE_NUM > _shaders;
    };
}

#endif

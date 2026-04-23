#include "Shader.h"

namespace ghost
{
    void* Shader::GetRawShaderPointer(ShaderType type) const
    {
        assert(type < SHADER_TYPE_NUM);
        return _shaders[type];
    }

    void Shader::UpdateRawShaderPointer(ShaderType type, void* shader)
    {
        assert(type < SHADER_TYPE_NUM);
        _shaders[type] = shader;
    }

    bool Shader::IsValid() const
    {
        for (const auto& shader : _shaders)
        {
            if (shader)
                return true;
        }

        return false;
    }

    bool Shader::IsShaderTypeValid(ShaderType type) const
    {
        assert(type <= SHADER_TYPE_NUM);

        return _shaders[type] != nullptr;
    }
}
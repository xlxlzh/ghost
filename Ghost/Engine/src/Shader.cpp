#include "Shader.h"

namespace ghost
{
    void* Shader::getRawShaderPointer(ShaderType type) const
    {
        assert(type < SHADER_TYPE_NUM);
        return _shaders[type];
    }

    void Shader::updateRawShaderPointer(ShaderType type, void* shader)
    {
        assert(type < SHADER_TYPE_NUM);
        _shaders[type] = shader;
    }

    bool Shader::isValid() const
    {
        for (const auto& shader : _shaders)
        {
            if (shader)
                return true;
        }

        return false;
    }

    bool Shader::isShaderTypeValid(ShaderType type) const
    {
        assert(type <= SHADER_TYPE_NUM);

        return _shaders[type] != nullptr;
    }
}
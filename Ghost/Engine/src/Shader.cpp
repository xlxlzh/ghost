#include "Shader.h"

namespace ghost
{
    void* Shader::getRawShaderPointer(ShaderType type) const
    {
        auto byteCodes = _shaders.find(type);
        return byteCodes == _shaders.end() ? nullptr : byteCodes->second;
    }
}
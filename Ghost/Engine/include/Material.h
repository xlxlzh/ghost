#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Resource.h"
#include "Shader.h"

#include <unordered_map>
#include <array>

namespace ghost
{
    class GHOST_API Material : public Resource
    {
    public:
        Material() : Material(RESOURCE_MATERIAL, "", 0) { }
        Material(int type, const std::string& name, int flag);

        virtual bool load(DataStream& dataStream) override;

        void apply();

        static int getTypeStatic() { return RESOURCE_MATERIAL; }

    private:
        ShaderResource* _shaderResource{ nullptr };
        std::unordered_map<std::string, std::string> _defines{};
        std::array<void*, (std::size_t)SHADER_NONE> _shaders;
        Shader* _handwareShader{ nullptr };
    };
}

#endif

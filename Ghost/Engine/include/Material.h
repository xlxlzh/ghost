#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Resource.h"
#include "Shader.h"

#include <unordered_map>
#include <array>

namespace ghost
{
    struct GHOST_API InputSignature
    {
        unsigned short _index;
        unsigned short _slot;
        std::string _semantic;

        bool operator == (const InputSignature& rhs)
        {
            return _slot == rhs._slot && _index == rhs._index
                && _semantic == rhs._semantic;
        }

        bool operator != (const InputSignature& rhs)
        {
            return !((*this) == rhs);
        }

        bool operator < (const InputSignature& rhs)
        {
            return _slot < rhs._slot || _index < rhs._index;
        }
    };

    using InputSignatureList = std::vector<InputSignature>;

//     struct InputSignatureDesc
//     {
//         InputSignatureList _signatures;
// 
//         bool operator < (const InputSignatureDesc& rhs)
//         {
//             bool bRet = true;
//             if (_signatures.size() > rhs._signatures.size())
//                 bRet = false;
//             else if (_signatures.size() == rhs._signatures.size())
//             {
// 
//             }
// 
//             return bRet;
//         }
//     };

    class GHOST_API ShaderParams
    {
    public:
        InputSignatureList _sigDesc;
    };

    using ShaderParamsList = std::array<ShaderParams, (std::size_t)SHADER_NONE>;
    class GHOST_API Material : public Resource
    {
    public:
        Material() : Material(RESOURCE_MATERIAL, "", 0) { }
        Material(int type, const std::string& name, int flag);

        virtual bool load(DataStream& dataStream) override;

        void apply();

        InputSignatureList* getShaderInputSignature() { return &_params[SHADER_VS]._sigDesc; }

        static int getTypeStatic() { return RESOURCE_MATERIAL; }
        ShaderResource* getLinkedShader() { return _shaderResource; }

    private:
        ShaderResource* _shaderResource{ nullptr };
        std::unordered_map<std::string, std::string> _defines{};
        std::array<void*, (std::size_t)SHADER_NONE> _shaders;
        Shader* _handwareShader{ nullptr };

        ShaderParamsList _params;
    };
}

#endif

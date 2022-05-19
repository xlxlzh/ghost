#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Resource.h"
#include "Shader.h"
#include "Texture2D.h"
#include "RenderCommon.h"
#include "TextureUnitState.h"

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
    };

    using InputSignatureList = std::vector<InputSignature>;

    struct GHOST_API ConstBufferVariable 
    {
        std::string _name;
        unsigned _offset;
        unsigned _size;
    };

    using ConstBufferVariableList = std::vector<ConstBufferVariable>;

    struct GHOST_API ConstBufferInfo
    {
        std::string _name;
        unsigned _bindPoint;
        unsigned _bufferSize;

        ConstBufferVariableList _variables;
    };

    using ConstBufferParamsList = std::vector<ConstBufferInfo>;

    struct GHOST_API TextureVariableInfo
    {
        std::string _name;
        unsigned _bindPoint;
        Texture2DPtr _texture;
    };

    using TextureVariableList = std::vector<TextureVariableInfo>;

    struct GHOST_API SamplerInfo
    {
        std::string _name;
        unsigned _bindPoint;
        Sampler _sampler;
    };

    using SamplerList = std::vector<SamplerInfo>;

    struct GHOST_API ShaderParams
    {
        InputSignatureList _sigDesc;
        TextureVariableList _textures;
        SamplerList _samplers;

        std::array<ConstBufferParamsList, (std::size_t)SHADER_TYPE_NUM> _constBuffers;
    };


    enum RenderPass
    {
        RENDER_PASS_FORWARD,
        RENDER_PASS_SHADOW,
        RENDER_PASS_DEPTH,
        RENDER_PASS_DEFERRED,
        RENDER_PASS_NUM
    };

    class GHOST_API ShaderPass
    {
        friend class Material;
    public:
        ShaderPass();
        ShaderPass(RenderPass pass);
        ~ShaderPass();

        RenderPass getRenderPass() const { return _passType; }
        ShaderResourcePtr getLinkedShader() { return _linkedShader; }
        InputSignatureList* getShaderInputSignature() { return &_params._sigDesc; }
        unsigned getConstBufferSlot(ShaderType type, const std::string& name);

        void applyTextureToSlot(const std::string& name, Texture2DPtr ptr);
        void applySamplerToSlot(const std::string& name, const Sampler& sampler);

    protected:
        ShaderResourcePtr _linkedShader = nullptr;
        RenderPass _passType;
         
        std::unordered_map<std::string, std::string> _defines{};

        Shader* _handwareShader{ nullptr };

        ShaderParams _params;


    };

    class GHOST_API Material : public Resource
    {
    public:
        Material() : Material(RESOURCE_MATERIAL, "", 0) { }
        Material(int type, const std::string& name, int flag);

        virtual bool load(DataStream& dataStream) override;
        virtual void save(DataStream& dataStream) override;

        void applyToRenderPass(RenderPass pass);

        static int getTypeStatic() { return RESOURCE_MATERIAL; }

        ShaderPass* getShaderPass(RenderPass pass);

    private:
        std::vector<ShaderPass> _passes;
    };

    DECLAR_SMART_POINTER(Material);
}

#endif

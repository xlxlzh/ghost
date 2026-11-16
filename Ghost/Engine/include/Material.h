#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Resource.h"
#include "Shader.h"
#include "ShaderAsset.h"
#include "Texture2D.h"
#include "RenderCommon.h"
#include "TextureUnitState.h"
#include "ConstBuffer.h"

#include <cstdint>
#include <unordered_map>
#include <array>

namespace ghost
{
    enum class MaterialParameterType : uint8_t
    {
        Unknown,
        Float,
        Float2,
        Float3,
        Float4,
        Color,
        Int,
        Bool
    };

    struct GHOST_API MaterialParameterValue
    {
        MaterialParameterType _type = MaterialParameterType::Unknown;
        std::array<float, 4> _values{};
    };

    using MaterialParameterMap = std::unordered_map<std::string, MaterialParameterValue>;

    constexpr const char* MaterialParameterBufferName = "MaterialParams";

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
        MaterialParameterType _type = MaterialParameterType::Unknown;
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
        Texture2DResPtr _texture;
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


    struct GHOST_API TextureSlotDefinition
    {
        std::string name;
        std::string path;
    };

    struct GHOST_API SamplerSlotDefinition
    {
        std::string name;
        FilterOptions minFilter = FO_LINEAR;
        FilterOptions magFilter = FO_LINEAR;
        FilterOptions mipFilter = FO_LINEAR;
    };

    class GHOST_API ShaderPass
    {
        friend class Material;
    public:
        ShaderPass();
        ShaderPass(RenderPass pass);
        ~ShaderPass();

        RenderPass GetRenderPass() const { return _passType; }
        const PassRenderState& GetRenderState() const { return _renderState; }
        ShaderResourcePtr GetLinkedShader() { return _linkedShader; }
        CompiledShaderPtr GetCompiledShader() { return _compiledShader; }
        InputSignatureList* GetShaderInputSignature() { return &_params._sigDesc; }
        unsigned GetConstBufferSlot(ShaderType type, const std::string& name);

        bool GetTextureBindPoint(const std::string& name, unsigned& bindPoint) const;
        bool GetSamplerBindPoint(const std::string& name, unsigned& bindPoint) const;
        const ConstBufferInfo* GetConstBufferInfo(ShaderType type, const std::string& name) const;

    private:
        void ApplyTextureToSlot(const std::string& name, Texture2DResPtr ptr);
        void ApplySamplerToSlot(const std::string& name, const Sampler& sampler);

    protected:
        ShaderResourcePtr _linkedShader = nullptr;
        CompiledShaderPtr _compiledShader = nullptr;
        RenderPass _passType = RENDER_PASS_FORWARD;

        std::unordered_map<std::string, std::string> _defines{};

        std::string _shaderSource;
        std::vector<ShaderEntryDefinition> _shaderEntries;
        std::vector<TextureSlotDefinition> _textureSlots;
        std::vector<SamplerSlotDefinition> _samplerSlots;

        Shader* _handwareShader{ nullptr };

        PassRenderState _renderState;
        ShaderParams _params;
    };

    class GHOST_API MaterialParameterBinding
    {
    public:
        void Apply(const MaterialParameterMap& defaults,
            const MaterialParameterMap* overrides, ShaderPass& pass);

    private:
        struct BufferEntry
        {
            ConstBufferPtr _buffer;
            unsigned _size = 0;
        };

        std::array<std::array<BufferEntry, static_cast<std::size_t>(SHADER_TYPE_NUM)>,
            static_cast<std::size_t>(RENDER_PASS_NUM)> _buffers{};
    };

    class Material;

    DECLAR_SMART_POINTER(Material);

    class GHOST_API MaterialInterface
    {
    public:
        virtual ~MaterialInterface() = default;

        virtual Material* GetBaseMaterial() = 0;
        virtual void ApplyToRenderPass(RenderPass pass) = 0;
        virtual uint32_t GetVersion() const = 0;
    };

    DECLAR_SMART_POINTER(MaterialInterface);

    class GHOST_API Material : public Resource, public MaterialInterface
    {
    public:
        Material() : Material(RESOURCE_MATERIAL, "", 0) { }
        Material(int type, const std::string& name, int flag);

        virtual bool Load(DataStream& dataStream) override;
        virtual bool Save(DataStream& dataStream) override;
        virtual bool SaveXml(DataStream& dataStream) override;

        virtual Material* GetBaseMaterial() override { return this; }
        virtual void ApplyToRenderPass(RenderPass pass) override;
        virtual uint32_t GetVersion() const override { return 0; }

        static int GetTypeStatic() { return RESOURCE_MATERIAL; }

        ShaderPass* GetShaderPass(RenderPass pass);

        const MaterialParameterMap& GetParameters() const { return _parameters; }

    private:
        static bool LoadPassesXml(DataStream& dataStream, std::vector<ShaderPass>& passes,
            MaterialParameterMap& parameters, const std::string& assetName);
        static bool LoadPassesBinary(DataStream& dataStream, std::vector<ShaderPass>& passes,
            MaterialParameterMap& parameters, const std::string& assetName);
        static bool FinalizePasses(std::vector<ShaderPass>& passes,
            const std::string& assetName);
        static bool SavePassesBinary(const std::vector<ShaderPass>& passes,
            const MaterialParameterMap& parameters, DataStream& dataStream,
            const std::string& assetName);
        static bool SavePassesXml(const std::vector<ShaderPass>& passes,
            const MaterialParameterMap& parameters, DataStream& dataStream,
            const std::string& assetName);

        std::vector<ShaderPass> _passes;
        MaterialParameterMap _parameters;
        MaterialParameterBinding _parameterBinding;
    };

    class GHOST_API MaterialInstanceDynamic : public MaterialInterface
    {
    public:
        explicit MaterialInstanceDynamic(const MaterialPtr& baseMaterial);

        virtual Material* GetBaseMaterial() override;
        virtual void ApplyToRenderPass(RenderPass pass) override;
        virtual uint32_t GetVersion() const override { return _version; }

        void SetTexture(const std::string& slot, const Texture2DResPtr& texture);
        void ClearTexture(const std::string& slot);
        void SetSampler(const std::string& slot, const Sampler& sampler);
        void ClearSampler(const std::string& slot);

        void SetParameter(const std::string& name, const MaterialParameterValue& value);
        void SetFloat(const std::string& name, float value);
        void SetFloat2(const std::string& name, float x, float y);
        void SetFloat3(const std::string& name, float x, float y, float z);
        void SetFloat4(const std::string& name, float x, float y, float z, float w);
        void SetColor(const std::string& name, float r, float g, float b, float a = 1.0f);
        void SetInt(const std::string& name, int value);
        void SetBool(const std::string& name, bool value);
        void ClearParameter(const std::string& name);

    protected:
        MaterialPtr _baseMaterial;
        std::unordered_map<std::string, Texture2DResPtr> _textureOverrides;
        std::unordered_map<std::string, Sampler> _samplerOverrides;
        MaterialParameterMap _parameterOverrides;
        MaterialParameterBinding _parameterBinding;
        uint32_t _version = 0;
    };

    DECLAR_SMART_POINTER(MaterialInstanceDynamic);
}

#endif

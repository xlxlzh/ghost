#include <algorithm>
#include <cstring>
#include <map>
#include <utility>

#include "BinaryArchive.h"
#include "DataStream.h"
#include "Engine.h"
#include "LogManager.h"
#include "tinyxml2.h"
#include "Material.h"
#include "GPUObjectManager.h"

namespace ghost
{
    namespace
    {
        constexpr uint32_t kMaxMaterialStringLength = 8192;
        constexpr uint32_t kMaxMaterialPasses = 64;
        constexpr uint32_t kMaxMaterialDefines = 4096;
        constexpr uint32_t kMaxMaterialEntries = 16;
        constexpr uint32_t kMaxMaterialSlots = 64;

        const std::map<std::string, FilterOptions> FilterOptionMappings =
        {
            {"FILTER_NONE", FO_NONE},
            {"FILTER_POINT", FO_POINT},
            {"FILTER_LINEAR", FO_LINEAR},
            {"FILTER_ANISOTROPIC", FO_ANISOTROPIC},
        };

        RenderPass ParsePassName(const char* name)
        {
            if (!name)
                return RENDER_PASS_FORWARD;

            if (strcmp(name, "forward") == 0)
                return RENDER_PASS_FORWARD;
            if (strcmp(name, "shadow") == 0)
                return RENDER_PASS_SHADOW;
            if (strcmp(name, "deferred") == 0)
                return RENDER_PASS_DEFERRED;
            if (strcmp(name, "depth") == 0)
                return RENDER_PASS_DEPTH;

            GHOST_LOG_FORMAT_WARNNING("Material: unknown pass name '%s', using forward.", name);
            return RENDER_PASS_FORWARD;
        }

        const char* GetPassName(RenderPass pass)
        {
            switch (pass)
            {
            case RENDER_PASS_FORWARD:  return "forward";
            case RENDER_PASS_SHADOW:   return "shadow";
            case RENDER_PASS_DEPTH:    return "depth";
            case RENDER_PASS_DEFERRED: return "deferred";
            default:                   return "forward";
            }
        }

        const char* GetFilterName(FilterOptions option)
        {
            switch (option)
            {
            case FO_NONE:        return "FILTER_NONE";
            case FO_POINT:       return "FILTER_POINT";
            case FO_LINEAR:      return "FILTER_LINEAR";
            case FO_ANISOTROPIC: return "FILTER_ANISOTROPIC";
            default:             return "FILTER_LINEAR";
            }
        }

        FilterOptions ParseFilterOption(const char* name)
        {
            if (name)
            {
                const auto& iterator = FilterOptionMappings.find(name);
                if (iterator != FilterOptionMappings.end())
                    return iterator->second;
            }

            GHOST_LOG_FORMAT_WARNNING("Material: unknown filter value '%s', using linear.",
                name ? name : "");
            return FO_LINEAR;
        }

        bool IsValidFilter(FilterOptions option)
        {
            return option >= FO_NONE && option <= FO_ANISOTROPIC;
        }

        std::vector<std::pair<std::string, std::string>> GetSortedDefines(
            const std::unordered_map<std::string, std::string>& defines)
        {
            std::vector<std::pair<std::string, std::string>> result(defines.begin(), defines.end());
            std::sort(result.begin(), result.end(),
                [](const auto& lhs, const auto& rhs)
                {
                    return lhs.first < rhs.first;
                });

            return result;
        }
    }

    ShaderPass::ShaderPass() = default;

    ShaderPass::ShaderPass(RenderPass pass) : _passType(pass)
    {
    }

    ShaderPass::~ShaderPass() = default;

    unsigned ShaderPass::GetConstBufferSlot(ShaderType type, const std::string& name)
    {
        auto& buffers = _params._constBuffers[type];
        for (auto& cb : buffers)
        {
            if (cb._name == name)
                return cb._bindPoint;
        }

        return 0;
    }

    void ShaderPass::ApplyTextureToSlot(const std::string& name, Texture2DResPtr ptr)
    {
        auto it = std::find_if(_params._textures.begin(), _params._textures.end(),
            [&name](const TextureVariableInfo& info)->bool
            {
                return info._name == name;
            });

        if (it != _params._textures.end())
            it->_texture = ptr;
    }

    void ShaderPass::ApplySamplerToSlot(const std::string& name, const Sampler& sampler)
    {
        auto it = std::find_if(_params._samplers.begin(), _params._samplers.end(),
            [&name](const SamplerInfo& info)->bool
            {
                return info._name == name;
            });

        if (it != _params._samplers.end())
            it->_sampler = sampler;
    }

    Material::Material(int type, const std::string& name, int flag) :
        Resource(type, name, flag)
    {
    }

    bool Material::Load(DataStream& dataStream)
    {
        _loaded = false;

        std::vector<ShaderPass> passes;
        const std::string assetName = dataStream.GetName();

        bool parsed = false;
        switch (DetectAssetEncoding(dataStream))
        {
        case AssetEncoding::Gres:
            parsed = LoadPassesBinary(dataStream, passes, assetName);
            break;

        case AssetEncoding::Xml:
            parsed = LoadPassesXml(dataStream, passes, assetName);
            break;

        default:
            GHOST_LOG_FORMAT_ERROR("Material::Load: unsupported asset format in '%s'.",
                assetName.c_str());
            return false;
        }

        if (!parsed)
            return false;

        if (!FinalizePasses(passes, assetName))
            return false;

        _passes.swap(passes);
        _loaded = true;
        return true;
    }

    bool Material::Save(DataStream& dataStream)
    {
        return SavePassesBinary(_passes, dataStream, GetName());
    }

    bool Material::SaveXml(DataStream& dataStream)
    {
        return SavePassesXml(_passes, dataStream, GetName());
    }

    bool Material::LoadPassesXml(DataStream& dataStream, std::vector<ShaderPass>& passes,
        const std::string& assetName)
    {
        std::string text;
        if (!ReadStreamAsText(dataStream, text))
        {
            GHOST_LOG_FORMAT_ERROR("Material::LoadPassesXml: empty or unreadable stream '%s'.",
                assetName.c_str());
            return false;
        }

        tinyxml2::XMLDocument document;
        if (document.Parse(text.c_str(), text.size()) != tinyxml2::XML_SUCCESS)
        {
            GHOST_LOG_FORMAT_ERROR("Material::LoadPassesXml: XML parse error in '%s'.",
                assetName.c_str());
            return false;
        }

        tinyxml2::XMLElement* root = document.RootElement();
        if (!root || strcmp(root->Name(), "Material") != 0)
        {
            GHOST_LOG_FORMAT_ERROR("Material::LoadPassesXml: missing <Material> root in '%s'.",
                assetName.c_str());
            return false;
        }

        const int version = root->IntAttribute("version", 1);
        if (version > 1)
        {
            GHOST_LOG_FORMAT_WARNNING("Material::LoadPassesXml: unsupported version %d in '%s'.",
                version, assetName.c_str());
        }

        passes.clear();

        for (tinyxml2::XMLElement* passElement = root->FirstChildElement("Pass");
            passElement != nullptr;
            passElement = passElement->NextSiblingElement("Pass"))
        {
            const char* passName = passElement->Attribute("name");
            if (!passName)
            {
                GHOST_LOG_FORMAT_ERROR("Material::LoadPassesXml: pass without a name in '%s'.",
                    assetName.c_str());
                return false;
            }

            ShaderPass pass(ParsePassName(passName));

            tinyxml2::XMLElement* definesElement = passElement->FirstChildElement("Defines");
            if (definesElement)
            {
                for (tinyxml2::XMLElement* defineElement = definesElement->FirstChildElement();
                    defineElement != nullptr;
                    defineElement = defineElement->NextSiblingElement())
                {
                    const char* name = defineElement->Name();
                    if (!name || !name[0])
                        continue;

                    const char* value = defineElement->Attribute("value");
                    pass._defines[name] = value ? value : "";
                }
            }

            tinyxml2::XMLElement* shaderElement = passElement->FirstChildElement("Shader");
            if (!shaderElement)
            {
                GHOST_LOG_FORMAT_ERROR("Material::LoadPassesXml: missing <Shader> in '%s'.",
                    assetName.c_str());
                return false;
            }

            const char* shaderSource = shaderElement->Attribute("source");
            if (!shaderSource || !shaderSource[0])
            {
                GHOST_LOG_FORMAT_ERROR("Material::LoadPassesXml: missing shader source in '%s'.",
                    assetName.c_str());
                return false;
            }

            pass._shaderSource = shaderSource;

            for (tinyxml2::XMLElement* entryElement = shaderElement->FirstChildElement();
                entryElement != nullptr;
                entryElement = entryElement->NextSiblingElement())
            {
                const char* typeName = entryElement->Name();
                const ShaderType type = GetShaderTypeByName(typeName);
                if (type >= SHADER_TYPE_NUM)
                {
                    GHOST_LOG_FORMAT_WARNNING("Material::LoadPassesXml: unknown shader stage '%s' in '%s'.",
                        typeName ? typeName : "", assetName.c_str());
                    continue;
                }

                const char* entryName = entryElement->Attribute("entry");
                if (!entryName || !entryName[0])
                    continue;

                ShaderEntryDefinition definition;
                definition.type = type;
                definition.entry = entryName;
                pass._shaderEntries.push_back(std::move(definition));
            }

            tinyxml2::XMLElement* texturesElement = passElement->FirstChildElement("Textures");
            if (texturesElement)
            {
                for (tinyxml2::XMLElement* textureElement = texturesElement->FirstChildElement();
                    textureElement != nullptr;
                    textureElement = textureElement->NextSiblingElement())
                {
                    const char* slotName = textureElement->Name();
                    if (!slotName || !slotName[0])
                        continue;

                    TextureSlotDefinition definition;
                    definition.name = slotName;

                    const char* texturePath = textureElement->Attribute("path");
                    if (texturePath)
                        definition.path = texturePath;

                    pass._textureSlots.push_back(std::move(definition));
                }
            }

            tinyxml2::XMLElement* samplersElement = passElement->FirstChildElement("Samplers");
            if (samplersElement)
            {
                for (tinyxml2::XMLElement* samplerElement = samplersElement->FirstChildElement();
                    samplerElement != nullptr;
                    samplerElement = samplerElement->NextSiblingElement())
                {
                    const char* slotName = samplerElement->Name();
                    if (!slotName || !slotName[0])
                        continue;

                    SamplerSlotDefinition definition;
                    definition.name = slotName;

                    if (tinyxml2::XMLElement* minElement = samplerElement->FirstChildElement("MIN"))
                        definition.minFilter = ParseFilterOption(minElement->Attribute("value"));
                    if (tinyxml2::XMLElement* magElement = samplerElement->FirstChildElement("MAG"))
                        definition.magFilter = ParseFilterOption(magElement->Attribute("value"));
                    if (tinyxml2::XMLElement* mipElement = samplerElement->FirstChildElement("MIP"))
                        definition.mipFilter = ParseFilterOption(mipElement->Attribute("value"));

                    pass._samplerSlots.push_back(std::move(definition));
                }
            }

            passes.push_back(std::move(pass));
        }

        return true;
    }

    bool Material::LoadPassesBinary(DataStream& dataStream, std::vector<ShaderPass>& passes,
        const std::string& assetName)
    {
        BinaryReader reader(dataStream);
        if (!ReadAssetHeader(reader, AssetFormat::GRES_MAGIC,
            static_cast<uint16_t>(RESOURCE_MATERIAL), AssetFormat::MATERIAL_VERSION))
        {
            GHOST_LOG_FORMAT_ERROR("Material::LoadPassesBinary: invalid header in '%s'.",
                assetName.c_str());
            return false;
        }

        uint32_t passCount = 0;
        if (!reader.ReadU32(passCount) || passCount > kMaxMaterialPasses)
        {
            GHOST_LOG_FORMAT_ERROR("Material::LoadPassesBinary: invalid pass count in '%s'.",
                assetName.c_str());
            return false;
        }

        passes.clear();
        passes.reserve(passCount);

        for (uint32_t passIndex = 0; passIndex < passCount; ++passIndex)
        {
            uint8_t passTypeValue = 0;
            if (!reader.ReadU8(passTypeValue) || passTypeValue >= RENDER_PASS_NUM)
            {
                GHOST_LOG_FORMAT_ERROR("Material::LoadPassesBinary: invalid pass type in '%s'.",
                    assetName.c_str());
                return false;
            }

            ShaderPass pass(static_cast<RenderPass>(passTypeValue));

            uint32_t defineCount = 0;
            if (!reader.ReadU32(defineCount) || defineCount > kMaxMaterialDefines)
            {
                GHOST_LOG_FORMAT_ERROR("Material::LoadPassesBinary: invalid define count in '%s'.",
                    assetName.c_str());
                return false;
            }

            for (uint32_t defineIndex = 0; defineIndex < defineCount; ++defineIndex)
            {
                std::string key;
                std::string value;
                if (!reader.ReadString(key, kMaxMaterialStringLength) ||
                    !reader.ReadString(value, kMaxMaterialStringLength) ||
                    key.empty())
                {
                    GHOST_LOG_FORMAT_ERROR("Material::LoadPassesBinary: invalid define in '%s'.",
                        assetName.c_str());
                    return false;
                }

                pass._defines[key] = value;
            }

            if (!reader.ReadString(pass._shaderSource, kMaxMaterialStringLength) ||
                pass._shaderSource.empty())
            {
                GHOST_LOG_FORMAT_ERROR("Material::LoadPassesBinary: missing shader source in '%s'.",
                    assetName.c_str());
                return false;
            }

            uint32_t entryCount = 0;
            if (!reader.ReadU32(entryCount) || entryCount > kMaxMaterialEntries)
            {
                GHOST_LOG_FORMAT_ERROR("Material::LoadPassesBinary: invalid shader entry count in '%s'.",
                    assetName.c_str());
                return false;
            }

            for (uint32_t entryIndex = 0; entryIndex < entryCount; ++entryIndex)
            {
                uint8_t shaderTypeValue = 0;
                std::string entry;
                if (!reader.ReadU8(shaderTypeValue) || shaderTypeValue >= SHADER_TYPE_NUM ||
                    !reader.ReadString(entry, kMaxMaterialStringLength) || entry.empty())
                {
                    GHOST_LOG_FORMAT_ERROR("Material::LoadPassesBinary: invalid shader entry in '%s'.",
                        assetName.c_str());
                    return false;
                }

                ShaderEntryDefinition definition;
                definition.type = static_cast<ShaderType>(shaderTypeValue);
                definition.entry = std::move(entry);
                pass._shaderEntries.push_back(std::move(definition));
            }

            uint32_t textureCount = 0;
            if (!reader.ReadU32(textureCount) || textureCount > kMaxMaterialSlots)
            {
                GHOST_LOG_FORMAT_ERROR("Material::LoadPassesBinary: invalid texture slot count in '%s'.",
                    assetName.c_str());
                return false;
            }

            for (uint32_t textureIndex = 0; textureIndex < textureCount; ++textureIndex)
            {
                TextureSlotDefinition definition;
                if (!reader.ReadString(definition.name, kMaxMaterialStringLength) ||
                    definition.name.empty() ||
                    !reader.ReadString(definition.path, kMaxMaterialStringLength))
                {
                    GHOST_LOG_FORMAT_ERROR("Material::LoadPassesBinary: invalid texture slot in '%s'.",
                        assetName.c_str());
                    return false;
                }

                pass._textureSlots.push_back(std::move(definition));
            }

            uint32_t samplerCount = 0;
            if (!reader.ReadU32(samplerCount) || samplerCount > kMaxMaterialSlots)
            {
                GHOST_LOG_FORMAT_ERROR("Material::LoadPassesBinary: invalid sampler slot count in '%s'.",
                    assetName.c_str());
                return false;
            }

            for (uint32_t samplerIndex = 0; samplerIndex < samplerCount; ++samplerIndex)
            {
                SamplerSlotDefinition definition;
                uint8_t minFilter = 0;
                uint8_t magFilter = 0;
                uint8_t mipFilter = 0;

                if (!reader.ReadString(definition.name, kMaxMaterialStringLength) ||
                    definition.name.empty() ||
                    !reader.ReadU8(minFilter) || !reader.ReadU8(magFilter) ||
                    !reader.ReadU8(mipFilter) ||
                    minFilter > FO_ANISOTROPIC || magFilter > FO_ANISOTROPIC ||
                    mipFilter > FO_ANISOTROPIC)
                {
                    GHOST_LOG_FORMAT_ERROR("Material::LoadPassesBinary: invalid sampler slot in '%s'.",
                        assetName.c_str());
                    return false;
                }

                definition.minFilter = static_cast<FilterOptions>(minFilter);
                definition.magFilter = static_cast<FilterOptions>(magFilter);
                definition.mipFilter = static_cast<FilterOptions>(mipFilter);
                pass._samplerSlots.push_back(std::move(definition));
            }

            passes.push_back(std::move(pass));
        }

        if (!reader.IsOk())
        {
            GHOST_LOG_FORMAT_ERROR("Material::LoadPassesBinary: %s", reader.GetError().c_str());
            return false;
        }

        return true;
    }

    bool Material::FinalizePasses(std::vector<ShaderPass>& passes,
        const std::string& assetName)
    {
        if (passes.empty())
            return true;

        auto renderDevice = Engine::GetInstance()->GetRenderDevice();
        if (!renderDevice)
        {
            GHOST_LOG_FORMAT_ERROR("Material::FinalizePasses: render device is not initialized.");
            return false;
        }

        for (ShaderPass& pass : passes)
        {
            if (pass._shaderSource.empty())
            {
                GHOST_LOG_FORMAT_ERROR("Material '%s': pass has no shader source.",
                    assetName.c_str());
                return false;
            }

            ShaderResourcePtr shader = GHOST_SMARTPOINTER_CAST(ShaderResource,
                ResourceManager::GetInstance()->AddResource(
                    RESOURCE_SHADER, pass._shaderSource, 0));

            if (!shader)
            {
                GHOST_LOG_FORMAT_ERROR("Material '%s': failed to load shader '%s'.",
                    assetName.c_str(), pass._shaderSource.c_str());
                return false;
            }

            pass._linkedShader = shader;

            for (const ShaderEntryDefinition& entry : pass._shaderEntries)
            {
                if (entry.type >= SHADER_TYPE_NUM || entry.entry.empty())
                {
                    GHOST_LOG_FORMAT_ERROR("Material '%s': invalid shader entry.",
                        assetName.c_str());
                    return false;
                }

                if (!renderDevice->CompileShader(entry.type, entry.entry.c_str(),
                    pass._defines, *shader))
                {
                    GHOST_LOG_FORMAT_ERROR("Material '%s': failed to compile %s entry '%s'.",
                        assetName.c_str(), ShaderTypeName[entry.type], entry.entry.c_str());
                    return false;
                }
            }

            if (shader->GetByteCodeByType(SHADER_VS))
                renderDevice->ReflectShader(shader, pass._params);

            for (const TextureSlotDefinition& slot : pass._textureSlots)
            {
                Texture2DResPtr texture = nullptr;
                if (!slot.path.empty())
                {
                    texture = GHOST_SMARTPOINTER_CAST(Texture2DRes,
                        ResourceManager::GetInstance()->AddResource(
                            RESOURCE_TEXTURE2D, slot.path, 0));

                    if (!texture)
                    {
                        GHOST_LOG_FORMAT_WARNNING("Material '%s': failed to load texture '%s'.",
                            assetName.c_str(), slot.path.c_str());
                    }
                }

                pass.ApplyTextureToSlot(slot.name, texture);
            }

            for (const SamplerSlotDefinition& slot : pass._samplerSlots)
            {
                Sampler sampler;
                sampler.SetFilter(slot.minFilter, slot.magFilter, slot.mipFilter);
                pass.ApplySamplerToSlot(slot.name, sampler);
            }
        }

        return true;
    }

    bool Material::SavePassesBinary(const std::vector<ShaderPass>& passes,
        DataStream& dataStream, const std::string& assetName)
    {
        if (!dataStream.IsWriteable())
        {
            GHOST_LOG_FORMAT_ERROR("Material::SavePassesBinary: stream is not writable.");
            return false;
        }

        if (passes.size() > kMaxMaterialPasses)
        {
            GHOST_LOG_FORMAT_ERROR("Material '%s': too many passes to save.", assetName.c_str());
            return false;
        }

        BinaryWriter writer(dataStream);
        if (!WriteAssetHeader(writer, AssetFormat::GRES_MAGIC,
            static_cast<uint16_t>(RESOURCE_MATERIAL), AssetFormat::MATERIAL_VERSION))
        {
            GHOST_LOG_FORMAT_ERROR("Material::SavePassesBinary: failed to write header.");
            return false;
        }

        writer.WriteU32(static_cast<uint32_t>(passes.size()));

        for (const ShaderPass& pass : passes)
        {
            if (pass._shaderSource.empty() ||
                pass._defines.size() > kMaxMaterialDefines ||
                pass._shaderEntries.size() > kMaxMaterialEntries ||
                pass._textureSlots.size() > kMaxMaterialSlots ||
                pass._samplerSlots.size() > kMaxMaterialSlots ||
                pass._passType >= RENDER_PASS_NUM)
            {
                GHOST_LOG_FORMAT_ERROR("Material '%s': invalid pass data.", assetName.c_str());
                return false;
            }

            writer.WriteU8(static_cast<uint8_t>(pass._passType));

            const auto defines = GetSortedDefines(pass._defines);
            writer.WriteU32(static_cast<uint32_t>(defines.size()));
            for (const auto& define : defines)
            {
                writer.WriteString(define.first);
                writer.WriteString(define.second);
            }

            writer.WriteString(pass._shaderSource);

            writer.WriteU32(static_cast<uint32_t>(pass._shaderEntries.size()));
            for (const ShaderEntryDefinition& entry : pass._shaderEntries)
            {
                if (entry.type >= SHADER_TYPE_NUM || entry.entry.empty())
                {
                    GHOST_LOG_FORMAT_ERROR("Material '%s': invalid shader entry.", assetName.c_str());
                    return false;
                }

                writer.WriteU8(static_cast<uint8_t>(entry.type));
                writer.WriteString(entry.entry);
            }

            writer.WriteU32(static_cast<uint32_t>(pass._textureSlots.size()));
            for (const TextureSlotDefinition& slot : pass._textureSlots)
            {
                if (slot.name.empty())
                {
                    GHOST_LOG_FORMAT_ERROR("Material '%s': texture slot without a name.",
                        assetName.c_str());
                    return false;
                }

                writer.WriteString(slot.name);
                writer.WriteString(slot.path);
            }

            writer.WriteU32(static_cast<uint32_t>(pass._samplerSlots.size()));
            for (const SamplerSlotDefinition& slot : pass._samplerSlots)
            {
                if (slot.name.empty() || !IsValidFilter(slot.minFilter) ||
                    !IsValidFilter(slot.magFilter) || !IsValidFilter(slot.mipFilter))
                {
                    GHOST_LOG_FORMAT_ERROR("Material '%s': invalid sampler slot.", assetName.c_str());
                    return false;
                }

                writer.WriteString(slot.name);
                writer.WriteU8(static_cast<uint8_t>(slot.minFilter));
                writer.WriteU8(static_cast<uint8_t>(slot.magFilter));
                writer.WriteU8(static_cast<uint8_t>(slot.mipFilter));
            }
        }

        if (!writer.IsOk())
        {
            GHOST_LOG_FORMAT_ERROR("Material::SavePassesBinary: %s", writer.GetError().c_str());
            return false;
        }

        return true;
    }

    bool Material::SavePassesXml(const std::vector<ShaderPass>& passes,
        DataStream& dataStream, const std::string& assetName)
    {
        if (!dataStream.IsWriteable())
        {
            GHOST_LOG_FORMAT_ERROR("Material::SavePassesXml: stream is not writable.");
            return false;
        }

        tinyxml2::XMLDocument document;
        document.InsertFirstChild(document.NewDeclaration());

        tinyxml2::XMLElement* root = document.NewElement("Material");
        root->SetAttribute("version", 1);
        document.InsertEndChild(root);

        for (const ShaderPass& pass : passes)
        {
            if (pass._shaderSource.empty())
            {
                GHOST_LOG_FORMAT_ERROR("Material '%s': pass has no shader source.", assetName.c_str());
                return false;
            }

            tinyxml2::XMLElement* passElement = document.NewElement("Pass");
            passElement->SetAttribute("name", GetPassName(pass._passType));

            const auto defines = GetSortedDefines(pass._defines);
            if (!defines.empty())
            {
                tinyxml2::XMLElement* definesElement = document.NewElement("Defines");
                for (const auto& define : defines)
                {
                    tinyxml2::XMLElement* defineElement = document.NewElement(define.first.c_str());
                    defineElement->SetAttribute("value", define.second.c_str());
                    definesElement->InsertEndChild(defineElement);
                }

                passElement->InsertEndChild(definesElement);
            }

            tinyxml2::XMLElement* shaderElement = document.NewElement("Shader");
            shaderElement->SetAttribute("source", pass._shaderSource.c_str());

            for (const ShaderEntryDefinition& entry : pass._shaderEntries)
            {
                if (entry.type >= SHADER_TYPE_NUM || entry.entry.empty())
                {
                    GHOST_LOG_FORMAT_ERROR("Material '%s': invalid shader entry.", assetName.c_str());
                    return false;
                }

                tinyxml2::XMLElement* entryElement = document.NewElement(ShaderTypeName[entry.type]);
                entryElement->SetAttribute("entry", entry.entry.c_str());
                shaderElement->InsertEndChild(entryElement);
            }

            passElement->InsertEndChild(shaderElement);

            if (!pass._textureSlots.empty())
            {
                tinyxml2::XMLElement* texturesElement = document.NewElement("Textures");
                for (const TextureSlotDefinition& slot : pass._textureSlots)
                {
                    if (slot.name.empty())
                    {
                        GHOST_LOG_FORMAT_ERROR("Material '%s': texture slot without a name.",
                            assetName.c_str());
                        return false;
                    }

                    tinyxml2::XMLElement* textureElement = document.NewElement(slot.name.c_str());
                    textureElement->SetAttribute("path", slot.path.c_str());
                    texturesElement->InsertEndChild(textureElement);
                }

                passElement->InsertEndChild(texturesElement);
            }

            if (!pass._samplerSlots.empty())
            {
                tinyxml2::XMLElement* samplersElement = document.NewElement("Samplers");
                for (const SamplerSlotDefinition& slot : pass._samplerSlots)
                {
                    if (slot.name.empty() || !IsValidFilter(slot.minFilter) ||
                        !IsValidFilter(slot.magFilter) || !IsValidFilter(slot.mipFilter))
                    {
                        GHOST_LOG_FORMAT_ERROR("Material '%s': invalid sampler slot.", assetName.c_str());
                        return false;
                    }

                    tinyxml2::XMLElement* samplerElement = document.NewElement(slot.name.c_str());

                    tinyxml2::XMLElement* minElement = document.NewElement("MIN");
                    minElement->SetAttribute("value", GetFilterName(slot.minFilter));
                    samplerElement->InsertEndChild(minElement);

                    tinyxml2::XMLElement* magElement = document.NewElement("MAG");
                    magElement->SetAttribute("value", GetFilterName(slot.magFilter));
                    samplerElement->InsertEndChild(magElement);

                    tinyxml2::XMLElement* mipElement = document.NewElement("MIP");
                    mipElement->SetAttribute("value", GetFilterName(slot.mipFilter));
                    samplerElement->InsertEndChild(mipElement);

                    samplersElement->InsertEndChild(samplerElement);
                }

                passElement->InsertEndChild(samplersElement);
            }

            root->InsertEndChild(passElement);
        }

        tinyxml2::XMLPrinter printer;
        document.Print(&printer);

        const std::size_t length = std::strlen(printer.CStr());
        if (dataStream.Write(printer.CStr(), length) != length)
        {
            GHOST_LOG_FORMAT_ERROR("Material::SavePassesXml: write failed.");
            return false;
        }

        return true;
    }

    void Material::ApplyToRenderPass(RenderPass pass)
    {
        ShaderPass* currentPass = GetShaderPass(pass);
        if (currentPass == nullptr || currentPass->_linkedShader == nullptr)
            return;

        if (!currentPass->_handwareShader)
        {
            auto renderDevice = Engine::GetInstance()->GetRenderDevice();
            currentPass->_handwareShader = renderDevice->CreateShader(currentPass->_linkedShader);
        }

        auto rendersystem = Engine::GetInstance()->GetRenderSystem();

        Shader* hardwareShader = currentPass->_handwareShader;
        if (hardwareShader && hardwareShader->IsValid())
        {
            rendersystem->SetShader(hardwareShader);

            const ShaderParams& params = currentPass->_params;

            for (const TextureVariableInfo& texInfo : params._textures)
            {
                auto texture = GPUObjectManager::GetInstance()->GetOrCreateTexture(texInfo._texture);
                rendersystem->SetTexture(texInfo._bindPoint, texture);
            }

            for (const SamplerInfo& sampler : params._samplers)
                rendersystem->SetSamplerState(sampler._bindPoint, sampler._sampler);
        }
    }

    ShaderPass* Material::GetShaderPass(RenderPass pass)
    {
        for (unsigned i = 0; i < _passes.size(); ++i)
        {
            if (_passes[i]._passType == pass)
                return &_passes[i];
        }

        return nullptr;
    }
}

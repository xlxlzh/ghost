#include <algorithm>
#include <cstring>
#include <map>
#include <sstream>
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
            if (!name)
            {
                GHOST_LOG_FORMAT_WARNNING("Material: unknown filter value, using linear.");
                return FO_LINEAR;
            }

            if (strcmp(name, "FILTER_NONE") == 0) return FO_NONE;
            if (strcmp(name, "FILTER_POINT") == 0) return FO_POINT;
            if (strcmp(name, "FILTER_LINEAR") == 0) return FO_LINEAR;
            if (strcmp(name, "FILTER_ANISOTROPIC") == 0) return FO_ANISOTROPIC;

            GHOST_LOG_FORMAT_WARNNING("Material: unknown filter value '%s', using linear.", name);
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

    static constexpr uint32_t kMaxMaterialParameters = 64;

    static int GetMaterialParameterComponentCount(MaterialParameterType type)
    {
        switch (type)
        {
        case MaterialParameterType::Float:
        case MaterialParameterType::Int:
        case MaterialParameterType::Bool:
            return 1;
        case MaterialParameterType::Float2:
            return 2;
        case MaterialParameterType::Float3:
            return 3;
        case MaterialParameterType::Float4:
        case MaterialParameterType::Color:
            return 4;
        default:
            return 0;
        }
    }

    static unsigned GetMaterialParameterByteSize(MaterialParameterType type)
    {
        switch (type)
        {
        case MaterialParameterType::Float:
        case MaterialParameterType::Int:
        case MaterialParameterType::Bool:
            return 4;
        case MaterialParameterType::Float2:
            return 8;
        case MaterialParameterType::Float3:
            return 12;
        case MaterialParameterType::Float4:
        case MaterialParameterType::Color:
            return 16;
        default:
            return 0;
        }
    }

    static bool IsMaterialParameterTypeCompatible(MaterialParameterType valueType,
        MaterialParameterType reflectedType)
    {
        if (valueType == reflectedType)
            return true;

        return (valueType == MaterialParameterType::Color &&
                reflectedType == MaterialParameterType::Float4) ||
            (valueType == MaterialParameterType::Float4 &&
                reflectedType == MaterialParameterType::Color);
    }

    static const char* GetMaterialParameterTypeName(MaterialParameterType type)
    {
        switch (type)
        {
        case MaterialParameterType::Float:
            return "Float";
        case MaterialParameterType::Float2:
            return "Float2";
        case MaterialParameterType::Float3:
            return "Float3";
        case MaterialParameterType::Float4:
            return "Float4";
        case MaterialParameterType::Color:
            return "Color";
        case MaterialParameterType::Int:
            return "Int";
        case MaterialParameterType::Bool:
            return "Bool";
        default:
            return nullptr;
        }
    }

    static MaterialParameterType ParseMaterialParameterType(const char* name)
    {
        if (!name)
            return MaterialParameterType::Unknown;

        for (int index = 1; index <= static_cast<int>(MaterialParameterType::Bool); ++index)
        {
            const MaterialParameterType type = static_cast<MaterialParameterType>(index);
            const char* typeName = GetMaterialParameterTypeName(type);
            if (typeName && strcmp(name, typeName) == 0)
                return type;
        }

        return MaterialParameterType::Unknown;
    }

    static bool ParseMaterialParameterValue(const char* text, MaterialParameterValue& value)
    {
        if (!text)
            return false;

        const int componentCount = GetMaterialParameterComponentCount(value._type);
        if (componentCount == 0)
            return false;

        std::istringstream stream(text);
        for (int index = 0; index < componentCount; ++index)
        {
            if (!(stream >> value._values[index]))
                return false;
        }

        return true;
    }

    static bool WriteMaterialParameterBytes(std::vector<unsigned char>& data,
        const ConstBufferVariable& variable, const MaterialParameterValue& value)
    {
        if (!IsMaterialParameterTypeCompatible(value._type, variable._type))
            return false;

        const unsigned byteSize = GetMaterialParameterByteSize(variable._type);
        if (byteSize == 0 || variable._offset + byteSize > data.size())
            return false;

        unsigned char* dest = data.data() + variable._offset;
        switch (variable._type)
        {
        case MaterialParameterType::Float:
        {
            const float scalar = value._values[0];
            memcpy(dest, &scalar, sizeof(scalar));
            break;
        }
        case MaterialParameterType::Float2:
            memcpy(dest, value._values.data(), sizeof(float) * 2);
            break;
        case MaterialParameterType::Float3:
            memcpy(dest, value._values.data(), sizeof(float) * 3);
            break;
        case MaterialParameterType::Float4:
        case MaterialParameterType::Color:
            memcpy(dest, value._values.data(), sizeof(float) * 4);
            break;
        case MaterialParameterType::Int:
        {
            const int32_t integer = static_cast<int32_t>(value._values[0]);
            memcpy(dest, &integer, sizeof(integer));
            break;
        }
        case MaterialParameterType::Bool:
        {
            const int32_t boolean = value._values[0] != 0.0f ? 1 : 0;
            memcpy(dest, &boolean, sizeof(boolean));
            break;
        }
        default:
            return false;
        }

        return true;
    }

    static void BuildMaterialParameterData(const MaterialParameterMap& defaults,
        const MaterialParameterMap* overrides, const ConstBufferInfo& bufferInfo,
        std::vector<unsigned char>& data)
    {
        data.assign(bufferInfo._bufferSize, 0);

        for (const ConstBufferVariable& variable : bufferInfo._variables)
        {
            const MaterialParameterValue* value = nullptr;

            if (overrides)
            {
                auto found = overrides->find(variable._name);
                if (found != overrides->end())
                    value = &found->second;
            }

            if (!value)
            {
                auto found = defaults.find(variable._name);
                if (found != defaults.end())
                    value = &found->second;
            }

            if (!value)
                continue;

            if (!WriteMaterialParameterBytes(data, variable, *value))
            {
                GHOST_LOG_FORMAT_WARNNING("Material parameter '%s' is missing or has a mismatched type.",
                    variable._name.c_str());
            }
        }
    }

    static void ReadMaterialParametersXml(tinyxml2::XMLElement* root,
        MaterialParameterMap& parameters)
    {
        tinyxml2::XMLElement* parametersElement = root->FirstChildElement("Parameters");
        if (!parametersElement)
            return;

        for (tinyxml2::XMLElement* element = parametersElement->FirstChildElement();
            element != nullptr;
            element = element->NextSiblingElement())
        {
            const char* name = element->Attribute("name");
            if (!name || !name[0])
                continue;

            const MaterialParameterType type = ParseMaterialParameterType(element->Name());
            if (type == MaterialParameterType::Unknown)
                continue;

            MaterialParameterValue value;
            value._type = type;

            const char* valueText = element->Attribute("value");
            if (!ParseMaterialParameterValue(valueText, value))
            {
                GHOST_LOG_FORMAT_WARNNING("Material parameter '%s' has an invalid value.", name);
                continue;
            }

            parameters[name] = value;
        }
    }

    static void WriteMaterialParametersXml(tinyxml2::XMLDocument& document,
        tinyxml2::XMLElement* root, const MaterialParameterMap& parameters)
    {
        if (parameters.empty())
            return;

        std::vector<std::pair<std::string, MaterialParameterValue>> sorted(parameters.begin(), parameters.end());
        std::sort(sorted.begin(), sorted.end(),
            [](const auto& lhs, const auto& rhs)
            {
                return lhs.first < rhs.first;
            });

        tinyxml2::XMLElement* parametersElement = document.NewElement("Parameters");

        for (const auto& entry : sorted)
        {
            const char* typeName = GetMaterialParameterTypeName(entry.second._type);
            if (!typeName)
                continue;

            tinyxml2::XMLElement* element = document.NewElement(typeName);
            element->SetAttribute("name", entry.first.c_str());

            std::ostringstream valueStream;
            const int componentCount = GetMaterialParameterComponentCount(entry.second._type);
            for (int index = 0; index < componentCount; ++index)
            {
                if (index > 0)
                    valueStream << ' ';
                valueStream << entry.second._values[index];
            }

            element->SetAttribute("value", valueStream.str().c_str());
            parametersElement->InsertEndChild(element);
        }

        root->InsertEndChild(parametersElement);
    }

    static bool ReadMaterialParametersBinary(BinaryReader& reader,
        MaterialParameterMap& parameters, const std::string& assetName)
    {
        uint32_t count = 0;
        if (!reader.ReadU32(count) || count > kMaxMaterialParameters)
        {
            GHOST_LOG_FORMAT_ERROR("Material '%s': invalid parameter count.", assetName.c_str());
            return false;
        }

        for (uint32_t index = 0; index < count; ++index)
        {
            std::string name;
            uint8_t typeValue = 0;
            if (!reader.ReadString(name, kMaxMaterialStringLength) || name.empty() ||
                !reader.ReadU8(typeValue))
            {
                GHOST_LOG_FORMAT_ERROR("Material '%s': invalid parameter entry.", assetName.c_str());
                return false;
            }

            const MaterialParameterType type = static_cast<MaterialParameterType>(typeValue);
            if (type <= MaterialParameterType::Unknown || type > MaterialParameterType::Bool)
            {
                GHOST_LOG_FORMAT_ERROR("Material '%s': invalid parameter type.", assetName.c_str());
                return false;
            }

            MaterialParameterValue value;
            value._type = type;

            for (int component = 0; component < 4; ++component)
            {
                if (!reader.ReadF32(value._values[component]))
                {
                    GHOST_LOG_FORMAT_ERROR("Material '%s': invalid parameter value.", assetName.c_str());
                    return false;
                }
            }

            parameters[name] = value;
        }

        return true;
    }

    static bool WriteMaterialParametersBinary(BinaryWriter& writer,
        const MaterialParameterMap& parameters)
    {
        if (parameters.size() > kMaxMaterialParameters)
            return false;

        std::vector<std::pair<std::string, MaterialParameterValue>> sorted(parameters.begin(), parameters.end());
        std::sort(sorted.begin(), sorted.end(),
            [](const auto& lhs, const auto& rhs)
            {
                return lhs.first < rhs.first;
            });

        writer.WriteU32(static_cast<uint32_t>(sorted.size()));

        for (const auto& entry : sorted)
        {
            const char* typeName = GetMaterialParameterTypeName(entry.second._type);
            if (entry.first.empty() || !typeName)
                return false;

            writer.WriteString(entry.first);
            writer.WriteU8(static_cast<uint8_t>(entry.second._type));

            for (int component = 0; component < 4; ++component)
                writer.WriteF32(entry.second._values[component]);
        }

        return writer.IsOk();
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

    const ConstBufferInfo* ShaderPass::GetConstBufferInfo(ShaderType type, const std::string& name) const
    {
        if (type >= SHADER_TYPE_NUM)
            return nullptr;

        for (const ConstBufferInfo& info : _params._constBuffers[static_cast<std::size_t>(type)])
        {
            if (info._name == name)
                return &info;
        }

        return nullptr;
    }

    bool ShaderPass::GetTextureBindPoint(const std::string& name, unsigned& bindPoint) const
    {
        for (const TextureVariableInfo& info : _params._textures)
        {
            if (info._name == name)
            {
                bindPoint = info._bindPoint;
                return true;
            }
        }

        return false;
    }

    bool ShaderPass::GetSamplerBindPoint(const std::string& name, unsigned& bindPoint) const
    {
        for (const SamplerInfo& info : _params._samplers)
        {
            if (info._name == name)
            {
                bindPoint = info._bindPoint;
                return true;
            }
        }

        return false;
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
        MaterialParameterMap parameters;
        const std::string assetName = dataStream.GetName();

        bool parsed = false;
        switch (DetectAssetEncoding(dataStream))
        {
        case AssetEncoding::Gres:
            parsed = LoadPassesBinary(dataStream, passes, parameters, assetName);
            break;

        case AssetEncoding::Xml:
            parsed = LoadPassesXml(dataStream, passes, parameters, assetName);
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
        _parameters.swap(parameters);
        _loaded = true;
        return true;
    }

    bool Material::Save(DataStream& dataStream)
    {
        return SavePassesBinary(_passes, _parameters, dataStream, GetName());
    }

    bool Material::SaveXml(DataStream& dataStream)
    {
        return SavePassesXml(_passes, _parameters, dataStream, GetName());
    }

    bool Material::LoadPassesXml(DataStream& dataStream, std::vector<ShaderPass>& passes,
        MaterialParameterMap& parameters, const std::string& assetName)
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
        parameters.clear();

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

        ReadMaterialParametersXml(root, parameters);

        return true;
    }

    bool Material::LoadPassesBinary(DataStream& dataStream, std::vector<ShaderPass>& passes,
        MaterialParameterMap& parameters, const std::string& assetName)
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
        parameters.clear();

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

        if (reader.RemainingBytes() > 0)
        {
            if (!ReadMaterialParametersBinary(reader, parameters, assetName))
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
            }

            pass._compiledShader = renderDevice->GetOrCreateCompiledShader(
                shader, pass._shaderEntries, pass._defines);

            if (!pass._compiledShader)
            {
                GHOST_LOG_FORMAT_ERROR("Material '%s': failed to compile shader '%s'.",
                    assetName.c_str(), pass._shaderSource.c_str());
                return false;
            }

            if (pass._compiledShader->GetByteCodeByType(SHADER_VS))
                renderDevice->ReflectShader(pass._compiledShader, pass._params);

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
        const MaterialParameterMap& parameters, DataStream& dataStream,
        const std::string& assetName)
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

        if (!WriteMaterialParametersBinary(writer, parameters))
        {
            GHOST_LOG_FORMAT_ERROR("Material '%s': failed to write parameters.", assetName.c_str());
            return false;
        }

        if (!writer.IsOk())
        {
            GHOST_LOG_FORMAT_ERROR("Material::SavePassesBinary: %s", writer.GetError().c_str());
            return false;
        }

        return true;
    }

    bool Material::SavePassesXml(const std::vector<ShaderPass>& passes,
        const MaterialParameterMap& parameters, DataStream& dataStream,
        const std::string& assetName)
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

        WriteMaterialParametersXml(document, root, parameters);

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
            currentPass->_handwareShader = renderDevice->CreateShader(currentPass->_compiledShader);
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

            _parameterBinding.Apply(_parameters, nullptr, *currentPass);
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

    void MaterialParameterBinding::Apply(const MaterialParameterMap& defaults,
        const MaterialParameterMap* overrides, ShaderPass& pass)
    {
        auto renderDevice = Engine::GetInstance()->GetRenderDevice();
        auto renderSystem = Engine::GetInstance()->GetRenderSystem();
        if (!renderDevice || !renderSystem)
            return;

        const std::size_t passIndex = static_cast<std::size_t>(pass.GetRenderPass());
        if (passIndex >= static_cast<std::size_t>(RENDER_PASS_NUM))
            return;

        for (std::size_t stage = 0; stage < static_cast<std::size_t>(SHADER_TYPE_NUM); ++stage)
        {
            const ConstBufferInfo* bufferInfo = pass.GetConstBufferInfo(
                static_cast<ShaderType>(stage), MaterialParameterBufferName);
            if (!bufferInfo || bufferInfo->_bufferSize == 0)
                continue;

            BufferEntry& entry = _buffers[passIndex][stage];
            if (!entry._buffer || entry._size != bufferInfo->_bufferSize)
            {
                entry._buffer = renderDevice->CreateConstBuffer(
                    bufferInfo->_bufferSize, USAGE_DYNAMIC, MaterialParameterBufferName);
                entry._size = bufferInfo->_bufferSize;
            }

            if (!entry._buffer)
                continue;

            std::vector<unsigned char> data;
            BuildMaterialParameterData(defaults, overrides, *bufferInfo, data);

            entry._buffer->WriteData(0, bufferInfo->_bufferSize, data.data(), true);
            renderSystem->SetConstBuffer(static_cast<ShaderType>(stage), entry._buffer);
        }
    }

    MaterialInstanceDynamic::MaterialInstanceDynamic(const MaterialPtr& baseMaterial) : _baseMaterial(baseMaterial)
    {
    }

    Material* MaterialInstanceDynamic::GetBaseMaterial()
    {
        return _baseMaterial.get();
    }

    void MaterialInstanceDynamic::ApplyToRenderPass(RenderPass pass)
    {
        if (!_baseMaterial)
            return;

        _baseMaterial->ApplyToRenderPass(pass);

        ShaderPass* shaderPass = _baseMaterial->GetShaderPass(pass);
        if (!shaderPass)
            return;

        auto renderSystem = Engine::GetInstance()->GetRenderSystem();

        for (const auto& textureOverride : _textureOverrides)
        {
            unsigned bindPoint = 0;
            if (!shaderPass->GetTextureBindPoint(textureOverride.first, bindPoint))
                continue;

            Texture2DPtr texture = textureOverride.second
                ? GPUObjectManager::GetInstance()->GetOrCreateTexture(textureOverride.second)
                : nullptr;
            renderSystem->SetTexture(bindPoint, texture);
        }

        for (const auto& samplerOverride : _samplerOverrides)
        {
            unsigned bindPoint = 0;
            if (!shaderPass->GetSamplerBindPoint(samplerOverride.first, bindPoint))
                continue;

            renderSystem->SetSamplerState(bindPoint, samplerOverride.second);
        }

        if (!_parameterOverrides.empty())
            _parameterBinding.Apply(_baseMaterial->GetParameters(), &_parameterOverrides, *shaderPass);
    }

    void MaterialInstanceDynamic::SetTexture(const std::string& slot, const Texture2DResPtr& texture)
    {
        if (slot.empty())
            return;

        _textureOverrides[slot] = texture;
        ++_version;
    }

    void MaterialInstanceDynamic::ClearTexture(const std::string& slot)
    {
        _textureOverrides.erase(slot);
        ++_version;
    }

    void MaterialInstanceDynamic::SetSampler(const std::string& slot, const Sampler& sampler)
    {
        if (slot.empty())
            return;

        _samplerOverrides[slot] = sampler;
        ++_version;
    }

    void MaterialInstanceDynamic::ClearSampler(const std::string& slot)
    {
        _samplerOverrides.erase(slot);
        ++_version;
    }

    void MaterialInstanceDynamic::SetParameter(const std::string& name,
        const MaterialParameterValue& value)
    {
        if (name.empty() || value._type == MaterialParameterType::Unknown)
            return;

        _parameterOverrides[name] = value;
        ++_version;
    }

    void MaterialInstanceDynamic::SetFloat(const std::string& name, float value)
    {
        MaterialParameterValue parameter;
        parameter._type = MaterialParameterType::Float;
        parameter._values[0] = value;
        SetParameter(name, parameter);
    }

    void MaterialInstanceDynamic::SetFloat2(const std::string& name, float x, float y)
    {
        MaterialParameterValue parameter;
        parameter._type = MaterialParameterType::Float2;
        parameter._values = { x, y, 0.0f, 0.0f };
        SetParameter(name, parameter);
    }

    void MaterialInstanceDynamic::SetFloat3(const std::string& name, float x, float y, float z)
    {
        MaterialParameterValue parameter;
        parameter._type = MaterialParameterType::Float3;
        parameter._values = { x, y, z, 0.0f };
        SetParameter(name, parameter);
    }

    void MaterialInstanceDynamic::SetFloat4(const std::string& name,
        float x, float y, float z, float w)
    {
        MaterialParameterValue parameter;
        parameter._type = MaterialParameterType::Float4;
        parameter._values = { x, y, z, w };
        SetParameter(name, parameter);
    }

    void MaterialInstanceDynamic::SetColor(const std::string& name,
        float r, float g, float b, float a /* = 1.0f */)
    {
        MaterialParameterValue parameter;
        parameter._type = MaterialParameterType::Color;
        parameter._values = { r, g, b, a };
        SetParameter(name, parameter);
    }

    void MaterialInstanceDynamic::SetInt(const std::string& name, int value)
    {
        MaterialParameterValue parameter;
        parameter._type = MaterialParameterType::Int;
        parameter._values[0] = static_cast<float>(value);
        SetParameter(name, parameter);
    }

    void MaterialInstanceDynamic::SetBool(const std::string& name, bool value)
    {
        MaterialParameterValue parameter;
        parameter._type = MaterialParameterType::Bool;
        parameter._values[0] = value ? 1.0f : 0.0f;
        SetParameter(name, parameter);
    }

    void MaterialInstanceDynamic::ClearParameter(const std::string& name)
    {
        _parameterOverrides.erase(name);
        ++_version;
    }
}

#include "ShaderAsset.h"

#include <cstring>

#include "BinaryArchive.h"
#include "LogManager.h"
#include "tinyxml2.h"

namespace ghost
{
    static constexpr uint32_t kMaxShaderPasses = 16;
    static constexpr uint32_t kMaxShaderEntries = 16;
    static constexpr uint32_t kMaxShaderStringLength = 8192;

    static RenderPass ParsePassName(const char* name)
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

        return RENDER_PASS_FORWARD;
    }

    static const char* GetPassName(RenderPass pass)
    {
        switch (pass)
        {
        case RENDER_PASS_FORWARD:
            return "forward";
        case RENDER_PASS_SHADOW:
            return "shadow";
        case RENDER_PASS_DEPTH:
            return "depth";
        case RENDER_PASS_DEFERRED:
            return "deferred";
        default:
            return "forward";
        }
    }

    static bool ParseBool(const char* text, bool& value)
    {
        if (!text)
            return false;

        if (strcmp(text, "true") == 0 || strcmp(text, "1") == 0)
        {
            value = true;
            return true;
        }

        if (strcmp(text, "false") == 0 || strcmp(text, "0") == 0)
        {
            value = false;
            return true;
        }

        return false;
    }

    static bool ParseCullMode(const char* text, CullMode& value)
    {
        if (!text)
            return false;

        if (strcmp(text, "cw") == 0)
        {
            value = CULL_CW;
            return true;
        }

        if (strcmp(text, "ccw") == 0)
        {
            value = CULL_CCW;
            return true;
        }

        if (strcmp(text, "none") == 0)
        {
            value = CULL_NONE;
            return true;
        }

        return false;
    }

    static bool ParseCompareFunction(const char* text, CompareFunction& value)
    {
        if (!text)
            return false;

        if (strcmp(text, "never") == 0)
            value = COMPARISON_NEVER;
        else if (strcmp(text, "always") == 0)
            value = COMPARISON_ALWAYS;
        else if (strcmp(text, "less") == 0)
            value = COMPARISON_LESS;
        else if (strcmp(text, "lessEqual") == 0)
            value = COMPARISON_LESS_EQUAL;
        else if (strcmp(text, "equal") == 0)
            value = COMPARISON_EQUAL;
        else if (strcmp(text, "notEqual") == 0)
            value = COMPARISON_NOT_EQUAL;
        else if (strcmp(text, "greaterEqual") == 0)
            value = COMPARISON_GREATER_EQUAL;
        else if (strcmp(text, "greater") == 0)
            value = COMPARISON_GREATER;
        else
            return false;

        return true;
    }

    static const char* GetCullModeName(CullMode value)
    {
        switch (value)
        {
        case CULL_CW:
            return "cw";
        case CULL_CCW:
            return "ccw";
        case CULL_NONE:
            return "none";
        default:
            return "cw";
        }
    }

    static const char* GetCompareFunctionName(CompareFunction value)
    {
        switch (value)
        {
        case COMPARISON_NEVER:
            return "never";
        case COMPARISON_ALWAYS:
            return "always";
        case COMPARISON_LESS:
            return "less";
        case COMPARISON_LESS_EQUAL:
            return "lessEqual";
        case COMPARISON_EQUAL:
            return "equal";
        case COMPARISON_NOT_EQUAL:
            return "notEqual";
        case COMPARISON_GREATER_EQUAL:
            return "greaterEqual";
        case COMPARISON_GREATER:
            return "greater";
        default:
            return "lessEqual";
        }
    }

    ShaderAsset::ShaderAsset() : Resource(RESOURCE_SHADER_ASSET, "", 0)
    {
    }

    bool ShaderAsset::Load(DataStream& dataStream)
    {
        std::string text;
        if (!ReadStreamAsText(dataStream, text))
        {
            GHOST_LOG_FORMAT_ERROR("ShaderAsset::Load: empty or unreadable stream '%s'.",
                dataStream.GetName().c_str());
            return false;
        }

        tinyxml2::XMLDocument document;
        if (document.Parse(text.c_str(), text.size()) != tinyxml2::XML_SUCCESS)
        {
            GHOST_LOG_FORMAT_ERROR("ShaderAsset::Load: XML parse error in '%s'.",
                dataStream.GetName().c_str());
            return false;
        }

        tinyxml2::XMLElement* root = document.RootElement();
        if (!root || (strcmp(root->Name(), "ShaderAsset") != 0 &&
            strcmp(root->Name(), "Shader") != 0))
        {
            GHOST_LOG_FORMAT_ERROR("ShaderAsset::Load: missing <ShaderAsset> root in '%s'.",
                dataStream.GetName().c_str());
            return false;
        }

        const int version = root->IntAttribute("version", 1);
        if (version > 1)
        {
            GHOST_LOG_FORMAT_WARNNING("ShaderAsset::Load: unsupported version %d in '%s'.",
                version, dataStream.GetName().c_str());
        }

        _passes.clear();

        for (tinyxml2::XMLElement* passElement = root->FirstChildElement("Pass");
            passElement != nullptr;
            passElement = passElement->NextSiblingElement("Pass"))
        {
            if (_passes.size() >= kMaxShaderPasses)
            {
                GHOST_LOG_FORMAT_ERROR("ShaderAsset::Load: too many passes in '%s'.",
                    dataStream.GetName().c_str());
                return false;
            }

            const char* passName = passElement->Attribute("name");
            if (!passName || !passName[0])
            {
                GHOST_LOG_FORMAT_ERROR("ShaderAsset::Load: pass without a name in '%s'.",
                    dataStream.GetName().c_str());
                return false;
            }

            ShaderPassDefinition definition;
            definition._passType = ParsePassName(passName);

            tinyxml2::XMLElement* sourceElement = passElement->FirstChildElement("Source");
            const char* sourcePath = sourceElement ? sourceElement->Attribute("path") : nullptr;
            if (!sourcePath || !sourcePath[0])
                sourcePath = passElement->Attribute("source");

            if (!sourcePath || !sourcePath[0])
            {
                GHOST_LOG_FORMAT_ERROR("ShaderAsset::Load: pass without a source in '%s'.",
                    dataStream.GetName().c_str());
                return false;
            }

            definition._sourcePath = sourcePath;

            for (tinyxml2::XMLElement* child = passElement->FirstChildElement();
                child != nullptr;
                child = child->NextSiblingElement())
            {
                const char* childName = child->Name();
                if (!childName || strcmp(childName, "Source") == 0 ||
                    strcmp(childName, "Defines") == 0 ||
                    strcmp(childName, "RenderState") == 0)
                    continue;

                const ShaderType type = GetShaderTypeByName(childName);
                if (type >= SHADER_TYPE_NUM)
                    continue;

                const char* entry = child->Attribute("entry");
                if (!entry || !entry[0])
                    continue;

                if (definition._entries.size() >= kMaxShaderEntries)
                {
                    GHOST_LOG_FORMAT_ERROR("ShaderAsset::Load: too many entries in '%s'.",
                        dataStream.GetName().c_str());
                    return false;
                }

                ShaderEntryDefinition entryDefinition;
                entryDefinition.type = type;
                entryDefinition.entry = entry;
                definition._entries.push_back(std::move(entryDefinition));
            }

            if (definition._entries.empty())
            {
                GHOST_LOG_FORMAT_ERROR("ShaderAsset::Load: pass without entries in '%s'.",
                    dataStream.GetName().c_str());
                return false;
            }

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
                    definition._defines[name] = value ? value : "";
                }
            }

            tinyxml2::XMLElement* stateElement = passElement->FirstChildElement("RenderState");
            if (stateElement)
            {
                bool parsed = false;

                if (ParseBool(stateElement->Attribute("depthTest"), parsed))
                {
                    definition._renderState._overrideDepthTest = true;
                    definition._renderState._depthTest = parsed;
                }

                if (ParseBool(stateElement->Attribute("depthWrite"), parsed))
                {
                    definition._renderState._overrideDepthWrite = true;
                    definition._renderState._depthWrite = parsed;
                }

                CompareFunction depthFunc = COMPARISON_LESS_EQUAL;
                if (ParseCompareFunction(stateElement->Attribute("depthFunc"), depthFunc))
                {
                    definition._renderState._overrideDepthFunc = true;
                    definition._renderState._depthFunc = depthFunc;
                }

                CullMode cullMode = CULL_CW;
                if (ParseCullMode(stateElement->Attribute("cull"), cullMode))
                {
                    definition._renderState._overrideCull = true;
                    definition._renderState._cull = cullMode;
                }
            }

            _passes.push_back(std::move(definition));
        }

        if (_passes.empty())
        {
            GHOST_LOG_FORMAT_ERROR("ShaderAsset::Load: no passes in '%s'.",
                dataStream.GetName().c_str());
            return false;
        }

        return true;
    }

    bool ShaderAsset::Save(DataStream& dataStream)
    {
        if (!dataStream.IsWriteable())
            return false;

        tinyxml2::XMLDocument document;
        document.InsertFirstChild(document.NewDeclaration());

        tinyxml2::XMLElement* root = document.NewElement("ShaderAsset");
        root->SetAttribute("version", 1);
        document.InsertEndChild(root);

        for (const ShaderPassDefinition& definition : _passes)
        {
            if (definition._sourcePath.empty() || definition._entries.empty())
                return false;

            tinyxml2::XMLElement* passElement = document.NewElement("Pass");
            passElement->SetAttribute("name", GetPassName(definition._passType));

            tinyxml2::XMLElement* sourceElement = document.NewElement("Source");
            sourceElement->SetAttribute("path", definition._sourcePath.c_str());
            passElement->InsertEndChild(sourceElement);

            for (const ShaderEntryDefinition& entry : definition._entries)
            {
                if (entry.type >= SHADER_TYPE_NUM || entry.entry.empty())
                    return false;

                tinyxml2::XMLElement* entryElement = document.NewElement(ShaderTypeName[entry.type]);
                entryElement->SetAttribute("entry", entry.entry.c_str());
                passElement->InsertEndChild(entryElement);
            }

            if (!definition._defines.empty())
            {
                tinyxml2::XMLElement* definesElement = document.NewElement("Defines");
                for (const auto& define : definition._defines)
                {
                    tinyxml2::XMLElement* defineElement = document.NewElement(define.first.c_str());
                    defineElement->SetAttribute("value", define.second.c_str());
                    definesElement->InsertEndChild(defineElement);
                }
                passElement->InsertEndChild(definesElement);
            }

            const PassRenderState& state = definition._renderState;
            if (state._overrideDepthTest || state._overrideDepthWrite ||
                state._overrideDepthFunc || state._overrideCull)
            {
                tinyxml2::XMLElement* stateElement = document.NewElement("RenderState");

                if (state._overrideDepthTest)
                    stateElement->SetAttribute("depthTest", state._depthTest ? "true" : "false");
                if (state._overrideDepthWrite)
                    stateElement->SetAttribute("depthWrite", state._depthWrite ? "true" : "false");
                if (state._overrideDepthFunc)
                    stateElement->SetAttribute("depthFunc", GetCompareFunctionName(state._depthFunc));
                if (state._overrideCull)
                    stateElement->SetAttribute("cull", GetCullModeName(state._cull));

                passElement->InsertEndChild(stateElement);
            }

            root->InsertEndChild(passElement);
        }

        tinyxml2::XMLPrinter printer;
        document.Print(&printer);

        const std::size_t length = std::strlen(printer.CStr());
        return dataStream.Write(printer.CStr(), length) == length;
    }
}

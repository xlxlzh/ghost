#include "Scene.h"

#include <algorithm>
#include <cstring>

#include "AssetIOUtils.h"
#include "BinaryArchive.h"
#include "tinyxml2.h"

#include "LogManager.h"

namespace ghost
{
    namespace
    {
        const char* SceneDataTypeToString(SceneNodeDataType type)
        {
            switch (type)
            {
            case SceneNodeDataType::Group:  return "Group";
            case SceneNodeDataType::Camera: return "Camera";
            case SceneNodeDataType::Light:  return "Light";
            case SceneNodeDataType::Model:  return "Model";
            }

            return "Group";
        }

        bool SceneDataTypeFromString(const char* text, SceneNodeDataType& type)
        {
            if (!text)
                return false;

            if (strcmp(text, "Group") == 0)
                type = SceneNodeDataType::Group;
            else if (strcmp(text, "Camera") == 0)
                type = SceneNodeDataType::Camera;
            else if (strcmp(text, "Light") == 0)
                type = SceneNodeDataType::Light;
            else if (strcmp(text, "Model") == 0)
                type = SceneNodeDataType::Model;
            else
                return false;

            return true;
        }

        const char* DefaultNodeName(SceneNodeDataType type)
        {
            switch (type)
            {
            case SceneNodeDataType::Group:  return "Group";
            case SceneNodeDataType::Camera: return "Camera";
            case SceneNodeDataType::Light:  return "Light";
            case SceneNodeDataType::Model:  return "Model";
            }

            return "Node";
        }

        SceneNodeData* FindDataNodeRecursive(SceneNodeData* node, const std::string& name)
        {
            if (!node)
                return nullptr;

            if (node->name == name)
                return node;

            for (auto& child : node->children)
            {
                if (SceneNodeData* found = FindDataNodeRecursive(child.get(), name))
                    return found;
            }

            return nullptr;
        }

        std::unique_ptr<SceneNodeData> ParseNode(tinyxml2::XMLElement* element, SceneNodeData* parent)
        {
            if (!element)
                return nullptr;

            const char* typeText = element->Attribute("type");
            SceneNodeDataType type = SceneNodeDataType::Group;
            if (!SceneDataTypeFromString(typeText, type))
            {
                GHOST_LOG_FORMAT_WARNNING("Scene: skipping node with unknown type '%s'.",
                    typeText ? typeText : "");
                return nullptr;
            }

            auto data = std::make_unique<SceneNodeData>();
            data->type = type;
            data->parent = parent;

            const char* name = element->Attribute("name");
            data->name = (name && name[0]) ? name : DefaultNodeName(type);

            data->position = AssetIO::ParseVector3OrDefault(element->Attribute("position"), Vector3f(0.0f, 0.0f, 0.0f));
            data->rotation = AssetIO::ParseVector3OrDefault(element->Attribute("rotation"), Vector3f(0.0f, 0.0f, 0.0f));
            data->scale = AssetIO::ParseVector3OrDefault(element->Attribute("scale"), Vector3f(1.0f, 1.0f, 1.0f));

            switch (type)
            {
            case SceneNodeDataType::Model:
            {
                const char* modelPath = element->Attribute("model");
                if (!modelPath || !modelPath[0])
                {
                    GHOST_LOG_FORMAT_WARNNING("Scene: skipping model node '%s' without a model path.",
                        data->name.c_str());
                    return nullptr;
                }

                data->modelPath = modelPath;

                const char* materialPath = element->Attribute("material");
                if (materialPath && materialPath[0])
                    data->materialPath = materialPath;

                data->visible = element->BoolAttribute("visible", true);
                data->acceptShadow = element->BoolAttribute("acceptShadow", true);
                data->projectShadow = element->BoolAttribute("projectShadow", true);
                break;
            }

            case SceneNodeDataType::Light:
            {
                int lightType = element->IntAttribute("lightType", static_cast<int>(LIGHT_DIRECTIONAL));
                if (lightType < 0 || lightType > LIGHT_SPOT)
                    lightType = LIGHT_DIRECTIONAL;

                data->lightType = static_cast<LightType>(lightType);
                data->color = AssetIO::ParseColorOrDefault(element->Attribute("color"), Color(1.0f, 1.0f, 1.0f, 1.0f));
                data->shininess = element->FloatAttribute("shininess", 100.0f);
                data->radius = element->FloatAttribute("radius", 0.0f);
                data->spotInner = element->FloatAttribute("spotInner", 0.0f);
                data->spotOuter = element->FloatAttribute("spotOuter", 0.0f);
                data->projectShadow = element->BoolAttribute("projectShadow", true);
                break;
            }

            case SceneNodeDataType::Camera:
            {
                data->cameraFov = element->FloatAttribute("fov", 90.0f);
                data->cameraAspect = element->FloatAttribute("aspect", 1.0f);
                data->cameraNear = element->FloatAttribute("near", 1.0f);
                data->cameraFar = element->FloatAttribute("far", 100.0f);
                break;
            }

            case SceneNodeDataType::Group:
                break;
            }

            for (tinyxml2::XMLElement* childElement = element->FirstChildElement("Node");
                childElement != nullptr;
                childElement = childElement->NextSiblingElement("Node"))
            {
                std::unique_ptr<SceneNodeData> child = ParseNode(childElement, data.get());
                if (child)
                    data->children.push_back(std::move(child));
            }

            return data;
        }

        void SaveNode(tinyxml2::XMLDocument& document, tinyxml2::XMLElement* parentElement,
            const SceneNodeData& data)
        {
            tinyxml2::XMLElement* element = document.NewElement("Node");

            element->SetAttribute("type", SceneDataTypeToString(data.type));
            element->SetAttribute("name", data.name.c_str());
            element->SetAttribute("position", AssetIO::FormatVector3(data.position).c_str());
            element->SetAttribute("rotation", AssetIO::FormatVector3(data.rotation).c_str());
            element->SetAttribute("scale", AssetIO::FormatVector3(data.scale).c_str());

            switch (data.type)
            {
            case SceneNodeDataType::Model:
                if (!data.modelPath.empty())
                    element->SetAttribute("model", data.modelPath.c_str());
                if (!data.materialPath.empty())
                    element->SetAttribute("material", data.materialPath.c_str());
                element->SetAttribute("visible", data.visible);
                element->SetAttribute("acceptShadow", data.acceptShadow);
                element->SetAttribute("projectShadow", data.projectShadow);
                break;

            case SceneNodeDataType::Light:
                element->SetAttribute("lightType", static_cast<int>(data.lightType));
                element->SetAttribute("color", AssetIO::FormatColor(data.color).c_str());
                element->SetAttribute("shininess", data.shininess);
                element->SetAttribute("radius", data.radius);
                element->SetAttribute("spotInner", data.spotInner);
                element->SetAttribute("spotOuter", data.spotOuter);
                element->SetAttribute("projectShadow", data.projectShadow);
                break;

            case SceneNodeDataType::Camera:
                element->SetAttribute("fov", data.cameraFov);
                element->SetAttribute("aspect", data.cameraAspect);
                element->SetAttribute("near", data.cameraNear);
                element->SetAttribute("far", data.cameraFar);
                break;

            case SceneNodeDataType::Group:
                break;
            }

            parentElement->InsertEndChild(element);

            for (const auto& child : data.children)
                SaveNode(document, element, *child);
        }

        constexpr uint32_t kMaxSceneStringLength = 64 * 1024;
        constexpr uint32_t kMaxSceneNodeCount = 1024 * 1024;
        constexpr unsigned kMaxSceneDepth = 128;

        void WriteNodeBinary(BinaryWriter& writer, const SceneNodeData& data)
        {
            writer.WriteU8(static_cast<uint8_t>(data.type));
            writer.WriteString(data.name);

            AssetIO::WriteVector3(writer, data.position);
            AssetIO::WriteVector3(writer, data.rotation);
            AssetIO::WriteVector3(writer, data.scale);

            switch (data.type)
            {
            case SceneNodeDataType::Model:
                writer.WriteString(data.modelPath);
                writer.WriteString(data.materialPath);
                writer.WriteBool(data.visible);
                writer.WriteBool(data.acceptShadow);
                writer.WriteBool(data.projectShadow);
                break;

            case SceneNodeDataType::Light:
                writer.WriteU8(static_cast<uint8_t>(data.lightType));
                AssetIO::WriteColor(writer, data.color);
                writer.WriteF32(data.shininess);
                writer.WriteF32(data.radius);
                writer.WriteF32(data.spotInner);
                writer.WriteF32(data.spotOuter);
                writer.WriteBool(data.projectShadow);
                break;

            case SceneNodeDataType::Camera:
                writer.WriteF32(data.cameraFov);
                writer.WriteF32(data.cameraAspect);
                writer.WriteF32(data.cameraNear);
                writer.WriteF32(data.cameraFar);
                break;

            case SceneNodeDataType::Group:
                break;
            }

            writer.WriteU32(static_cast<uint32_t>(data.children.size()));
            for (const auto& child : data.children)
                WriteNodeBinary(writer, *child);
        }

        bool ReadNodeBinary(BinaryReader& reader, SceneNodeData* parent, unsigned depth,
            uint32_t& remainingNodes, std::unique_ptr<SceneNodeData>& result)
        {
            if (depth > kMaxSceneDepth)
            {
                GHOST_LOG_FORMAT_ERROR("Scene::Load: node nesting exceeds %u levels.", kMaxSceneDepth);
                return false;
            }

            if (remainingNodes == 0)
            {
                GHOST_LOG_FORMAT_ERROR("Scene::Load: node count exceeds the allowed limit.");
                return false;
            }

            --remainingNodes;

            uint8_t typeValue = 0;
            if (!reader.ReadU8(typeValue))
                return false;

            if (typeValue > static_cast<uint8_t>(SceneNodeDataType::Model))
            {
                GHOST_LOG_FORMAT_ERROR("Scene::Load: invalid node type %u.", static_cast<unsigned>(typeValue));
                return false;
            }

            auto data = std::make_unique<SceneNodeData>();
            data->type = static_cast<SceneNodeDataType>(typeValue);
            data->parent = parent;

            if (!reader.ReadString(data->name, kMaxSceneStringLength))
                return false;

            if (data->name.empty())
                data->name = DefaultNodeName(data->type);

            if (!AssetIO::ReadVector3(reader, data->position) ||
                !AssetIO::ReadVector3(reader, data->rotation) ||
                !AssetIO::ReadVector3(reader, data->scale))
            {
                return false;
            }

            switch (data->type)
            {
            case SceneNodeDataType::Model:
                if (!reader.ReadString(data->modelPath, kMaxSceneStringLength) ||
                    !reader.ReadString(data->materialPath, kMaxSceneStringLength))
                {
                    return false;
                }

                if (data->modelPath.empty())
                {
                    GHOST_LOG_FORMAT_ERROR("Scene::Load: model node '%s' has no model path.", data->name.c_str());
                    return false;
                }

                if (!reader.ReadBool(data->visible) ||
                    !reader.ReadBool(data->acceptShadow) ||
                    !reader.ReadBool(data->projectShadow))
                {
                    return false;
                }
                break;

            case SceneNodeDataType::Light:
            {
                uint8_t lightTypeValue = 0;
                if (!reader.ReadU8(lightTypeValue))
                    return false;

                if (lightTypeValue > static_cast<uint8_t>(LIGHT_SPOT))
                {
                    GHOST_LOG_FORMAT_ERROR("Scene::Load: invalid light type %u.", static_cast<unsigned>(lightTypeValue));
                    return false;
                }

                data->lightType = static_cast<LightType>(lightTypeValue);

                if (!AssetIO::ReadColor(reader, data->color) ||
                    !reader.ReadF32(data->shininess) ||
                    !reader.ReadF32(data->radius) ||
                    !reader.ReadF32(data->spotInner) ||
                    !reader.ReadF32(data->spotOuter) ||
                    !reader.ReadBool(data->projectShadow))
                {
                    return false;
                }
                break;
            }

            case SceneNodeDataType::Camera:
                if (!reader.ReadF32(data->cameraFov) ||
                    !reader.ReadF32(data->cameraAspect) ||
                    !reader.ReadF32(data->cameraNear) ||
                    !reader.ReadF32(data->cameraFar))
                {
                    return false;
                }
                break;

            case SceneNodeDataType::Group:
                break;
            }

            uint32_t childCount = 0;
            if (!reader.ReadU32(childCount))
                return false;

            if (childCount > remainingNodes)
            {
                GHOST_LOG_FORMAT_ERROR("Scene::Load: child node count exceeds the allowed limit.");
                return false;
            }

            if (childCount > 0)
            {
                data->children.reserve(childCount);
                for (uint32_t index = 0; index < childCount; ++index)
                {
                    std::unique_ptr<SceneNodeData> child;
                    if (!ReadNodeBinary(reader, data.get(), depth + 1, remainingNodes, child))
                        return false;

                    data->children.push_back(std::move(child));
                }
            }

            result = std::move(data);
            return true;
        }
    }

    Scene::Scene() : Resource(RESOURCE_SCENE, "", 0)
    {
        _root = std::make_unique<SceneNodeData>();
        _root->type = SceneNodeDataType::Group;
        _root->name = "Scene";
    }

    Scene::~Scene() = default;

    bool Scene::Load(DataStream& dataStream)
    {
        _loaded = false;

        switch (DetectAssetEncoding(dataStream))
        {
        case AssetEncoding::Gmap:
            return LoadBinary(dataStream);

        case AssetEncoding::Xml:
            return LoadXml(dataStream);

        case AssetEncoding::Gres:
            GHOST_LOG_FORMAT_ERROR("Scene::Load: '%s' is a GRES asset, not a GMAP scene.",
                dataStream.GetName().c_str());
            return false;

        default:
            GHOST_LOG_FORMAT_ERROR("Scene::Load: unknown asset format in '%s'.",
                dataStream.GetName().c_str());
            return false;
        }
    }

    bool Scene::LoadBinary(DataStream& dataStream)
    {
        BinaryReader reader(dataStream);
        if (!ReadAssetHeader(reader, AssetFormat::GMAP_MAGIC,
            static_cast<uint16_t>(RESOURCE_SCENE), AssetFormat::SCENE_VERSION))
        {
            GHOST_LOG_FORMAT_ERROR("Scene::LoadBinary: invalid header in '%s'.",
                dataStream.GetName().c_str());
            return false;
        }

        uint32_t childCount = 0;
        if (!reader.ReadU32(childCount))
        {
            GHOST_LOG_FORMAT_ERROR("Scene::LoadBinary: failed to read root child count in '%s'.",
                dataStream.GetName().c_str());
            return false;
        }

        if (childCount > kMaxSceneNodeCount)
        {
            GHOST_LOG_FORMAT_ERROR("Scene::LoadBinary: root child count exceeds the allowed limit.");
            return false;
        }

        auto newRoot = std::make_unique<SceneNodeData>();
        newRoot->type = SceneNodeDataType::Group;
        newRoot->name = "Scene";

        uint32_t remainingNodes = kMaxSceneNodeCount;
        for (uint32_t index = 0; index < childCount; ++index)
        {
            std::unique_ptr<SceneNodeData> child;
            if (!ReadNodeBinary(reader, newRoot.get(), 1, remainingNodes, child))
            {
                GHOST_LOG_FORMAT_ERROR("Scene::LoadBinary: failed to read node %u in '%s'.",
                    static_cast<unsigned>(index), dataStream.GetName().c_str());
                return false;
            }

            newRoot->children.push_back(std::move(child));
        }

        if (!reader.IsOk())
        {
            GHOST_LOG_FORMAT_ERROR("Scene::LoadBinary: %s", reader.GetError().c_str());
            return false;
        }

        _root = std::move(newRoot);
        _loaded = true;
        return true;
    }

    bool Scene::LoadXml(DataStream& dataStream)
    {
        _loaded = false;

        std::string text;
        if (!ReadStreamAsText(dataStream, text))
        {
            GHOST_LOG_FORMAT_ERROR("Scene::Load: empty or unreadable stream '%s'.",
                dataStream.GetName().c_str());
            return false;
        }

        tinyxml2::XMLDocument document;
        if (document.Parse(text.c_str(), text.size()) != tinyxml2::XML_SUCCESS)
        {
            GHOST_LOG_FORMAT_ERROR("Scene::Load: XML parse error in '%s'.",
                dataStream.GetName().c_str());
            return false;
        }

        tinyxml2::XMLElement* sceneElement = document.FirstChildElement("Scene");
        if (!sceneElement)
        {
            GHOST_LOG_FORMAT_ERROR("Scene::Load: missing <Scene> root in '%s'.",
                dataStream.GetName().c_str());
            return false;
        }

        const int version = sceneElement->IntAttribute("version", 1);
        if (version > 1)
        {
            GHOST_LOG_FORMAT_WARNNING("Scene::Load: unsupported scene version %d in '%s'.",
                version, dataStream.GetName().c_str());
        }

        auto newRoot = std::make_unique<SceneNodeData>();
        newRoot->type = SceneNodeDataType::Group;
        newRoot->name = "Scene";

        for (tinyxml2::XMLElement* element = sceneElement->FirstChildElement("Node");
            element != nullptr;
            element = element->NextSiblingElement("Node"))
        {
            std::unique_ptr<SceneNodeData> child = ParseNode(element, newRoot.get());
            if (child)
                newRoot->children.push_back(std::move(child));
        }

        _root = std::move(newRoot);
        _loaded = true;
        return true;
    }

    bool Scene::Save(DataStream& dataStream)
    {
        if (!_root)
        {
            GHOST_LOG_FORMAT_ERROR("Scene::Save: scene has no root data.");
            return false;
        }

        if (!dataStream.IsWriteable())
        {
            GHOST_LOG_FORMAT_ERROR("Scene::Save: stream '%s' is not writable.",
                dataStream.GetName().c_str());
            return false;
        }

        BinaryWriter writer(dataStream);
        if (!WriteAssetHeader(writer, AssetFormat::GMAP_MAGIC,
            static_cast<uint16_t>(RESOURCE_SCENE), AssetFormat::SCENE_VERSION))
        {
            GHOST_LOG_FORMAT_ERROR("Scene::Save: failed to write header to '%s'.",
                dataStream.GetName().c_str());
            return false;
        }

        writer.WriteU32(static_cast<uint32_t>(_root->children.size()));
        for (const auto& child : _root->children)
            WriteNodeBinary(writer, *child);

        if (!writer.IsOk())
        {
            GHOST_LOG_FORMAT_ERROR("Scene::Save: %s", writer.GetError().c_str());
            return false;
        }

        return true;
    }

    bool Scene::SaveXml(DataStream& dataStream)
    {
        if (!_root)
        {
            GHOST_LOG_FORMAT_ERROR("Scene::SaveXml: scene has no root data.");
            return false;
        }

        if (!dataStream.IsWriteable())
        {
            GHOST_LOG_FORMAT_ERROR("Scene::SaveXml: stream '%s' is not writable.",
                dataStream.GetName().c_str());
            return false;
        }

        tinyxml2::XMLDocument document;
        document.InsertFirstChild(document.NewDeclaration());

        tinyxml2::XMLElement* sceneElement = document.NewElement("Scene");
        sceneElement->SetAttribute("version", 1);
        document.InsertEndChild(sceneElement);

        for (const auto& child : _root->children)
            SaveNode(document, sceneElement, *child);

        tinyxml2::XMLPrinter printer;
        document.Print(&printer);

        const std::size_t length = std::strlen(printer.CStr());
        const std::size_t written = dataStream.Write(printer.CStr(), length);
        if (written != length)
        {
            GHOST_LOG_FORMAT_ERROR("Scene::Save: wrote %u of %u bytes to '%s'.",
                static_cast<unsigned>(written), static_cast<unsigned>(length),
                dataStream.GetName().c_str());
            return false;
        }

        return true;
    }

    SceneNodeData* Scene::AddNode(SceneNodeDataType type, const std::string& name,
        SceneNodeData* parent)
    {
        if (!_root)
            return nullptr;

        if (!parent)
            parent = _root.get();

        auto data = std::make_unique<SceneNodeData>();
        data->type = type;
        data->name = name.empty() ? DefaultNodeName(type) : name;
        data->parent = parent;

        SceneNodeData* result = data.get();
        parent->children.push_back(std::move(data));
        return result;
    }

    bool Scene::RemoveNode(SceneNodeData* node)
    {
        if (!node || !node->parent || node == _root.get())
            return false;

        std::vector<std::unique_ptr<SceneNodeData>>& siblings = node->parent->children;
        auto it = std::find_if(siblings.begin(), siblings.end(),
            [node](const std::unique_ptr<SceneNodeData>& entry)
            {
                return entry.get() == node;
            });

        if (it == siblings.end())
            return false;

        siblings.erase(it);
        return true;
    }

    void Scene::ClearNodes()
    {
        if (_root)
            _root->children.clear();
    }

    SceneNodeData* Scene::FindNode(const std::string& name) const
    {
        return FindDataNodeRecursive(_root.get(), name);
    }
}

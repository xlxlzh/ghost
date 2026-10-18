#include "Scene.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <sstream>

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

        std::string FormatVector(const Vector3f& value)
        {
            char buffer[128] = {};
            std::snprintf(buffer, sizeof(buffer), "%.9g %.9g %.9g",
                value._x, value._y, value._z);
            return buffer;
        }

        std::string FormatColor(const Color& color)
        {
            char buffer[160] = {};
            std::snprintf(buffer, sizeof(buffer), "%.9g %.9g %.9g %.9g",
                color._r, color._g, color._b, color._a);
            return buffer;
        }

        Vector3f ParseVector(const char* text, const Vector3f& fallback)
        {
            if (!text)
                return fallback;

            std::istringstream stream(text);
            Vector3f value = fallback;
            if (!(stream >> value._x >> value._y >> value._z))
                return fallback;

            return value;
        }

        Color ParseColor(const char* text, const Color& fallback)
        {
            if (!text)
                return fallback;

            std::istringstream stream(text);
            Color value = fallback;
            if (!(stream >> value._r >> value._g >> value._b >> value._a))
                return fallback;

            return value;
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

            data->position = ParseVector(element->Attribute("position"), Vector3f(0.0f, 0.0f, 0.0f));
            data->rotation = ParseVector(element->Attribute("rotation"), Vector3f(0.0f, 0.0f, 0.0f));
            data->scale = ParseVector(element->Attribute("scale"), Vector3f(1.0f, 1.0f, 1.0f));

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
                data->color = ParseColor(element->Attribute("color"), Color(1.0f, 1.0f, 1.0f, 1.0f));
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
            element->SetAttribute("position", FormatVector(data.position).c_str());
            element->SetAttribute("rotation", FormatVector(data.rotation).c_str());
            element->SetAttribute("scale", FormatVector(data.scale).c_str());

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
                element->SetAttribute("color", FormatColor(data.color).c_str());
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

        const unsigned size = static_cast<unsigned>(dataStream.GetSize());
        if (size == 0)
        {
            GHOST_LOG_FORMAT_ERROR("Scene::Load: empty stream '%s'.", dataStream.GetName().c_str());
            return false;
        }

        std::vector<char> buffer(size + 1, 0);
        const std::size_t readCount = dataStream.Read(buffer.data(), size);
        if (readCount != size)
        {
            GHOST_LOG_FORMAT_ERROR("Scene::Load: read %u of %u bytes from '%s'.",
                static_cast<unsigned>(readCount), size, dataStream.GetName().c_str());
            return false;
        }

        tinyxml2::XMLDocument document;
        if (document.Parse(buffer.data(), size) != tinyxml2::XML_SUCCESS)
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

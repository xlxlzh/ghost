#ifndef _SCENE_H_
#define _SCENE_H_

#include <memory>
#include <string>
#include <vector>

#include "Ghost.h"
#include "Resource.h"
#include "Vector3.h"
#include "Color.h"
#include "Light.h"

namespace ghost
{
    enum class SceneNodeDataType
    {
        Group,
        Camera,
        Light,
        Model
    };

    struct SceneNodeData
    {
        SceneNodeDataType type = SceneNodeDataType::Group;
        std::string name;

        Vector3f position = Vector3f(0.0f, 0.0f, 0.0f);
        Vector3f rotation = Vector3f(0.0f, 0.0f, 0.0f);
        Vector3f scale = Vector3f(1.0f, 1.0f, 1.0f);

        std::string modelPath;
        std::string materialPath = "Materials/DefaultMaterial.xml";

        bool visible = true;
        bool acceptShadow = true;
        bool projectShadow = true;

        LightType lightType = LIGHT_DIRECTIONAL;
        Color color = Color(1.0f, 1.0f, 1.0f, 1.0f);
        float shininess = 100.0f;
        float radius = 0.0f;
        float spotInner = 0.0f;
        float spotOuter = 0.0f;

        float cameraFov = 90.0f;
        float cameraAspect = 1.0f;
        float cameraNear = 1.0f;
        float cameraFar = 100.0f;

        SceneNodeData* parent = nullptr;
        std::vector<std::unique_ptr<SceneNodeData>> children;

        SceneNodeData() = default;
        ~SceneNodeData() = default;
        SceneNodeData(const SceneNodeData&) = delete;
        SceneNodeData& operator=(const SceneNodeData&) = delete;
    };

    class GHOST_API Scene final : public Resource
    {
    public:
        Scene();
        ~Scene() override;

        bool Load(DataStream& dataStream) override;
        bool Save(DataStream& dataStream) override;

        SceneNodeData* GetRoot() const { return _root.get(); }

        SceneNodeData* AddNode(SceneNodeDataType type, const std::string& name,
            SceneNodeData* parent = nullptr);

        bool RemoveNode(SceneNodeData* node);

        void ClearNodes();

        SceneNodeData* FindNode(const std::string& name) const;

        DECLAR_RESOURCE_TYPE(SCENE);

    private:
        std::unique_ptr<SceneNodeData> _root;
    };

    DECLAR_SMART_POINTER(Scene)
}

#endif

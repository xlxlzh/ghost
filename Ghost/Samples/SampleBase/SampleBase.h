#include "Application.h"

#include "SceneManager.h"
#include "Camera.h"

using namespace ghost;

class SampleBase : public Application
{
public:
    virtual void onInit() override;
    virtual void onExit() override;
    virtual void onUpdate() override;
    virtual void onTick(float deltaTime) override;

    virtual void onKeydown(GhostKey key) override;

private:
    void _generateResourcesPath(const std::string& workdir);

private:
    std::string _resourcesPath{};

    SceneManager* _scene;
    Camera* _mainCamera;

    Light* _mainLight;
};
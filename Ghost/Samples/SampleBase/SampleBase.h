#include "Application.h"

#include "SceneManager.h"
#include "Camera.h"

using namespace ghost;

class SampleBase : public Application
{
public:
    virtual void onInit() override;
    virtual void onExit() override;
    virtual void onTick(float deltaTime) override;

    virtual void onKeydown(GhostKey key) override;

    virtual void initSample(){ }

protected:
    void _generateResourcesPath(const std::string& workdir);

protected:
    std::string _resourcesPath{};

    SceneManager* _scene;
    Camera* _mainCamera;
};
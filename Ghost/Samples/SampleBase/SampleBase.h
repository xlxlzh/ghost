#include "Application.h"

#include "SceneManager.h"
#include "Camera.h"

using namespace ghost;

class SampleBase : public Application
{
public:
    virtual void OnInit() override;
    virtual void OnExit() override;
    virtual void OnTick(float deltaTime) override;

    virtual void InitSample(){ }

private:
    void GenerateResourcesPath(const std::string& workdir);

private:
    std::string _resourcesPath{};
};
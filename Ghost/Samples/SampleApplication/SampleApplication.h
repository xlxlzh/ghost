#include "Application.h"

#include "SceneManager.h"
#include "Camera.h"

#include "SettingPage.h"
#include "SampleBase.h"

using namespace ghost;

class SampleApplication : public SampleBase
{
public:
    virtual void InitSample() override;
    virtual void OnUpdate() override;

    virtual void OnKeydown(GhostKey key) override;

private:
    SceneManager* _scene;
    Camera* _mainCamera;

    SettingPagePtr _settingPage;

    Light* _mainLight;
};
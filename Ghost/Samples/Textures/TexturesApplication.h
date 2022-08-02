#include "Application.h"

#include "SceneManager.h"
#include "Camera.h"

#include "SettingPage.h"
#include "SampleBase.h"

using namespace ghost;

class TexturesApplication : public SampleBase
{
public:
    virtual void initSample() override;
    virtual void onUpdate() override;

    virtual void onKeydown(GhostKey key) override;

private:
    SceneManager* _scene;
    Camera* _mainCamera;

    SettingPagePtr _settingPage;

    Light* _mainLight;
};
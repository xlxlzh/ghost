#include "GUIManager.h"
#include "Color.h"
#include "Vector3.h"

class SettingPage : public ghost::GuiPage
{
public:
    SettingPage();

    virtual void show();

    const ghost::Color& getLightColor() const { return _lightColor; }
    const ghost::Vector3f& getLightDirection() const { return _lightDirection; }

private:
    float _lightDirection[3];

    ghost::Color _lightColor;
};

DECLAR_SMART_POINTER(SettingPage);
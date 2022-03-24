#include "SettingPage.h"
#include "imgui.h"

SettingPage::SettingPage() 
    : _lightColor(1.0f, 1.0f, 1.0f)
{
    _lightDirection[0] = 0.0f;
    _lightDirection[1] = 0.0f;
    _lightDirection[2] = 1.0f;
}

void SettingPage::show()
{
    ImGui::Begin("Sample Settings");

    ImGui::ColorEdit4("Light Color", _lightColor.getColorPtr());

    ImGui::InputFloat3("Light Direction", _lightDirection);
    ImGui::End();
}
#include "ContentBrowser.h"
#include "imgui.h"

ContentBrowserPage::ContentBrowserPage()
{
}

void ContentBrowserPage::Show()
{
    ImGui::Begin("ContentBrowser.Pages");
    
    ImGui::Button("Content Browser");

    ImGui::End();
}
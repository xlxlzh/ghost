#include "GUIManager.h"
#include "imgui.h"
#include <algorithm>

namespace ghost
{
    GuiManager::GuiManager()
    {

    }

    GuiManager::~GuiManager()
    {

    }

    GuiHandle GuiManager::RegisterGuiPage(GuiPagePtr page, bool enable /* = true */)
    {
        if (page == nullptr)
            return GUI_NO_HANDLE;

        for (std::size_t i = 0; i < _pages.size(); ++i)
        {
            auto& pageInfo = _pages[i];
            if (pageInfo.pagePtr == page)
            {
                pageInfo.isEnable = enable;
                return i;
            }
                
        }

        for (std::size_t i = 0; i < _pages.size(); ++i)
        {
            auto& pageInfo = _pages[i];
            if (pageInfo.handle == GUI_NO_HANDLE)
            {
                pageInfo.handle = i;
                pageInfo.pagePtr = page;
                pageInfo.isEnable = enable;
                return i;
            }
        }

        GuiPageInfo newPage;
        newPage.pagePtr = page;
        newPage.handle = _pages.size();
        newPage.isEnable = enable;
        _pages.push_back(newPage);
            
        return newPage.handle;
    }

    void GuiManager::UnregisterGuiPage(GuiHandle pageHandle)
    {
        if (pageHandle >= _pages.size())
            return;

        auto& pageInfo = _pages[pageHandle];
        pageInfo.handle = GUI_NO_HANDLE;
        pageInfo.pagePtr = nullptr;
        pageInfo.isEnable = false;
    }

    void GuiManager::UnregisterGuiPage(const GuiPagePtr& page)
    {
        if (page == nullptr)
            return;

        auto it = std::find_if(_pages.begin(), _pages.end(),
            [&page](const GuiPageInfo& info)->bool
            {
                return page == info.pagePtr;
            }
        );

        if (it != _pages.end())
        {
            it->handle = GUI_NO_HANDLE;
            it->pagePtr = nullptr;
            it->isEnable = false;
        }
    }

    void GuiManager::EnableGuiPage(GuiHandle pageHandle)
    {
        if (pageHandle >= _pages.size())
            return;

        auto& pageInfo = _pages[pageHandle];
        if (pageInfo.pagePtr)
            pageInfo.isEnable = true;
    }

    void GuiManager::DisableGuiPage(GuiHandle pageHandle)
    {
        if (pageHandle >= _pages.size())
            return;

        _pages[pageHandle].isEnable = false;
    }

    void GuiManager::RenderAllPages()
    {
        if (ImGui::GetCurrentContext())
        {
            ImGui::NewFrame();

            for (const auto& page : _pages)
            {
                if (page.isEnable && page.pagePtr)
                    page.pagePtr->Show();
            }

            ImGui::Render();
        }
    }
}
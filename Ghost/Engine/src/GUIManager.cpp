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

    GuiHandle GuiManager::registerGuiPage(GuiPagePtr page, bool enable /* = true */)
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

    void GuiManager::unregisterGuiPage(GuiHandle pageHandle)
    {
        if (pageHandle >= _pages.size())
            return;

        auto& pageInfo = _pages[pageHandle];
        pageInfo.handle = GUI_NO_HANDLE;
        pageInfo.pagePtr = nullptr;
        pageInfo.isEnable = false;
    }

    void GuiManager::unregisterGuiPage(const GuiPagePtr& page)
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

    void GuiManager::enableGuiPage(GuiHandle pageHandle)
    {
        if (pageHandle >= _pages.size())
            return;

        auto& pageInfo = _pages[pageHandle];
        if (pageInfo.pagePtr)
            pageInfo.isEnable = true;
    }

    void GuiManager::disableGuiPage(GuiHandle pageHandle)
    {
        if (pageHandle >= _pages.size())
            return;

        _pages[pageHandle].isEnable = false;
    }

    void GuiManager::renderAllPages()
    {
        if (ImGui::GetCurrentContext())
        {
            ImGui::NewFrame();

            for (const auto& page : _pages)
            {
                if (page.isEnable && page.pagePtr)
                    page.pagePtr->show();
            }

            ImGui::Render();
        }
    }
}
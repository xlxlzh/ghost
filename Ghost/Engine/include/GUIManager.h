#ifndef _GUI_MANAGER_H_
#define _GUI_MANAGER_H_

#include <vector>

#include "Ghost.h"
#include "SingleTon.h"

namespace ghost
{
    class GHOST_API GuiPage
    {
    public:
        GuiPage() { }
        virtual ~GuiPage() { }

        virtual void Show() = 0;
    };

    DECLAR_SMART_POINTER(GuiPage);

    using GuiHandle = std::size_t;
    static const int GUI_NO_HANDLE = -1;

    class GHOST_API GuiManager : public SingleTon<GuiManager>
    {
    public:
        GuiManager();
        ~GuiManager();

        GuiHandle RegisterGuiPage(GuiPagePtr page, bool enable = true);
        void UnregisterGuiPage(const GuiPagePtr& page);
        void UnregisterGuiPage(GuiHandle pageHandle);

        void EnableGuiPage(GuiHandle pageHandle);
        void DisableGuiPage(GuiHandle pageHandle);

        void RenderAllPages();

    private:

        struct GuiPageInfo
        {
            GuiPagePtr pagePtr;
            bool       isEnable;
            GuiHandle  handle;

            GuiPageInfo()
                : pagePtr(nullptr)
                , isEnable(true)
                , handle(GUI_NO_HANDLE)
            {

            }
             
        };

        std::vector<GuiPageInfo> _pages;
    };
}

#endif

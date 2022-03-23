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

        virtual void show() = 0;
    };

    DECLAR_SMART_POINTER(GuiPage);

    using GuiHandle = int;
    static const int GUI_NO_HANDLE = -1;

    class GHOST_API GuiManager : public SingleTon<GuiManager>
    {
    public:
        GuiManager();
        ~GuiManager();

        GuiHandle registerGuiPage(GuiPagePtr page, bool enable = true);
        void unregisterGuiPage(const GuiPagePtr& page);
        void unregisterGuiPage(GuiHandle pageHandle);

        void enableGuiPage(GuiHandle pageHandle);
        void disableGuiPage(GuiHandle pageHandle);

        void renderAllPages();

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

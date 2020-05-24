#include "Timer.h"
#include <windows.h>

namespace ghost
{
    class GHOST_API TimerWin32 : public Timer
    {
    public:
        TimerWin32();
        virtual ~TimerWin32();

        virtual void start() override;
        virtual void reset() override;

        virtual double getTime() override;

    private:
        LARGE_INTEGER  _timerFreq;
        DWORD_PTR      _affMask;
    };
}
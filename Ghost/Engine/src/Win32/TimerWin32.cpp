#include "TimerWin32.h"

namespace ghost
{
    TimerWin32::TimerWin32()
    {
        DWORD_PTR procMask, sysMask;
        GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);
        _affMask = 1;
        while ((_affMask & procMask) == 0) _affMask <<= 1;

        DWORD_PTR threadAffMask = SetThreadAffinityMask(GetCurrentThread(), _affMask);
        QueryPerformanceFrequency(&_timerFreq);
        SetThreadAffinityMask(GetCurrentThread(), threadAffMask);
    }

    TimerWin32::~TimerWin32()
    {

    }

    void TimerWin32::reset()
    {
        _elapsedTime = 0.0;
        if (_enable)
        {
            _startTime = getTime();
        }
    }

    void TimerWin32::start()
    {
        _enable = true;
        _startTime = getTime();
        _elapsedTime = 0.0;
    }

    double TimerWin32::getTime()
    {
        DWORD_PTR threadAffMask = SetThreadAffinityMask(GetCurrentThread(), _affMask);

        LARGE_INTEGER curTick;
        QueryPerformanceCounter(&curTick);

        SetThreadAffinityMask(GetCurrentThread(), threadAffMask);

        return (double)curTick.QuadPart / (double)_timerFreq.QuadPart * 1000.0;
    }
}
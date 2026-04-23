#ifndef _TIMER_H_
#define _TIMER_H_

#include "Ghost.h"
#include <memory>

namespace ghost
{
    class GHOST_API Timer
    {
    public:
        Timer() = default;
        virtual ~Timer() { }

        virtual void Start() = 0;
        virtual void Reset() = 0;
        virtual double GetTime() = 0;

        void Pause()
        {
            if (_enable)
            {
                double endTime = GetTime();
                _elapsedTime += endTime - _startTime;
                _enable = false;
            }
        }

        float GetElapsedTimeMS()
        {
            if (_enable)
            {
                double endTime = GetTime();
                _elapsedTime += endTime - _startTime;
                _startTime = endTime;
            }

            return (float)_elapsedTime;
        }

    protected:
        double         _startTime = 0.0;
        double         _elapsedTime = 0.0;
        bool           _enable = false;
    };

    using TimerPtr = std::shared_ptr<Timer>;
}

#endif

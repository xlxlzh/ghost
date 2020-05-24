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

        virtual void start() = 0;
        virtual void reset() = 0;
        virtual double getTime() = 0;

        void pause()
        {
            if (_enable)
            {
                double endTime = getTime();
                _elapsedTime += endTime - _startTime;
                _enable = false;
            }
        }

        float getElapsedTimeMS()
        {
            if (_enable)
            {
                double endTime = getTime();
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

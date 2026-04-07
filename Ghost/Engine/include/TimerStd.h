#include <chrono>
#include "Timer.h"
namespace ghost
{
    class GHOST_API TimerStd : public Timer
    {
    public:
        TimerStd() = default;
        virtual ~TimerStd() = default;
        
        virtual void start() override;
        virtual void reset() override;
        virtual double getTime() override;
        
    private:
        
    };
}
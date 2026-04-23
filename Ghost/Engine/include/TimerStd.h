#include <chrono>
#include "Timer.h"
namespace ghost
{
    class GHOST_API TimerStd : public Timer
    {
    public:
        TimerStd() = default;
        virtual ~TimerStd() = default;
        
        virtual void Start() override;
        virtual void Reset() override;
        virtual double GetTime() override;
        
    private:
        
    };
}
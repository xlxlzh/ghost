#ifndef _GHOST_CONFIG_H_
#define _GHOST_CONFIG_H_

#include <string>
#include "Ghost.h"

namespace ghost
{
    class GHOST_API GhostConfig
    {
    public:
        const std::string& getLogFile() const { return _logFile; }

    private:
        std::string _logFile;
    };
}

#endif // !_GHOST_CONFIG_H_

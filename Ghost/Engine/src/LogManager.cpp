#include "LogManager.h"

namespace ghost
{
    Log::Log(std::string name, LogLevel level /* = LOG_DEBUG */, bool timestamp /* = true */) : _logName(name), _level(level), _enableTimeStamp(timestamp)
    {
        _stream.open(name);
    }

    void Log::outputMessage(std::string message)
    {
        _stream << message << std::endl;
        _stream.flush();
    }
}
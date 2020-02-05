#ifndef _LOGMANAGER_H_
#define _LOGMANAGER_H_

#include <fstream>
#include "SingleTon.h"

namespace ghost
{
    enum LogLevel
    {
        LOG_DEBUG,
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR
    };

    class Log
    {
    public:
        Log(std::string name, LogLevel level = LOG_DEBUG, bool timestamp = true);

        std::string getName() const { return _logName; }
        LogLevel getLogLevel() const { return _level; }

        void outputMessage(std::string message);

    private:
        std::ofstream _stream;
        std::string _logName;
        LogLevel _level;
        bool _enableTimeStamp;
    };

    class LogManager : public SingleTon<LogManager>
    {
    public:
        LogManager() = default;

    public:


    private:

    };
}

#endif

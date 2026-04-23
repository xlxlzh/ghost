#ifndef _LOGMANAGER_H_
#define _LOGMANAGER_H_

#include <fstream>
#include <map>
#include <string>
#include "SingleTon.h"
#include "Ghost.h"

namespace ghost
{
    enum LogMessageLevel
    {
        LOG_DEBUG,
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR
    };

    enum LogLevel
    {
        LOW_LOW,
        LOG_NORMAL,
        LOG_HIGH
    };

    static std::string LogMessageLevelStr[] =
    {
        "[DEBUG]",
        "[INFO]",
        "[WARNING]",
        "[ERROR]"
    };

    class GHOST_API Log
    {
    public:
        Log(std::string name, LogLevel level = LOG_NORMAL, bool timestamp = true);
        ~Log();

        std::string GetName() const { return _logName; }
        LogLevel GetLogLevel() const { return _level; }

        void LogMessage(LogMessageLevel level, const std::string& message);

    private:
        std::ofstream _stream;
        std::string _logName;
        LogLevel _level;
        bool _enableTimeStamp;
    };

    using LogPtr = std::shared_ptr<Log>;

    class GHOST_API LogManager : public SingleTon<LogManager>
    {
    public:
        LogManager() = default;

        LogPtr AddLog(const std::string& name, LogLevel level = LOG_NORMAL, bool enableTimestamp = true, bool defaultLog = true);
        void DestoryLog(const std::string& name);
        LogPtr GetLog(const std::string& name) const;

        void SetDefaultLog(LogPtr log) { _defeaultLog = log; }
        LogPtr GetDefaultLog() const { return _defeaultLog; }

        void LogMessage(LogMessageLevel level, const std::string& message);
        void LogDebug(const std::string& message);
        void LogInfo(const std::string& message);
        void LogError(const std::string& message);
        void LogWarning(const std::string& message);

    private:
        std::map<std::string, LogPtr> _logs;
        LogPtr _defeaultLog;
    };

    GHOST_API std::string FormatStringToString(const char* fmt, ...);

    #define GHOST_LOG_FORMAT(level, fmt, ...) LogManager::GetInstance()->LogMessage(level, FormatStringToString(fmt, ##__VA_ARGS__));
    #define GHOST_LOG_FORMAT_DEBUG(fmt, ...) GHOST_LOG_FORMAT(LOG_DEBUG, fmt, ##__VA_ARGS__)
    #define GHOST_LOG_FORMAT_INFO(fmt, ...) GHOST_LOG_FORMAT(LOG_INFO, fmt, ##__VA_ARGS__)
    #define GHOST_LOG_FORMAT_WARNNING(fmt, ...) GHOST_LOG_FORMAT(LOG_WARNING, fmt, ##__VA_ARGS__)
    #define GHOST_LOG_FORMAT_ERROR(fmt, ...) GHOST_LOG_FORMAT(LOG_ERROR, fmt, ##__VA_ARGS__)
    #define GHOST_LOG_FORMAT_ERROR_THROW(fmt, ...) GHOST_LOG_FORMAT(LOG_ERROR, fmt, ##__VA_ARGS__);throw;

}

#endif

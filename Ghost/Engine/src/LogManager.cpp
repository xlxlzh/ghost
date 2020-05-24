#include "LogManager.h"
#include <ctime>
#include <iomanip>

namespace ghost
{
    Log::Log(std::string name, LogLevel level /* = LOG_DEBUG */, bool timestamp /* = true */) : _logName(name), _level(level), _enableTimeStamp(timestamp)
    {
        _stream.open(name);
    }

    Log::~Log()
    {
        if (_stream.is_open())
        {
            _stream.close();
        }
    }

    void Log::logMessage(LogMessageLevel level, const std::string& message)
    {
        if (_enableTimeStamp)
        {
            tm* pTime = nullptr;
            time_t ctTime;
            time(&ctTime);
            pTime = localtime(&ctTime);

            _stream << std::setw(2) << std::setfill('0') << pTime->tm_hour
                << ":" << std::setw(2) << std::setfill('0') << pTime->tm_min
                << ":" << std::setw(2) << std::setfill('0') << pTime->tm_sec
                << ": ";
        }

        _stream << LogMessageLevelStr[level] << ' ' << message << std::endl;
        _stream.flush();
    }


    LogPtr LogManager::addLog(const std::string& name, LogLevel level /* = LOG_NORMAL */, bool enableTimestamp /* = true */, bool defaultLog /* = false */)
    {
        LogPtr newLog = std::make_shared<Log>(name, level, enableTimestamp);
        _logs[name] = newLog;

        if (defaultLog)
        {
            _defeaultLog = newLog;
        }

        return newLog;
    }

    void LogManager::logMessage(LogMessageLevel level, const std::string& message)
    {
        if (_defeaultLog)
        {
            _defeaultLog->logMessage(level, message);
        }
    }

    void LogManager::logDebug(const std::string& message)
    {
#ifdef _DEBUG
        logMessage(LOG_DEBUG, message);
#endif
    }

    void LogManager::logInfo(const std::string& message)
    {
        logMessage(LOG_INFO, message);
    }

    void LogManager::logError(const std::string& message)
    {
        logMessage(LOG_ERROR, message);
    }

    void LogManager::logWarning(const std::string& message)
    {
        logMessage(LOG_WARNING, message);
    }

    void LogManager::destoryLog(const std::string& name)
    {
        auto log = _logs.find(name);
        if (log != _logs.end())
        {
            if (_defeaultLog == log->second)
            {
                _defeaultLog = nullptr;
            }

            _logs.erase(log);
        }

        if (nullptr == _defeaultLog && !_logs.empty())
        {
            _defeaultLog = _logs.begin()->second;
        }
    }

    LogPtr LogManager::getLog(const std::string& name) const
    {
        auto log = _logs.find(name);
        return log == _logs.end() ? nullptr : log->second;
    }
}
/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LOGGER_H_
#define _TOMIC_LOGGER_H_

#include <tomic/Common.h>
#include <memory>

TOMIC_BEGIN

enum class LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL,
    COUNT
};

class ILogger
{
public:
    virtual ~ILogger() = default;

    virtual void Log(LogLevel level, const char* format, ...) = 0;
    virtual int Count(LogLevel level) = 0;

    // Only log level higher than or equal to the given level will be logged.
    // But will also count the logs of lower level.
    virtual ILogger* SetLogLevel(LogLevel level) = 0;
    virtual ILogger* SetWriter(twio::IWriterPtr writer) = 0;
};

using ILoggerPtr = std::shared_ptr<ILogger>;

const char* LogLevelToString(LogLevel level);

TOMIC_END

#endif

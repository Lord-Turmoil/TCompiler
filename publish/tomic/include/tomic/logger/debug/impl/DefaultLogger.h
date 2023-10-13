/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_DEFAULT_LOGGER_H_
#define _TOMIC_DEFAULT_LOGGER_H_

#include <tomic/logger/debug/ILogger.h>
#include <memory>

TOMIC_BEGIN

class DefaultLogger;
using DefaultLoggerPtr = std::shared_ptr<DefaultLogger>;

class DefaultLogger : public ILogger
{
public:
    DefaultLogger();
    ~DefaultLogger() override = default;

    static DefaultLoggerPtr New();

    void Log(LogLevel level, const char* format, ...) override;
    int Count(LogLevel level) override;

    DefaultLogger* SetLogLevel(LogLevel level) override
    {
        _level = level;
        return this;
    }

    DefaultLogger* SetWriter(twio::IWriterPtr writer) override
    {
        _writer = writer;
        return this;
    }

private:
    twio::IWriterPtr _writer;
    LogLevel _level;
    int _count[(int)LogLevel::COUNT];
};

TOMIC_END

#endif

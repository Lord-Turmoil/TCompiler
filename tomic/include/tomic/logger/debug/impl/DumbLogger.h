/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_DUMB_LOGGER_H_
#define _TOMIC_DUMB_LOGGER_H_

#include <tomic/logger/debug/ILogger.h>
#include <memory>

TOMIC_BEGIN

// Dumb logger is dumb, so it won't write anything to the writer.
// However, it still remembers the number of logs.

class DumbLogger;
using DumbLoggerPtr = std::shared_ptr<DumbLogger>;


class DumbLogger : public ILogger
{
public:
    DumbLogger();
    ~DumbLogger() override = default;

    static DumbLoggerPtr New();

    void LogFormat(LogLevel level, const char* format, ...) override;
    void LogVFormat(LogLevel level, const char* format, va_list args) override;

    int Count(LogLevel level) override;

    DumbLogger* SetLogLevel(LogLevel level) override { return this; }
    DumbLogger* SetWriter(twio::IWriterPtr writer) override { return this; }

private:
    int _count[static_cast<int>(LogLevel::COUNT)];
};


TOMIC_END

#endif // _TOMIC_DUMB_LOGGER_H_

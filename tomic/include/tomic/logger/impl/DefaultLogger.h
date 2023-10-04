/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_DEFAULT_LOGGER_H_
#define _TOMIC_DEFAULT_LOGGER_H_

#include "../ILogger.h"
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

    DefaultLogger* SetWriter(twio::IWriterPtr writer)
    {
        _writer = writer;
        return this;
    }

    void Log(LogLevel level, const char* format, ...) override;
    int Count(LogLevel level) override;

private:
    twio::IWriterPtr _writer;
    int _count[(int)LogLevel::COUNT];
};

TOMIC_END

#endif

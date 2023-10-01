/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_ILOGGER_H_
#define _TOMIC_ILOGGER_H_

#include "../Common.h"

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
};

TOMIC_END

#endif

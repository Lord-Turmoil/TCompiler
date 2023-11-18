/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/logger/debug/ILogger.h>

TOMIC_BEGIN

const char* LogLevelToString(LogLevel level)
{
    switch (level)
    {
    case LogLevel::DEBUG:
        return "Debug";
    case LogLevel::INFO:
        return "Info";
    case LogLevel::WARNING:
        return "Warning";
    case LogLevel::ERROR:
        return "Error";
    case LogLevel::FATAL:
        return "Fatal";
    default:
        return "Unknown";
    }
}


TOMIC_END

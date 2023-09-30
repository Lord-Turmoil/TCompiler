/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../../include/tomic/logger/impl/DefaultLogger.h"

TOMIC_BEGIN

DefaultLogger::DefaultLogger() : _count{0}
{
}

void DefaultLogger::Log(LogLevel level, const char* format, ...)
{
    _count[(int)level]++;

    if (_writer == nullptr)
    {
        return;
    }

    _writer->WriteFormat("[%s] ", _LogLevelToString(level));
    va_list args;
    va_start(args, format);
    _writer->WriteFormat(format, args);
    va_end(args);

    _writer->Write("\n");
}

const char* DefaultLogger::_LogLevelToString(LogLevel level)
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

int DefaultLogger::Count(LogLevel level)
{
    return _count[(int)level];
}

TOMIC_END

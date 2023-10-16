/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/logger/debug/impl/DefaultLogger.h>

TOMIC_BEGIN

DefaultLogger::DefaultLogger() : _count{ 0 }, _level(LogLevel::DEBUG)
{
}

DefaultLoggerPtr DefaultLogger::New()
{
    return std::make_shared<DefaultLogger>();
}

void DefaultLogger::LogFormat(LogLevel level, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    LogVFormat(level, format, args);
    va_end(args);
}

void DefaultLogger::LogVFormat(LogLevel level, const char* format, va_list args)
{
    _count[(int)level]++;

    if (_writer == nullptr || (level < _level))
    {
        return;
    }

    _writer->WriteFormat("[%s] ", LogLevelToString(level));
    _writer->WriteFormat(format, args);

    _writer->Write("\n");
}

int DefaultLogger::Count(LogLevel level)
{
    return _count[(int)level];
}

TOMIC_END

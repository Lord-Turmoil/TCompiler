/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/logger/debug/impl/DumbLogger.h>

TOMIC_BEGIN

DumbLogger::DumbLogger() : _count{ 0 }
{
}

DumbLoggerPtr DumbLogger::New()
{
    return std::make_shared<DumbLogger>();
}

void DumbLogger::LogFormat(LogLevel level, const char* format, ...)
{
    LogVFormat(level, format, nullptr);
}

void DumbLogger::LogVFormat(LogLevel level, const char* format, va_list args)
{
    _count[(int)level]++;
}

int DumbLogger::Count(LogLevel level)
{
    return _count[(int)level];
}

TOMIC_END

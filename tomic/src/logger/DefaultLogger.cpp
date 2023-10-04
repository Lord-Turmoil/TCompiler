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

DefaultLoggerPtr DefaultLogger::New()
{
    return std::make_shared<DefaultLogger>();
}

void DefaultLogger::Log(LogLevel level, const char* format, ...)
{
    _count[(int)level]++;

    if (_writer == nullptr)
    {
        return;
    }

    _writer->WriteFormat("[%s] ", LogLevelToString(level));
    va_list args;
    va_start(args, format);
    _writer->WriteFormat(format, args);
    va_end(args);

    _writer->Write("\n");
}

int DefaultLogger::Count(LogLevel level)
{
    return _count[(int)level];
}

TOMIC_END

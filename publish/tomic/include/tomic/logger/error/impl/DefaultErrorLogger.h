/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _DEFAULT_ERROR_LOGGER_
#define _DEFAULT_ERROR_LOGGER_

#include <tomic/logger/error/IErrorLogger.h>
#include <tomic/logger/error/IErrorMapper.h>
#include <memory>
#include <string>

TOMIC_BEGIN

class DefaultErrorEntry
{
public:
    DefaultErrorEntry(int line, int column, ErrorType type, const char* msg)
            : _line(line), _column(column), _type(type)
    {
        if (msg)
        {
            _msg = msg;
        }
    }

    int _line;
    int _column;
    ErrorType _type;
    std::string _msg;
};

class DefaultErrorLogger : public IErrorLogger
{
public:
    DefaultErrorLogger(IErrorMapperPtr mapper);
    ~DefaultErrorLogger() override = default;

    virtual void Log(int line, int column, ErrorType type, const char* msg) override;
    virtual void LogFormat(int line, int column, ErrorType type, const char* format, ...) override;

    virtual void Dumps(twio::IWriterPtr writer) override;

    virtual int Count() override;

private:
    IErrorMapperPtr _mapper;
    std::vector<DefaultErrorEntry> _entries;
};

TOMIC_END

#endif // _DEFAULT_ERROR_LOGGER_

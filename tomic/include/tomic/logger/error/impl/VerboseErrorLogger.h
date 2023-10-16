/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_VERBOSE_ERROR_LOGGER_
#define _TOMIC_VERBOSE_ERROR_LOGGER_

#include <tomic/logger/error/IErrorLogger.h>
#include <tomic/logger/error/IErrorMapper.h>
#include <memory>
#include <string>
#include <vector>

TOMIC_BEGIN

class VerboseErrorEntry
{
public:
    VerboseErrorEntry(int line, int column, ErrorType type, const char* msg)
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

class VerboseErrorLogger : public IErrorLogger
{
public:
    VerboseErrorLogger(IErrorMapperPtr mapper);
    ~VerboseErrorLogger() override = default;

    virtual void LogFormat(int line, int column, ErrorType type, const char* format, ...) override;
    virtual void LogVFormat(int line, int column, ErrorType type, const char* format, va_list args) override;

    virtual void Dumps(twio::IWriterPtr writer) override;

    virtual int Count() override;

private:
    IErrorMapperPtr _mapper;
    std::vector<VerboseErrorEntry> _entries;
};

TOMIC_END

#endif // _TOMIC_VERBOSE_ERROR_LOGGER_

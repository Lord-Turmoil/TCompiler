/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _DEFAULT_ERROR_LOGGER_
#define _DEFAULT_ERROR_LOGGER_

#include "../IErrorLogger.h"
#include "../IErrorMapper.h"
#include <memory>

TOMIC_BEGIN

class DefaultErrorLogger : public IErrorLogger
{
public:
    DefaultErrorLogger(IErrorMapperPtr mapper);
    ~DefaultErrorLogger() override = default;

    virtual void Log(int line, int column, ErrorType type, const char* msg) override;
    virtual void LogFormat(int line, int column, ErrorType type, const char* format, ...) override;

    virtual void Dumps(twio::IWriterPtr writer) override;

    // Using PIMPL idiom to hide the implementation details
    class DefaultErrorEntry;

private:
    IErrorMapperPtr _mapper;

    std::vector<std::unique_ptr<DefaultErrorEntry>> _entries;
};

TOMIC_END

#endif // _DEFAULT_ERROR_LOGGER_

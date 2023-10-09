/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_STANDARD_ERROR_LOGGER_H_
#define _TOMIC_STANDARD_ERROR_LOGGER_H_

#include "../IErrorLogger.h"
#include "../IErrorMapper.h"
#include <memory>
#include <vector>

TOMIC_BEGIN

class StandardErrorLogger : public IErrorLogger
{
public:
    StandardErrorLogger(IErrorMapperPtr mapper);
    ~StandardErrorLogger() override = default;

    virtual void Log(int line, int column, ErrorType type, const char* msg) override;

    virtual void Dumps(twio::IWriterPtr writer) override;

private:
    IErrorMapperPtr _mapper;

    // Using PIMPL idiom to hide the implementation details
    class StandardErrorEntry;
    std::vector<std::unique_ptr<StandardErrorEntry>> _entries;
};

TOMIC_END

#endif // _TOMIC_STANDARD_ERROR_LOGGER_H_

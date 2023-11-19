/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_VERBOSE_ERROR_MAPPER_H_
#define _TOMIC_VERBOSE_ERROR_MAPPER_H_

#include <tomic/logger/error/ErrorType.h>
#include <tomic/logger/error/IErrorMapper.h>

TOMIC_BEGIN

class VerboseErrorMapper final : public IErrorMapper
{
public:
    VerboseErrorMapper();
    ~VerboseErrorMapper() override = default;

    const char* Description(ErrorType type) override;

private:
    const char* _descriptions[static_cast<int>(ErrorType::ERR_COUNT)];
};


TOMIC_END

#endif // _TOMIC_VERBOSE_ERROR_MAPPER_H_

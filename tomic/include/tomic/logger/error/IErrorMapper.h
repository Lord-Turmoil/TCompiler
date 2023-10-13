/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_ERROR_MAPPER_H_
#define _TOMIC_ERROR_MAPPER_H_

#include <tomic/Shared.h>
#include <tomic/logger/error/ErrorType.h>
#include <memory>

TOMIC_BEGIN

class IErrorMapper
{
public:
    virtual ~IErrorMapper() = default;

    virtual const char* Description(ErrorType type) = 0;
};

using IErrorMapperPtr = std::shared_ptr<IErrorMapper>;

TOMIC_END

#endif // _TOMIC_ERROR_MAPPER_H_

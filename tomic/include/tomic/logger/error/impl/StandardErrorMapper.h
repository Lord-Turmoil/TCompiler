/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_STANDARD_ERROR_MAPPER_H_
#define _TOMIC_STANDARD_ERROR_MAPPER_H_

#include <tomic/logger/error/IErrorMapper.h>

TOMIC_BEGIN

class StandardErrorMapper final : public IErrorMapper
{
public:
    StandardErrorMapper();
    ~StandardErrorMapper() override = default;

    const char* Description(ErrorType type) override;

private:
    const char* _descriptions[static_cast<int>(ErrorType::ERR_COUNT)];
};


TOMIC_END

#endif // _TOMIC_STANDARD_ERROR_MAPPER_H_

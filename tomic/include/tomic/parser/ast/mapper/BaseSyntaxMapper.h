/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_BASE_SYNTAX_MAPPER_H_
#define _TOMIC_BASE_SYNTAX_MAPPER_H_

#include "ISyntaxMapper.h"

TOMIC_BEGIN

// Still abstract.
class BaseSyntaxMapper : public ISyntaxMapper
{
public:
    ~BaseSyntaxMapper() override = default;

    const char* Description(SyntaxType type) const override;

protected:
    std::unordered_map<SyntaxType, const char*> _typeToDescription;
};

TOMIC_END

#endif // _TOMIC_BASE_SYNTAX_MAPPER_H_

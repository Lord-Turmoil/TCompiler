/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_SYNTAX_MAPPER_H_
#define _TOMIC_SYNTAX_MAPPER_H_

#include <tomic/parser/ast/SyntaxType.h>

TOMIC_BEGIN

class ISyntaxMapper
{
public:
    virtual ~ISyntaxMapper() = default;

    virtual const char* Description(SyntaxType type) const = 0;
};


using ISyntaxMapperPtr = std::shared_ptr<ISyntaxMapper>;

TOMIC_END

#endif // _TOMIC_SYNTAX_MAPPER_H_

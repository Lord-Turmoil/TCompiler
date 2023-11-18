/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/parser/ast/mapper/BaseSyntaxMapper.h>

TOMIC_BEGIN

const char* BaseSyntaxMapper::Description(SyntaxType type) const
{
    auto iter = _typeToDescription.find(type);
    if (iter == _typeToDescription.end())
    {
        return nullptr;
    }
    return iter->second;
}


TOMIC_END

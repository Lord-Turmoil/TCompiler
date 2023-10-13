/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_REDUCE_SYNTAX_MAPPER_H_
#define _TOMIC_REDUCE_SYNTAX_MAPPER_H_

#include <tomic/parser/ast/mapper/BaseSyntaxMapper.h>
#include <memory>

TOMIC_BEGIN

class ReducedSyntaxMapper final : public BaseSyntaxMapper
{
public:
    ReducedSyntaxMapper();
    ~ReducedSyntaxMapper() override = default;

private:
    void _Init();
};

using ReducedSyntaxMapperPtr = std::shared_ptr<ReducedSyntaxMapper>;

TOMIC_END

#endif // _TOMIC_REDUCE_SYNTAX_MAPPER_H_

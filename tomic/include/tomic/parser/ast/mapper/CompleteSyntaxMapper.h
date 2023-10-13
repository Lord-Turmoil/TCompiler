/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_COMPLETE_SYNTAX_MAPPER_H_
#define _TOMIC_COMPLETE_SYNTAX_MAPPER_H_

#include <tomic/parser/ast/mapper/BaseSyntaxMapper.h>
#include <memory>

TOMIC_BEGIN

class CompleteSyntaxMapper final : public BaseSyntaxMapper
{
public:
    CompleteSyntaxMapper();
    ~CompleteSyntaxMapper() override = default;

    const char* Description(SyntaxType type) const override;

private:
    void _Init();
};

using CompleteSyntaxMapperPtr = std::shared_ptr<CompleteSyntaxMapper>;

TOMIC_END

#endif // _TOMIC_COMPLETE_SYNTAX_MAPPER_H_

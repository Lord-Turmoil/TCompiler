/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_SEMANTIC_PARSER_H_
#define _TOMIC_SEMANTIC_PARSER_H_

#include "../Common.h"
#include "ast/SyntaxTree.h"
#include "table/SymbolTable.h"

TOMIC_BEGIN

class ISemanticParser
{
public:
    virtual ~ISemanticParser() = default;

    virtual SymbolTablePtr Parse(SyntaxTreePtr tree) = 0;
};

TOMIC_END

#endif // _TOMIC_SEMANTIC_PARSER_H_

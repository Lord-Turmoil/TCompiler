/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_SEMANTIC_ANALYZER_H_
#define _TOMIC_SEMANTIC_ANALYZER_H_

#include <tomic/Shared.h>
#include <tomic/parser/ast/SyntaxTree.h>
#include <tomic/parser/table/SymbolTable.h>
#include <memory>

TOMIC_BEGIN

class ISemanticAnalyzer
{
public:
    virtual ~ISemanticAnalyzer() = default;

    virtual SymbolTablePtr Analyze(SyntaxTreePtr tree) = 0;
};


using ISemanticAnalyzerPtr = std::shared_ptr<ISemanticAnalyzer>;

TOMIC_END

#endif // _TOMIC_SEMANTIC_ANALYZER_H_

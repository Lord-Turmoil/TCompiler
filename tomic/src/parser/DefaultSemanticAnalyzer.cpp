/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../../include/tomic/parser/impl/DefaultSemanticAnalyzer.h"

TOMIC_BEGIN

DefaultSemanticAnalyzer::DefaultSemanticAnalyzer(IErrorLoggerPtr logger)
        : _logger(logger)
{
    TOMIC_ASSERT(_logger);
}

SymbolTablePtr DefaultSemanticAnalyzer::Analyze(SyntaxTreePtr tree)
{
    TOMIC_ASSERT(tree);
    _table = SymbolTable::New();

    tree->Accept(this);

    return _table;
}

TOMIC_END

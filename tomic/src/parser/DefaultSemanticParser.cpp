/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/parser/impl/DefaultSemanticParser.h>

TOMIC_BEGIN

DefaultSemanticParser::DefaultSemanticParser(ISemanticAnalyzerPtr analyzer, ILoggerPtr logger)
    : _analyzer(analyzer), _logger(logger)
{
    TOMIC_ASSERT(_analyzer);
    TOMIC_ASSERT(_logger);
}


SymbolTablePtr DefaultSemanticParser::Parse(SyntaxTreePtr tree)
{
    TOMIC_ASSERT(tree);

    auto table = _analyzer->Analyze(tree);

    return table;
}


TOMIC_END

/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_DEFAULT_SEMANTIC_PARSER_H_
#define _TOMIC_DEFAULT_SEMANTIC_PARSER_H_

#include "../../Common.h"
#include "../ISemanticParser.h"
#include "../ISemanticAnalyzer.h"
#include "../ast/AstVisitor.h"
#include "../../logger/debug/ILogger.h"

TOMIC_BEGIN

class DefaultSemanticParser : public ISemanticParser
{
public:
    DefaultSemanticParser(ISemanticAnalyzerPtr analyzer, ILoggerPtr logger);
    ~DefaultSemanticParser() override = default;

    SymbolTablePtr Parse(SyntaxTreePtr tree) override;

private:
    ISemanticAnalyzerPtr _analyzer;
    ILoggerPtr _logger;
};

TOMIC_END

#endif // _TOMIC_DEFAULT_SEMANTIC_PARSER_H_

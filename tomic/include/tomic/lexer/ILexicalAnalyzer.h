/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LEXICAL_ANALYSER_H_
#define _TOMIC_LEXICAL_ANALYSER_H_

#include <tomic/Shared.h>
#include <tomic/lexer/token/Token.h>
#include <memory>

TOMIC_BEGIN

class ILexicalAnalyzer
{
public:
    virtual ~ILexicalAnalyzer() = default;

    virtual ILexicalAnalyzer* SetReader(twio::IAdvancedReaderPtr reader) = 0;

    virtual TokenPtr Next() = 0;
};

using ILexicalAnalyzerPtr = std::shared_ptr<ILexicalAnalyzer>;

TOMIC_END

#endif // _TOMIC_LEXICAL_ANALYZER_H_

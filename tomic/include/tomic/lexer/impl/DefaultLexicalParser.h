/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_DEFULAT_LEXICAL_PARSER_H_
#define _TOMIC_DEFULAT_LEXICAL_PARSER_H_

#include "../../Common.h"
#include "../ILexicalParser.h"
#include "../ILexicalAnalyzer.h"
#include <vector>

TOMIC_BEGIN

class DefaultLexicalParser : public ILexicalParser
{
public:
    DefaultLexicalParser(ILexicalAnalyserPtr analyser);
    virtual ~DefaultLexicalParser() = default;

    DefaultLexicalParser* SetReader(twio::IAdvancedReaderPtr reader) override;

    TokenPtr Next() override;
    TokenPtr Rewind() override;

private:
    ILexicalAnalyserPtr _analyser;
    std::vector<TokenPtr> _tokens;
    std::vector<TokenPtr>::iterator _current;
};

TOMIC_END

#endif // _TOMIC_DEFULAT_LEXICAL_PARSER_H_

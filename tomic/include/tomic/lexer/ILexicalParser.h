/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LEXICAL_PARSER_H_
#define _TOMIC_LEXICAL_PARSER_H_

#include "../Common.h"
#include "ILexicalAnalyzer.h"
#include <memory>

TOMIC_BEGIN

// Lexical parse will guarantee that the token is valid.
class ILexicalParser
{
public:
    virtual ~ILexicalParser() = default;

    virtual ILexicalParser* SetReader(twio::IAdvancedReaderPtr reader) = 0;

    // Shall not return nullptr.
    virtual TokenPtr Current() = 0;
    virtual TokenPtr Next() = 0;
    virtual TokenPtr Rewind() = 0;

    virtual int SetCheckPoint() = 0;
    virtual void Rollback(int checkpoint) = 0;
};

using ILexicalParserPtr = std::shared_ptr<ILexicalParser>;

TOMIC_END

#endif // _TOMIC_LEXICAL_PARSER_H_

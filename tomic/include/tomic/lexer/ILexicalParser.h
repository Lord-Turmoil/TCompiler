/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_ILEXICAL_PARSER_H_
#define _TOMIC_ILEXICAL_PARSER_H_

#include "../Defines.h"
#include "ILexicalAnalyser.h"
#include "../../../../twio/include/twio/core/IReader.h"
#include "../../../../twio/include/twio/core/IWriter.h"
#include <memory>

TOMIC_BEGIN

// Lexical parse will guarantee that the token is valid.
class ILexicalParser
{
public:
    virtual ~ILexicalParser() = default;

    virtual ILexicalParser* SetReader(twio::IAdvancedReaderPtr reader) = 0;

    virtual TokenPtr Next() = 0;
    virtual TokenPtr Rewind() = 0;
};

using ILexicalParserPtr = std::shared_ptr<ILexicalParser>;

TOMIC_END

#endif // _TOMIC_ILEXICAL_PARSER_H_

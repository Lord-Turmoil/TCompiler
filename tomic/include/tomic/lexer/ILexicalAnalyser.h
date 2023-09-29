/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_ILEXICAL_ANALYSER_H_
#define _TOMIC_ILEXICAL_ANALYSER_H_

#include "../Defines.h"
#include "../../../../twio/include/twio/core/IReader.h"
#include "token/Token.h"

TOMIC_BEGIN

class ILexicalAnalyser
{
public:
    virtual ~ILexicalAnalyser() = default;

    virtual ILexicalAnalyser* SetReader(twio::IAdvancedReaderPtr reader) = 0;

    virtual TokenPtr Next() = 0;
};

TOMIC_END

#endif // _TOMIC_ILEXICAL_ANALYSER_H_

/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TMOIC_ISYNTACTIC_PARSER_H_
#define _TMOIC_ISYNTACTIC_PARSER_H_

#include "../Common.h"
#include <memory>

TOMIC_BEGIN

class SyntacticTree;
using SyntacticTreePtr = std::shared_ptr<SyntacticTree>;

class ISyntacticParser
{
public:
    virtual ~ISyntacticParser() = default;

    virtual ISyntacticParser* SetReader(twio::IAdvancedReaderPtr reader) = 0;

    virtual SyntacticTreePtr Parse() = 0;
};

TOMIC_END

#endif // _TMOIC_ISYNTACTIC_PARSER_H_

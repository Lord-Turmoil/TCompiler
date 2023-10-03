/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

// Common header files for tomic.

#include "tomic/lexer/IPreprocessor.h"
#include "tomic/lexer/impl/DefaultPreprocessor.h"

#include "tomic/lexer/token/ITokenMapper.h"
#include "tomic/lexer/impl/token/DefaultTokenMapper.h"

#include "tomic/lexer/ILexicalAnalyzer.h"
#include "tomic/lexer/impl/DefaultLexicalAnalyzer.h"
#include "tomic/lexer/ILexicalParser.h"
#include "tomic/lexer/impl/DefaultLexicalParser.h"

#include "tomic/parser/ISyntacticParser.h"
#include "tomic/parser/impl/DefaultSyntacticParser.h"

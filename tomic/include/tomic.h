/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

// Common header files for tomic.

// lexer
#include "tomic/lexer/IPreprocessor.h"
#include "tomic/lexer/impl/DefaultPreprocessor.h"

#include "tomic/lexer/token/ITokenMapper.h"
#include "tomic/lexer/impl/token/DefaultTokenMapper.h"

#include "tomic/lexer/ILexicalAnalyzer.h"
#include "tomic/lexer/impl/DefaultLexicalAnalyzer.h"
#include "tomic/lexer/ILexicalParser.h"
#include "tomic/lexer/impl/DefaultLexicalParser.h"

// parser
#include "tomic/parser/ISyntacticParser.h"
#include "tomic/parser/impl/DefaultSyntacticParser.h"

#include "tomic/parser/ast/SyntaxType.h"
#include "tomic/parser/ast/mapper/ISyntaxMapper.h"
#include "tomic/parser/ast/mapper/CompleteSyntaxMapper.h"
#include "tomic/parser/ast/mapper/ReducedSyntaxMapper.h"

#include "tomic/parser/ast/printer/IAstPrinter.h"
#include "tomic/parser/ast/printer/XmlAstPrinter.h"
#include "tomic/parser/ast/printer/JsonAstPrinter.h"
#include "tomic/parser/ast/printer/StandardAstPrinter.h"

// logger
#include "tomic/logger/ILogger.h"
#include "tomic/logger/impl/DefaultLogger.h"
#include "tomic/logger/impl/DumbLogger.h"

// others
#include "tomic/utils/Config.h"
#include "tomic/utils/StringUtil.h"

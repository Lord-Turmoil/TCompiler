/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_SYMBOL_TABLE_UTIL_H_
#define _TOMIC_SYMBOL_TABLE_UTIL_H_

#include <tomic/Shared.h>
#include <tomic/parser/table/SymbolTableForward.h>
#include <tomic/parser/table/SymbolTableEntry.h>
#include <tomic/parser/ast/SyntaxNode.h>
#include <string>
#include <utility>

TOMIC_BEGIN

namespace SymbolTableUtil
{

// Used for illegal defines.
std::string GetUniqueName();

// This node should be a FuncFParams.
void BuildParamVariableEntries(SyntaxNodePtr funcParams,
                               std::vector<std::pair<SyntaxNodePtr, VariableEntryPtr>>& list);

}

TOMIC_END

#endif // _TOMIC_SYMBOL_TABLE_UTIL_H_

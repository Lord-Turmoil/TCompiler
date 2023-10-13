/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_SYMBOL_TABLE_FORWARD_H_
#define _TOMIC_SYMBOL_TABLE_FORWARD_H_

#include <tomic/Shared.h>

TOMIC_BEGIN

class SymbolTable;
using SymbolTablePtr = std::shared_ptr<SymbolTable>;

class SymbolTableBlock;
using SymbolTableBlockPtr = std::shared_ptr<SymbolTableBlock>;

class SymbolTableEntry;
using SymbolTableEntryPtr = std::shared_ptr<SymbolTableEntry>;

TOMIC_END

#endif // _TOMIC_SYMBOL_TABLE_FORWARD_H_

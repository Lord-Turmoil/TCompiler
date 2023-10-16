/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_SYMBOL_TABLE_H_
#define _TOMIC_SYMBOL_TABLE_H_

#include <tomic/Shared.h>
#include <tomic/parser/table/SymbolTableForward.h>
#include <memory>
#include <unordered_map>

TOMIC_BEGIN

class SymbolTable
{
public:
    SymbolTable() : _nextId(0) {}
    ~SymbolTable() = default;

    static SymbolTablePtr New()
    {
        return std::make_shared<SymbolTable>();
    }

    // Just remember the block you created, you can only get it
    // via id later. :(
    SymbolTableBlockPtr NewRoot();
    SymbolTableBlockPtr NewBlock(SymbolTableBlockPtr parent);

    SymbolTableBlockPtr GetBlock(int id) const;

private:
    std::unordered_map<int, SymbolTableBlockSmartPtr> _blocks;
    int _nextId;
};

TOMIC_END

#endif // _TOMIC_SYMBOL_TABLE_H_

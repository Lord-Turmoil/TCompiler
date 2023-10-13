/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/parser/table/SymbolTable.h>
#include <tomic/parser/table/SymbolTableBlock.h>
#include <tomic/parser/table/SymbolTableEntry.h>

TOMIC_BEGIN

SymbolTableBlockPtr SymbolTable::NewRoot()
{
    return SymbolTableBlock::New(_nextId++, this, nullptr);
}

SymbolTableBlockPtr SymbolTable::NewBlock(SymbolTableBlockPtr parent)
{
    return SymbolTableBlock::New(_nextId++, this, parent);
}

SymbolTableBlockPtr SymbolTable::GetBlock(int id) const
{
    auto it = _blocks.find(id);
    if (it != _blocks.end())
    {
        return it->second;
    }

    return nullptr;
}

TOMIC_END

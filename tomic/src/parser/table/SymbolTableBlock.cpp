/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/parser/table/SymbolTable.h>
#include <tomic/parser/table/SymbolTableBlock.h>
#include <tomic/parser/table/SymbolTableEntry.h>

TOMIC_BEGIN

SymbolTableBlockPtr SymbolTableBlock::NewChild()
{
    return _table->NewBlock(this);
}


SymbolTableBlockPtr SymbolTableBlock::AddEntry(SymbolTableEntryPtr entry)
{
    TOMIC_ASSERT(entry);

    _entries.push_back(entry);

    return this;
}


SymbolTableEntryPtr SymbolTableBlock::FindEntry(const std::string& name) const
{
    auto entry = FindLocalEntry(name);
    if (entry)
    {
        return entry;
    }

    if (_parent)
    {
        return _parent->FindEntry(name);
    }

    return nullptr;
}


SymbolTableEntryPtr SymbolTableBlock::FindLocalEntry(const std::string& name) const
{
    for (auto entry : _entries)
    {
        if (entry->Name() == name)
        {
            return entry;
        }
    }

    return nullptr;
}


TOMIC_END

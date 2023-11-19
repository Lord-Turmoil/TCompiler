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


SymbolTableBlockPtr SymbolTableBlock::AddEntry(SymbolTableEntrySmartPtr entry)
{
    TOMIC_ASSERT(entry);

    _entries.push_back(entry);

    return this;
}


SymbolTableEntrySmartPtr SymbolTableBlock::FindEntry(const std::string& name) const
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


SymbolTableEntrySmartPtr SymbolTableBlock::FindLocalEntry(const std::string& name) const
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

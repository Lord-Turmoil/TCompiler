/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_SYMBOL_TABLE_BLOCK_H_
#define _TOMIC_SYMBOL_TABLE_BLOCK_H_

#include <tomic/parser/table/SymbolTableForward.h>
#include <tomic/Shared.h>

#include <memory>
#include <string>
#include <vector>

TOMIC_BEGIN

class SymbolTableBlock
{
    friend class SymbolTable;

public:
    SymbolTableBlockPtr NewChild();

    int Id() const { return _id; }
    SymbolTableBlockPtr Parent() const { return _parent; }

    // Warning: Add entry do not check legality.
    SymbolTableBlockPtr AddEntry(SymbolTableEntryPtr entry);

    // Find entry in this block and its ancestors.
    SymbolTableEntryPtr FindEntry(const std::string& name) const;
    // Find entry in this block only.
    SymbolTableEntryPtr FindLocalEntry(const std::string& name) const;

private:
    SymbolTableBlock(int id, SymbolTable* table, SymbolTableBlockPtr parent)
        : _id(id), _table(table), _parent(parent)
    {
        TOMIC_ASSERT(_table);
    }


    static SymbolTableBlockSmartPtr New(int id, SymbolTable* table, SymbolTableBlockPtr parent)
    {
        return std::shared_ptr<SymbolTableBlock>(new SymbolTableBlock(id, table, parent));
    }

private:
    int _id;

    SymbolTable* _table;
    SymbolTableBlockPtr _parent;

    std::vector<SymbolTableEntryPtr> _entries;
};


TOMIC_END

#endif // _TOMIC_SYMBOL_TABLE_BLOCK_H_
